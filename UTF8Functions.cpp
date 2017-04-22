#include "UTF8Functions.h"
#include "Functions.h"

using namespace doxyCraft;

bool UTF8Functions::isValid(const char* str)
{
	return isValid(str, 0, calcSizeChar(str));
}

bool UTF8Functions::isValid(const char* str, const size_t positionBegin)
{
	return isValid(str, positionBegin, calcSizeChar(str));
}

bool UTF8Functions::isValid(const char* str, const size_t positionBegin, const size_t positionEnd)
{
	size_t sizeStr = calcSizeChar(str);
	size_t it      = positionBegin;
	size_t itend   = positionEnd;
	
	if( it > sizeStr || itend > sizeStr )
	{
		return false;
	}
	
	while( it < itend )
	{
		if((0xF8&str[it]) == 0xF0 )
		{
			if( it + 1 == itend ||
			    it + 2 == itend ||
			    it + 3 == itend )
			{
				return false;
			}
			
			if(((0xC0&str[it + 1]) != 0x80) ||
			   ((0xC0&str[it + 2]) != 0x80) ||
			   ((0xC0&str[it + 3]) != 0x80))
			{
				return false;
			}
			
			if( str[it] == '\xF0' )
			{
				if( str[it + 1] < '\x90' || str[it + 1] > '\xBF' )
				{
					return false;
				}
			}
			else if( str[it] == '\xF4' )
			{
				if( str[it + 1] < '\x80' || str[it + 1] > '\x8F' )
				{
					return false;
				}
			}
			
			it += 4;
		}
		else if((0xF0&str[it]) == 0xE0 )
		{
			if((it + 1) == itend || (it + 2) == itend )
			{
				return false;
			}
			
			if(((0xC0&str[it + 1]) != 0x80) ||
			   ((0xC0&str[it + 2]) != 0x80))
			{
				return false;
			}
			
			if( str[it] == '\xE0' )
			{
				if( str[it + 1] < '\xA0' ||
				    str[it + 1] > '\xBF' )
				{
					return false;
				}
			}
			else if( str[it] == '\xED' )
			{
				if( str[it + 1] < '\x80' || str[it + 1] > '\x9F' )
				{
					return false;
				}
			}
			
			it += 3;
		}
		else if((0xE0&str[it]) == 0xC0 )
		{
			if( it + 1 == itend )
			{
				return false;
			}
			
			if((0xC0&str[it + 1]) != 0x80 )
			{
				return false;
			}
			it += 2;
		}
		else if((0x80&str[it]) == 0x00 )
		{
			it += 1;
		}
		else
		{
			return false;
		}
	}
	
	return true;
}

size_t UTF8Functions::calcLengthStr(const char* str, const size_t positionBegin, const size_t positionEnd)
{
	size_t it      = positionBegin;
	size_t itend   = positionEnd;
	size_t ret     = 0;
	
	while( it < itend )
	{
		char byte = str[it];
		if( 0xf0 == (0xf8&byte))
		{
			it += 4;
		}
		else if( 0xe0 == (0xf0&byte))
		{
			it += 3;
		}
		else if( 0xc0 == (0xe0&byte))
		{
			it += 2;
		}
		else
		{
			it += 1;
		}
		++ret;
	}
	return ret;
}

size_t UTF8Functions::getCodepointLen(const char* str, size_t n)
{
	if( 0xf0 == (0xf8&str[n]))
	{
		return 4;
	}
	else if( 0xe0 == (0xf0&str[n]))
	{
		return 3;
	}
	else if( 0xc0 == (0xe0&str[n]))
	{
		return 2;
	}
	return 1;
}

size_t UTF8Functions::getRawIndex(const char* str, size_t pos)
{
	size_t i = 0;
	while (pos)
	{
		i+= getCodepointLen(str, i);
		pos--;
	}
	return i;
}