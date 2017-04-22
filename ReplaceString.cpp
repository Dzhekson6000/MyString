#include "String.h"
#include "Functions.h"

using namespace doxyCraft;

String &String::replace(size_t pos, size_t count, const String &rhs, size_t pos2, size_t count2)
{
	if( pos > length()) return *this;
	String ret;
	ret.set(_str, UTF8Functions::getRawIndex(_str, pos));
	ret.append(rhs.c_str(), pos2, count2);
	ret.append(_str, pos + count, length());
	set(ret.c_str(), calcSizeChar(ret.c_str()));
	return *this;
}

String &String::replace(size_t pos, size_t count, const String &rhs)
{
	return replace(pos, count, rhs, 0, rhs.length());
}

String &String::replace(Iterator beg, Iterator end, Iterator beg2, Iterator end2)
{
	String str_aux;
	str_aux.set(beg2, static_cast<size_t>(end2 - beg2));
	return replace(Iterator::_distance(begin(), beg), Iterator::_distance(beg, end), str_aux, 0, str_aux.length());
}

String &String::replace(Iterator beg, Iterator end, const char* rhs, size_t n)
{
	return replace(Iterator::_distance(begin(), beg), Iterator::_distance(beg, end), String(rhs), 0, n);
}

String &String::replace(Iterator beg, Iterator end, const char* rhs)
{
	String str_aux(rhs);
	return replace(Iterator::_distance(begin(), beg), Iterator::_distance(beg, end), str_aux, 0, str_aux.length());
}