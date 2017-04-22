#ifndef STRING_H
#define STRING_H

#include <algorithm>
#include <cstring>
#include "Iterator.h"
#include <string>
#include <limits>
#include <vector>
#include "../../../Shared/Macros.h"
#include "UTF8Functions.h"
#include "StringOperators.h"
#include <boost/locale/generator.hpp>
#include <boost/locale/conversion.hpp>

namespace doxyCraft
{
	class String;
	
class WideCharacter
	{
	public:
		WideCharacter(char* = nullptr, String* = nullptr);
		WideCharacter(const WideCharacter &rhs);
		virtual ~WideCharacter() = default;
		
		WideCharacter &operator=(int);
		WideCharacter &operator=(char);
		WideCharacter &operator=(const WideCharacter &rhs)
		{
			str_ = rhs.str_;
			std::copy(rhs.bytesArray, rhs.bytesArray+MaxUTF8CharLen, bytesArray);
			return *this;
		}
		
		bool operator==(int symbol) const;
		inline bool operator==(char symbol) const { return symbol == **bytesArray; }
		bool operator==(char const* str) const;
		
		inline bool operator!=(int symbol) const { return !operator==(symbol); }
		inline bool operator!=(char symbol) const { return !operator==(symbol); }
		inline bool operator!=(char const* symbol) const { return !operator==(symbol); }
		
		inline operator char &() { return **bytesArray; }
		inline char* operator&() { return *bytesArray; }
		
		inline char* c_str() { return *bytesArray; };
	
	private:
		
		char* bytesArray[MaxUTF8CharLen];
		String* str_;
		
		friend std::ostream &operator<<(std::ostream &os, const WideCharacter &rhs)
		{
			short actual_size
					      {
							      static_cast <short>
							      (
									      UTF8Functions::getCodepointLen
											      (*rhs.bytesArray, 0)
							      )
					      };
			char  ptr[actual_size + 1];
			ptr[actual_size] = 0;
			for( short i = 0; i < actual_size; i++ )
				*(ptr + i) = **(rhs.bytesArray + i);
			os<<ptr;
			return os;
		}
		
		unsigned little_endian_check() const;
	};
	
	typedef std::vector<String> StringArray;
	
	class String final
	{
	public:
		inline WideCharacter operator[](const size_t &range) const { return WideCharacter( &_str[UTF8Functions::getRawIndex(_str, range)], const_cast<String*>(this) ); }
		
		inline WideCharacter operator[](const size_t &range) { return WideCharacter( &_str[UTF8Functions::getRawIndex(_str, range)], this ); }
		
		class Iterator:public doxyCraft::Iterator
		{
		SYNTHESIZE_READONLY(String*, _obj, Object);
		public:
			Iterator(String* obj):IteratorBase(), _obj(obj)
			{
				
			}
			
			virtual ~Iterator() = default;
			
			virtual Iterator operator+(size_t n) const
			{
				String::Iterator it(*this);
				for( ; n > 0; --n )
					++it;
				return it;
			}
			
			virtual Iterator operator-(size_t n) const
			{
				String::Iterator it(*this);
				for( ; n > 0; --n )
					--it;
				return it;
			}
			
			virtual Iterator &operator++() override
			{
				if( _iteration < _obj->getCountChar())
				{
					_iteration += UTF8Functions::getCodepointLen(_obj->c_str(), _iteration);
				}
				return *this;
			}
			
			virtual Iterator &operator--() override
			{
				if( _iteration > 0 )
				{
					for( size_t i = 0; i < _iteration; )
					{
						size_t codepointLen = UTF8Functions::getCodepointLen(_obj->c_str(), i);
						if( i + codepointLen == _iteration )
						{
							_iteration = i;
							break;
						}
						i += codepointLen;
					}
				}
				return *this;
			}
			
			virtual Iterator &operator+=(size_t right) override
			{
				for( ; right != 0 && _iteration < _obj->getCountChar(); )
				{
					size_t codepointLen = UTF8Functions::getCodepointLen(_obj->c_str(), _iteration);
					_iteration += codepointLen;
					--right;
				}
				return *this;
			}
			
			typename String::Iterator::difference_type operator-(String::Iterator &it) const
			{
				return ((*_obj)[_iteration]).c_str() - (&*it);
			}
			
			virtual WideCharacter operator*()
			{
				return WideCharacter(&(_obj->_str[_iteration]), _obj);
			}
			
			virtual operator char*() const override
			{
				return (*_obj)[_iteration].c_str();
			}
			
			static size_t _distance(Iterator it1, Iterator it2)
			{
				return UTF8Functions::calcLengthStr(it1._obj->_str, it1.getIteration(), it2.getIteration());
			}
		};
		
		typedef Iterator iterator;
		typedef Iterator const_iterator;
		
		static const size_t npos = SIZE_MAX;
		
		String();
		String(size_t size);
		String(const char* str);
		String(const String &other);
		String(String &&other);
		~String();
		
		void clear();
		
		void resize(const size_t newSize);
		
		void set(const char* const str, size_t size);
		
		inline void appened(const String &string, size_t index) { appened(string._str, index, string._countChar); }
		void appened(const char* string, size_t index);
		void appened(const char* str, size_t index, size_t sizeStr);
		
		String &append(const char*);
		inline String &append(const char* str, size_t n) { return append(str, 0, n); }
		String &append(const char*, size_t, size_t);
		
		StringArray split(const char* str, bool saveSplitString = false) const;
		
		const String substr(size_t pos, size_t len) const;
		inline const String substr(size_t pos) const { return substr(pos, _size - pos); }
		
		inline Iterator createIterator() const { return Iterator((String*)this); }
		inline Iterator begin() const { return createIterator(); }
		inline Iterator end() const { return createIterator() + _countChar; }
		inline const Iterator cbegin() const { return createIterator(); }
		inline const Iterator cend() const { return createIterator() + _countChar; }
		
		inline WideCharacter at(size_t n) { return WideCharacter( &_str[UTF8Functions::getRawIndex(_str, n)], this ); }
		
		inline const char* c_str() const { return _str; }
		inline std::string toStdString() const { return std::string(_str); }
		inline explicit operator bool() const { return nullptr != _str; }
		
		inline size_t length() const { return _length; }
		inline size_t size() const { return _size; }
		
		friend std::istream &operator>>(std::istream &is, String &str);
		
		String &erase(size_t pos = 0, size_t len = npos);
		
		template<class InputIter>
		String &erase(InputIter it)
		{
			return erase(Iterator::_distance(begin(), it), 1);
		}
		
		template<class InputIter, typename = RequireIterator<InputIter>>
		String &erase(InputIter first, InputIter last)
		{
			return erase(Iterator::_distance(begin(), first), Iterator::_distance(first, last));
		}
		
		String &operator=(const char* other);
		String &operator=(const String& other);
		String &operator=(String&& other);
		
		String operator+(const char* right) const;
		String operator+(const String &right) const;
		inline String &operator+=(const char* right)
		{
			appened(right, _length);
			return *this;
		}
		inline String &operator+=(const String &right) { return operator+=(right.c_str()); }
		String &operator+=(char c);
		
		enum deleteDuplicate
		{
			beginning = 1,
			middle = 2,
			ending = 4
		};
		
		String &deleteDuplicateTabsAndSpaces(int duplicate);
		
		//need for map
		bool operator<(const String &Sr) const;
		bool operator==(const String &Sr) const;
		
		String &assign(const char* str, size_t n);
		
		inline String &tolower() { return assign(boost::locale::to_lower(_str, boost::locale::generator()("")).c_str(),_countChar); }
		inline String &toupper() { return assign(boost::locale::to_upper(_str, boost::locale::generator()("")).c_str(),_countChar); }
		
		// source to FindString.cpp
		size_t find(const char* str, size_t npos = 0, size_t n = String::npos) const;
		size_t find(String &str) const;
		size_t rfind(const char* str, size_t npos, size_t) const;
		size_t rfind(const char* str, size_t npos = String::npos) const;
		size_t rfind(String &str) const;
		size_t find_last_of(const String &str, size_t pos = String::npos) const;
		size_t find_last_of(const char* str, size_t pos = String::npos) const;
		size_t find_last_of(const char* str, size_t pos, size_t n) const;
		
		// source to ReplaceString.cpp
		String &replace(size_t pos, size_t count, const String &rhs, size_t pos2, size_t count2);
		String &replace(size_t pos, size_t count, const String &rhs);
		String &replace(Iterator beg, Iterator end, Iterator beg2, Iterator end2);
		String &replace(Iterator beg, Iterator end, const char* rhs, size_t n);
		String &replace(Iterator beg, Iterator end, const char* rhs);
		
		inline bool empty() const noexcept
		{ return _length==0; }
		
		friend String &operator+=(String &Sl, const bool val);
		friend String &operator+=(String &Sl, const double val);
		friend String &operator+=(String &Sl, const float val);
		friend String &operator+=(String &Sl, const unsigned int val);
		friend String &operator+=(String &Sl, const int val);
		friend String operator+(const String &Sl, const bool val);
		friend String operator+(const String &str, const double d);
		friend String operator+(const String &str, const float f);
		friend String operator+(const String &str, const unsigned int a);
		friend String operator+(const String &str, const int a);
		friend WideCharacter;
	private:
		char* _str;
		size_t _length;
		size_t _size;
	SYNTHESIZE_READONLY(size_t, _countChar, CountChar);
	};
	
}

namespace std
{
	#include <bits/functional_hash.h>
	
	template<>
	struct hash<doxyCraft::String>
	{
		size_t operator()(const doxyCraft::String &__s) const
		{
			return std::_Hash_impl::hash(__s.c_str(), __s.getCountChar());
		}
	};
}


#endif //STRING_H
