#ifndef UTF8FUNCTIONS_H
#define UTF8FUNCTIONS_H

#include "stddef.h"
#include "Iterator.h"

namespace doxyCraft
{
	class UTF8Functions
	{
	public:
		static bool isValid(const char* str);
		static bool isValid(const char* str, const size_t positionBegin);
		static bool isValid(const char* str, const size_t positionBegin, const size_t positionEnd);
	
		static size_t calcLengthStr(const char* str, const size_t positionBegin, const size_t positionEnd);
		static size_t getCodepointLen(const char* str, size_t n);
		static size_t getRawIndex(const char* str, size_t pos = 0);
	};
}

#endif //UTF8FUNCTIONS_H
