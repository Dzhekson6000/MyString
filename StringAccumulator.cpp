#include "StringAccumulator.h"
using namespace doxyCraft;

StringAccumulator::StringAccumulator(size_t n):_count(n),_size(_count)
{
	if (n)
		_symbols = ALLOC_ARRAY<char, AL_DYNAMIC>((unsigned int)n);
	_symbols = nullptr;
}

StringAccumulator::~StringAccumulator()
{
	if (_symbols)
		FREE_ARRAY(_symbols);
}

void StringAccumulator::alloc()
{
	++_size; //for '\0'
	if (_symbols)
	{
		_symbols = RELOC_ARRAY<char, AL_DYNAMIC>(_symbols, (unsigned int)_size);
	}
	else _symbols = ALLOC_ARRAY<char, AL_DYNAMIC>((unsigned int)_size);
}

StringAccumulator &StringAccumulator::operator=(const char* str)
{
	if (strlen(str) > _size)
	{
		_count = strlen(str);
		_size  = _count + (_count/2);
		this->alloc();
	}
	strcpy(_symbols, str);
	return *this;
}

StringAccumulator &StringAccumulator::operator+=(const char* str)
{
	size_t old_count = _count;
	if (_count + strlen(str) > _size)
	{
		_count += strlen(str);
		_size += _size +_count;
		this->alloc();
	}
	strcpy(_symbols+old_count, str);
	return *this;
}

StringAccumulator &StringAccumulator::operator=(char c)
{
	if (!_size)
	{
		_size = MALLOC_CHAR_ITERATION;
		_count = 1;
		this->alloc();
	}
	*_symbols = c;
	return *this;
}

StringAccumulator &StringAccumulator::operator+=(char c)
{
	if (_count + 1 > _size)
	{
		_size += MALLOC_CHAR_ITERATION;
		this->alloc();
	}
	_symbols[_count++] = c;
	_symbols[_count] = '\0';
	return *this;
}

void StringAccumulator::clear()
{
	_count = 0;
	*_symbols = '\0';
}

StringAccumulator::operator String()
{
	String ret;
	size_t i, symLen, strLen;
	for (i = strLen = strlen(_symbols); i >= 1; --i)
	{
		if ((symLen = UTF8Functions::getCodepointLen(_symbols+i, size_t())) > 1)
			break;
	}
	if (i)
	{
		if (i+symLen == strLen)
		{
			ret.append(_symbols);
		}
		else
		{
			char * tmp = ALLOC_ARRAY<char, AL_DYNAMIC>((unsigned int)i);
			memcpy(tmp,_symbols,i);
			tmp[i] = '\0';
			ret.append(tmp);
			FREE_ARRAY(tmp);
		}
	}
	return ret;
}

