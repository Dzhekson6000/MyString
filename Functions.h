#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "String.h"
#include <iconv.h>

namespace doxyCraft
{
	size_t calcSizeChar(const char* str);
	String toUTF8String(const char* ansiChars, const unsigned int size);
}

#endif //FUNCTIONS_H
