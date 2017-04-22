#ifndef STRING_ACCUMULATOR_H
#define STRING_ACCUMULATOR_H
#include "String.h"
#include "Shared/Allocator/Allocator.h"
#include "Functions.h"

namespace  doxyCraft
{
	class StringAccumulator
	{
		static constexpr size_t MALLOC_CHAR_ITERATION {8};
	private:
		size_t _count;
		size_t _size;
		char  *_symbols;
		
		void alloc();
	public:
		StringAccumulator(size_t n=0);
		~StringAccumulator();
		
		StringAccumulator &operator=(const char *str);
		StringAccumulator &operator+=(const char *str);
		
		StringAccumulator &operator=(char c);
		StringAccumulator &operator+=(char c);
		
		void clear();
		operator String();
	};
}

#endif
