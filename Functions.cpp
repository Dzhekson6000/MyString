#include "Functions.h"

#include <fstream>
#include <sstream>

using namespace doxyCraft;

size_t doxyCraft::calcSizeChar(const char* str)
{
	size_t ret;
	for(ret=0; str[ret]!='\0'; ++ret);
	return ret;
}

String doxyCraft::toUTF8String(const char* ansiChars, const unsigned int size)
{
	unsigned long oldSize = size;
	unsigned long newSize = size*sizeof(wchar_t);
	String    ret(newSize);
	newSize += 1;
	auto cp = iconv_open("UTF-8", "ASCII");
	char* tmpCharsIn  = (char*)ansiChars;
	char* tmpCharsOut = (char*)ret.c_str();
	iconv(cp, &tmpCharsIn, &oldSize, &tmpCharsOut, &newSize);
	if( !iconv_close(cp))
	{
		return String();
	}
	return ret;
}


namespace doxyCraft
{
	std::istream &operator>>(std::istream &is, String &str)
	{
		if( is )
		{
			try
			{
				dynamic_cast<std::ifstream&>(is);
				is.seekg(0, is.end);
				std::streampos size = is.tellg();
				String     buff(size);
				is.seekg(0, is.beg);
				is.read(const_cast<char*>(buff.c_str()), size);
				if( str.getCountChar())
					str.append(buff.c_str());
				else
					str.set(buff.c_str(), size);
				return is;
			}
			catch(...){ }
			try
			{
				std::stringstream &ss = static_cast<std::stringstream &>(is);
				String buff(ss.str().c_str());
				if( str.getCountChar())
					str.append(buff.c_str());
				else
					str.set(buff.c_str(), buff.size() - 1);
			}
			catch(...){ }
		}
		return is;
	}
	
}