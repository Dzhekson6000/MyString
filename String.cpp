#include "String.h"
#include "Shared/Allocator/Allocator.h"
#include "Functions.h"
using namespace doxyCraft;

String::String():String((size_t)0)
{
}

String::String(size_t size):
		_str(nullptr),
		_length(0),
		_size(size + 1),
		_countChar(0)
{
	_str = ALLOC_ARRAY<char, AL_DYNAMIC>(_size);
	_str[0] = '\0';
}

String::String(const char* str):String(calcSizeChar(str))
{
	set(str, calcSizeChar(str));
}

String::String(const String &str):String(calcSizeChar(str.c_str()))
{
	set(str._str, calcSizeChar(str._str));
}

String::String(String &&other)
{
	_str = nullptr;
	*this = std::move(other);
}

String::~String()
{
	if( _str )
		FREE_ARRAY(_str);
}

void String::clear()
{
	_length    = 0;
	_size      = 0;
	_countChar = 0;
	_str[0] = '\0';
}

void String::resize(size_t newSize)
{
	_size = newSize + 1;
	if( _str )
	{
		_str = RELOC_ARRAY<char, AL_DYNAMIC>(_str, (unsigned int)_size);
	}
	else
	{
		_str = ALLOC_ARRAY<char, AL_DYNAMIC>((unsigned int)_size);
	}
}

void String::set(const char* const str, size_t size)
{
	if( str == 0 )
	{
		_str = nullptr;
		return;
	}
	_countChar = size;
	if( size + 1 > _size )
	{
		resize((size_t)(size*1.5));
	}
	memcpy(_str, str, size);
	_str[size] = '\0';
	
	_length = UTF8Functions::calcLengthStr(str, 0, size);
	if( !UTF8Functions::isValid(str))
	{
		clear();
	}
}

void String::appened(const char* string, size_t index)
{
	appened(string, index, calcSizeChar(string));
}

void String::appened(const char* str, size_t index, size_t charCnt)
{
	size_t pos = UTF8Functions::getRawIndex(_str, index);
	
	if( pos > _countChar )
	{
		return;
	}
	size_t newSizeStr = _countChar + charCnt;
	if( newSizeStr + 1 > _size )
	{
		resize(newSizeStr);
	}
	if( pos != _countChar && _countChar != 0 )
	{
		for( size_t i = _countChar;
		     i > pos;
		     i-- )
		{
			_str[charCnt + i - 1] = _str[i - 1];
		}
	}
	
	for( size_t i = 0;
	     i < charCnt;
	     i++ )
	{
		_str[pos + i] = str[i];
	}
	
	_countChar += charCnt;
	_str[newSizeStr] = '\0';
	_length += UTF8Functions::calcLengthStr(_str, pos, pos + charCnt);
}

String &String::append(const char* str)
{
	return append(str, 0, calcSizeChar(str));
}

String &String::append(const char* str, size_t index, size_t charCnt)
{
	size_t i, j, stop;
	index   = UTF8Functions::getRawIndex(str, index);
	charCnt = UTF8Functions::getRawIndex(str + index, charCnt);
	size_t sizeStr    = calcSizeChar(str);
	size_t newSizeStr = _size + sizeStr - index;
	
	if( 0 == charCnt || index > sizeStr )
	{
		return *this;
	}
	
	if( newSizeStr > _size )
	{
		resize(newSizeStr);
	}
	
	for( j = _countChar, i = index, stop = 0; stop < charCnt && i < sizeStr; i++, stop++ )
	{
		_str[j++] = str[i];
	}
	
	_length += UTF8Functions::calcLengthStr(str, index, calcSizeChar(str));
	_countChar += charCnt;
	_str[j] = '\0';
	return *this;
}

const String String::substr(size_t pos, size_t len) const
{
	if( pos >= _length )
	{
		return String();
	}
	
	Iterator it = cbegin();
	it += pos;
	size_t posUTF8 = it.getIteration();
	it += len;
	size_t endPosUTF8 = it.getIteration();
	size_t newLen     = UTF8Functions::calcLengthStr(_str, posUTF8, endPosUTF8);
	size_t countChar  = endPosUTF8 - posUTF8;
	
	String ret(countChar);
	
	for( size_t i  = 0; i < countChar; i++ )
	{
		ret._str[i] = _str[posUTF8 + i];
		if( _str[posUTF8 + i] == '\0' )
		{
			break;
		}
	}
	ret._str[countChar] = '\0';
	ret._length = newLen;
	ret._countChar = countChar;
	return ret;
}

String &String::erase(size_t pos, size_t len)
{
	size_t tail_length = ((len < _length) ? (_length - pos - len) : 0);
	return replace(pos, _length - pos, *this, _length - tail_length, tail_length);
}

String &String::operator=(const char* other)
{
	set(other, calcSizeChar(other));
	return *this;
}

String &String::operator=(const String& other)
{
	if( this == &other )
	{
		return *this;
	}
	
	set(other._str, other._countChar);
	return *this;
}

String &String::operator=(String&& other)
{
	if (this != &other)
	{
		if( _str )
		{
			FREE_ARRAY(_str);
		}
		_str = other._str;
		_length = other._length;
		_size = other._size;
		_countChar = other._countChar;
		other._str = nullptr;
	}
	return *this;
}

String String::operator+(const char* right) const
{
	String ret;
	ret = *this;
	ret.appened(right, _length, calcSizeChar(right));
	return ret;
}

String String::operator+(const String &right) const
{
	String ret;
	ret = *this;
	ret.appened(right._str, ret._length, calcSizeChar(right._str));
	return ret;
}

String &String::assign(const char* str, size_t n)
{
	for( size_t i = 0; i < n; ++i )
	{
		if( str[i] == '\0' )
		{
			clear();
			append(str, i);
			return *this;
		}
	}
	
	clear();
	append(str, n);
	return *this;
}

bool String::operator<(const String &Rs) const
{
	return doxyCraft::operator<(*this, Rs);
};

bool String::operator==(const String &Rs) const
{
	return doxyCraft::operator==(*this, Rs);
}

StringArray String::split(const char* str, bool saveSplitString) const
{
	StringArray strings;
	strings.reserve(10);
	for(size_t i = 0, j = 0; i <= _length; ++i)
	{
		if( ((*this)[i] == str) || i == _length )
		{
			!saveSplitString ? strings.emplace_back(substr(j,i - j)) : strings.emplace_back(substr(j,i - j + calcSizeChar(str)));
			j = i + 1;
		}
	}
	return strings;
}

String &String::operator+=(char c)
{
	static size_t storedNewLength;
	size_t symbolSize = UTF8Functions::getCodepointLen(&c, size_t());
	if (1==symbolSize && storedNewLength == 0)
		_length += 1;
	else
		storedNewLength = symbolSize - 1;
	if (storedNewLength)
		--storedNewLength;
	if ( size() < 1+length() )
		resize ( ((1+size())/2) * size() );
	_str[_countChar++] = c;
	_str[_countChar] = '\0';
	return *this;
}

String &String::deleteDuplicateTabsAndSpaces(int duplicate = middle)
{
	if(duplicate & middle)
	{
		for( size_t i = 0; i < _size; ++i )
		{
			auto cr = (*this)[i];
			
			if(cr == ' ' || cr == '\t')
			{
				for( size_t j = i + 1; j < _size; ++j )
				{
					if (i+1>length())
						break;
						
					auto crj = (*this)[j];
					
					if(crj != ' ' && crj != '\t')
					{
						if(j != i + 1)
						{
							erase(i, j - i - 1);
						}
						break;
					}
				}
			}
		}
	}
	if(duplicate & beginning)
	{
		while (true)
		{
			if (!length())
				break;
			if ((*this)[0] == ' ' || (*this)[0] == '\t')
				erase(0, 1);
			else break;
		}
	}
	if(duplicate & ending)
	{
		while(true)
		{
			if (!length())
				break;
			if ((*this)[length()-1] == ' ' || (*this)[length()-1] == '\t')
				erase(length()-1, 1);
			else
				break;
		}
	}
	return *this;
}

/// WideCharacter Class

WideCharacter::WideCharacter(char* ptr, String * str): str_(str)
{
	short i;
	short size = static_cast<short> (
			UTF8Functions::getCodepointLen(ptr, 0)
	);
		
	for ( i = 0; i < size; i++ )
	{
		*(bytesArray + i) = ptr + i;
	}
}

WideCharacter &WideCharacter::operator=(int symbol)
{
	int val;
	short size, i, j{0};
	char wchar[MaxUTF8CharLen]{0};
	
	for ( size = 0, val = symbol; val;
	      val >>= 8*sizeof(char), size++ );
	
	short symbol_length
	{
		static_cast <short> (
			UTF8Functions::getCodepointLen(*bytesArray, 0)
		)
	};
	
	if ( LITTLE_ENDIAN == little_endian_check() )
	{
		std::reverse ( (char*)(&symbol), (char*)(&symbol) + sizeof(int) );
		j = sizeof(int) - size;
	}
	
	size_t position_beg = *bytesArray - str_->_str;
	
	if ( size != symbol_length )
	{
		for ( i = 0; i < size; i++ )
		{
			*(wchar + i) = *( (char*)(&symbol) + i + j );
		}
		String oneCharacterString;
		oneCharacterString.set( str_->_str, position_beg );
		oneCharacterString.append(wchar, 1);
		oneCharacterString.append( str_->substr( position_beg + 1, str_->length( ) ).c_str( ) );
		*str_ = oneCharacterString;
	}
	else
	{
		for ( i = 0; i < size; i++ )
		{
			**( bytesArray + i ) = *( (char*)(&symbol) + i + j );
		}
		
	}
	return *this;
}

WideCharacter &WideCharacter::operator=(char symbol)
{
	short symbol_length
	{
		static_cast <short> (
			UTF8Functions::getCodepointLen(*bytesArray, 0)
		)
	};

	if ( sizeof(char) != symbol_length )
	{
		size_t position_beg = *bytesArray - str_->_str;
		
		for ( size_t i = position_beg + 1; i < calcSizeChar(str_->_str); i++, position_beg++ )
		{
			*(str_->_str + position_beg) = *(str_->_str + position_beg + 1);
		}
		
		str_->_str[str_->getCountChar( ) - 1] = 0;
		str_->_countChar -= symbol_length - sizeof(char);
	}
	
	**bytesArray = symbol;
	
	return *this;
}

bool WideCharacter::operator==(int symbol) const
{
	int val;
	short size, i, j;
	
	for ( size = 0, val = symbol; val;
	      val >>= 8*sizeof(char), size++ );
	
	if ( LITTLE_ENDIAN == little_endian_check() )
	{
		std::reverse ( (char*)(&symbol), (char*)(&symbol) + sizeof(int) );
		j = sizeof(int) - size;
	}
	
	for ( i = 0; i < size; i++ )
	{
		if ( **( bytesArray + i ) != *( (char*)(&symbol) + i + j ) )
		{
			return false;
		}
	}
	
	return true;
}

bool WideCharacter::operator==(char const* str) const
{
	for ( unsigned short i = 0; i < MaxUTF8CharLen && i < calcSizeChar(str); i++ )
	{
		if ( **(bytesArray + i) != *(str + i) )
		{
			return false;
		}
	}
	
	return true;
}

unsigned WideCharacter::little_endian_check() const
{
	unsigned int chk_little_endian = 1;
	return
	(
		*( reinterpret_cast <unsigned char*> (&chk_little_endian) ) ?
		LITTLE_ENDIAN : BIG_ENDIAN
	);

}

WideCharacter::WideCharacter(const WideCharacter &rhs)
{
	operator=(rhs);
}
