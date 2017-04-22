#ifndef STRINGOPERATOR_H
#define STRINGOPERATOR_H

#include "String.h"

namespace doxyCraft
{
	size_t _len(const char *);
	template <class T, typename = doxyCraft::RequiredCharTypes<T,T>>
	size_t _len(const T &c)
	{
		return 	c.length();
	}
	class String;
	String &operator+=(String &Sl, const bool val);
	String &operator+=(String &Sl, const double val);
	String &operator+=(String &Sl, const float val);
	String &operator+=(String &Sl, const unsigned int val);
	String &operator+=(String &Sl, const unsigned long int val);
	String &operator+=(String &Sl, const int val);
	String operator+(const String &Sl, const bool val);
	String operator+(const String &str, const double d);
	String operator+(const String &str, const float f);
	String operator+(const String &str, const unsigned int a);
	String operator+(const String &str, const unsigned long int a);
	String operator+(const String &str, const int a);
	
	#define OPERATOR template<class T1, class T2, typename = doxyCraft::RequiredCharTypes<T1, T2>> bool operator
	#define OP_EQ       { return ( doxyCraft::_len(s1) == doxyCraft::_len(s2) && std::equal(doxyCraft::begin(s1), doxyCraft::end(s1), doxyCraft::begin(s2)) ); }
	#define OP_GR       { return operator<(s2, s1); }
	#define OP_GR_EQ    { return operator>(s1, s2) || operator==(s1, s2); }
	#define OP_LE_EQ    { return operator<(s1, s2) || operator==(s1, s2); }
	#define OP_N_EQ     { return !operator==(s1, s2); }
	#define OP_LE						\
	{							\
			auto beg1 = doxyCraft::begin(s1);	\
			auto end1 = doxyCraft::end(s1);		\
			auto beg2 = doxyCraft::begin(s2);	\
			auto end2 = doxyCraft::end(s2);		\
			auto size1 = std::distance(beg1, end1);	\
			auto size2 = std::distance(beg2, end2);	\
			auto min = std::min(size1, size2);	\
			auto compare = memcmp(beg1, beg2, min);	\
			if ( !compare ) compare = size1 - size2;\
			return compare < 0;			\
	}
	
	OPERATOR<(T1 &s1, T2 &s2) OP_LE
	
	OPERATOR<(const T1 &s1, T2 &s2) OP_LE
	
	OPERATOR<(T1 &s1, const T2 &s2) OP_LE
	
	OPERATOR<(const T1 &s1, const T2 &s2) OP_LE
	
	OPERATOR==(T1 &s1, T2 &s2) OP_EQ
	
	OPERATOR==(const T1 &s1, T2 &s2) OP_EQ
	
	OPERATOR==(T1 &s1, T2 const &s2) OP_EQ
	
	OPERATOR==(const T1 &s1, const T2 &s2) OP_EQ
	
	OPERATOR>(T1 &s1, T2 &s2) OP_GR
	
	OPERATOR>(const T1 &s1, T2 &s2) OP_GR
	
	OPERATOR>(T1 &s1, const T2 &s2) OP_GR
	
	OPERATOR>(const T1 &s1, const T2 &s2) OP_GR
	
	OPERATOR!=(T1 &s1, T2 &s2) OP_N_EQ
	
	OPERATOR!=(const T1 &s1, T2 &s2) OP_N_EQ
	
	OPERATOR!=(T1 &s1, const T2 &s2) OP_N_EQ
	
	OPERATOR!=(const T1 &s1, const T2 &s2) OP_N_EQ
	
	OPERATOR>=(T1 &s1, T2 &s2) OP_GR_EQ
	
	OPERATOR>=(const T1 &s1, T2 &s2) OP_GR_EQ
	
	OPERATOR>=(T1 &s1, const T2 &s2) OP_GR_EQ
	
	OPERATOR>=(const T1 &s1, const T2 &s2) OP_GR_EQ
	
	OPERATOR<=(T1 &s1, T2 &s2) OP_LE_EQ
	
	OPERATOR<=(const T1 &s1, T2 &s2) OP_LE_EQ
	
	OPERATOR<=(T1 &s1, const T2 &s2) OP_LE_EQ
	
	OPERATOR<=(const T1 &s1, const T2 &s2) OP_LE_EQ
}

#endif //STRINGOPERATOR_H
