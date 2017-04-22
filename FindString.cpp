#include "String.h"
#include "Functions.h"

using namespace doxyCraft;

size_t String::find_last_of(const char * str, size_t pos, size_t n) const
{
	size_t rfnd, ret = 0;
	bool found = false;
	String find_str(str);
	for ( size_t i = 0; i < find_str.length();  i++ )
	{
		size_t symbol_len = UTF8Functions::getCodepointLen(find_str[i].c_str(), 0);
		char f_str[symbol_len];
		memcpy (f_str, find_str[i].c_str(), symbol_len);
		f_str[symbol_len] = 0;
		
		if  ( String::npos != (rfnd = rfind(
				f_str, length() - calcSizeChar(f_str) - symbol_len,
				UTF8Functions::calcLengthStr(find_str[i].c_str(), 0, 1))))
		{
			found = true;
			if ( ret < rfnd )
			{
				ret = rfnd;
			}
		}
	}
	return found ? ret: String::npos;
}

size_t String::find_last_of(const char * str, size_t pos) const
{
	return find_last_of ( str, pos, UTF8Functions::calcLengthStr( str, 0, calcSizeChar(str) ) );
}

size_t String::find_last_of(const String &str, size_t pos) const
{
	return find_last_of ( str.c_str(), pos, str.length() );
}

size_t String::rfind(String &str) const
{
	return rfind(str.c_str());
}

size_t String::rfind(const char *str, size_t npos) const
{
	size_t i, len;
	for ( len=0, i=0; i<calcSizeChar(str); ++len, i+=UTF8Functions::getCodepointLen(str, i) );
	return rfind(str, npos, len);
}

size_t String::rfind(const char *str, size_t npos, size_t n) const
{
	if (n<=length()) npos = std::min(size_t(length()-n), npos);
	do if (!memcmp(_str + UTF8Functions::getRawIndex(_str, npos), str, n)) return npos; while(npos-->0);
	return String::npos;
}

size_t String::find(String &str) const
{
	return find(str.c_str(), 0);
}

size_t String::find(const char* str, size_t npos, size_t n) const
{
	const char* str_;
	if( String::npos == n )
	{
		str_ = str;
	}
	else
	{
		String str_aux;
		str_aux.set(str, UTF8Functions::getRawIndex(str, n));
		str_ = str_aux.c_str();
	}
	
	size_t border = calcSizeChar(str_) - 1;
	size_t _size  = getCountChar();
	size_t i, j, k;
	
	auto offset_calc = [&](size_t &idx, size_t &position)
	{
		const size_t pos = position;
		idx = position = 0;
		for( size_t i = 0; i < pos; i++ )
		{
			idx += UTF8Functions::getCodepointLen(_str, idx);
			position++;
		}
	};
	
	for( offset_calc(i, npos); i <= _size; i += UTF8Functions::getCodepointLen(_str, i), ++npos )
	{
		for( j = i, k = 0; j <= _size && k <= border; ++j, ++k )
		{
			if( _str[j] != str_[k] )
			{
				break;
			}
			if( k == border )
			{
				return npos;
			}
		}
	}
	return String::npos;
}

