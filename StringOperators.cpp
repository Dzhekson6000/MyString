#include "StringOperators.h"


using namespace doxyCraft;

String &doxyCraft::operator+=(String &Sl, const bool val)
{
	if( val )
	{
		Sl.append("1");
	}
	else
	{
		Sl.append("0");
	}
	
	return Sl;
}

String doxyCraft::operator+(const String &Sl, const bool val)
{
	auto temp = Sl;
	if( val )
	{
		temp.append("1");
	}
	else
	{
		temp.append("0");
	}
	
	return temp;
}

String &doxyCraft::operator+=(String &Sl, const double val)
{
	
	if( val > 9999999999999999 )
	{
		errno = ERANGE;
		perror(strerror(EOVERFLOW));
		return Sl;
	}
	
	char array[27] = {0};
	sprintf(array, "%lf", val);
	
	for( int i = 26; i >= 3; i-- )
	{
		if( array[i] == '0' )
		{
			array[i] = ' ';
		}
		else if( array[i] == '.' )
		{
			array[i] = ' ';
			break;
		}
		else if( array[i] > '0' )
		{
			break;
		}
	}
	Sl.append(array);
	
	for( int i = Sl.length() - 1; i >= 0; i-- )
	{
		if( Sl[i] >= '0' && Sl[i] <= '9' )
		{
			Sl.erase(i + 1, Sl.length());
			break;
		}
	}
	return Sl;
}

String &doxyCraft::operator+=(String &Sl, const float val)
{
	if( val > 9999999 )
	{
		errno = ERANGE;
		perror(strerror(EOVERFLOW));
		return Sl;
	}
	
	char* array = new char[17];
	sprintf(array, "%lf", val);
	
	for( int i = 16; i >= 3; i-- )
	{
		if( array[i] == '0' )
		{
			array[i] = ' ';
		}
		else if( array[i] == '.' )
		{
			array[i] = ' ';
			break;
		}
		else if( array[i] > '0' )
		{
			break;
		}
	}
	
	Sl.append(array);
	
	for( int i = Sl.length() - 1; i >= 0; i-- )
	{
		if( Sl[i] >= '0' && Sl[i] <= '9' )
		{
			Sl.erase(i + 1, Sl.length());
			break;
		}
	}
	
	delete[] array;
	return Sl;
}

String &doxyCraft::operator+=(String &Sl, const unsigned int val)
{
	int b = val;
	
	short int count = (b == 0 ? 1 : 0);
	while( b != 0 )
	{
		count++;
		b /= 10;
	}
	
	if( count == 1 )
	{
		char* c = new char[2];
		c[1] = '\0';
		*c = val + '0';
		Sl.append(c);
		delete[] c;
		return Sl;
	}
	
	char* buffer = new char[count];
	buffer[count] = '\0';
	b = val;
	while( true )
	{
		buffer[--count] = (b%10) + '0';
		b /= 10;
		if( b < 10 )
		{
			buffer[--count] = b + '0';
			break;
		}
	}
	Sl.append(buffer);
	delete[] buffer;
	return Sl;
}

String &doxyCraft::operator+=(String &Sl, const unsigned long int val)
{
	unsigned long int b = val;
	
	short int count = (b == 0 ? 1 : 0);
	while( b != 0 )
	{
		count++;
		b /= 10;
	}
	
	if( count == 1 )
	{
		char* c = new char[2];
		c[1] = '\0';
		*c = val + '0';
		Sl.append(c);
		delete[] c;
		return Sl;
	}
	
	char* buffer = new char[count];
	buffer[count] = '\0';
	b = val;
	while( true )
	{
		buffer[--count] = (b%10) + '0';
		b /= 10;
		if( b < 10 )
		{
			buffer[--count] = b + '0';
			break;
		}
	}
	Sl.append(buffer);
	delete[] buffer;
	return Sl;
}



String &doxyCraft::operator+=(String &Sl, const int val)
{
	
	int b = val;
	if( val < 0 )
	{
		Sl.append("-");
	}
	
	b = abs(val);
	
	short int count = (b == 0 ? 1 : 0);
	while( b != 0 )
	{
		count++;
		b /= 10;
	}
	b               = abs(val);
	
	if( count == 1 )
	{
		char* c = new char[2];
		c[1] = '\0';
		*c = (char)b + '0';
		Sl.append(c);
		delete[] c;
		return Sl;
	}
	
	char* buffer = new char[count];
	buffer[count] = '\0';
	while( true )
	{
		buffer[--count] = (b%10) + '0';
		b /= 10;
		if( b < 10 )
		{
			buffer[--count] = b + '0';
			break;
		}
	}
	Sl.append(buffer);
	delete[] buffer;
	return Sl;
}


String doxyCraft::operator+(const String &str, const double d)
{
	if( d > 9999999999999999 )
	{
		errno = ERANGE;
		perror(strerror(EOVERFLOW));
		return str;
	}
	
	String temp      = str;
	char   array[27] = {0};
	sprintf(array, "%f", d);
	
	for( int i = 26; i >= 3; i-- )
	{
		if( array[i] == '0' )
		{
			array[i] = ' ';
		}
		else if( array[i] == '.' )
		{
			array[i] = ' ';
			break;
		}
		else if( array[i] > '0' )
		{
			break;
		}
	}
	temp.append(array);
	
	for( int i = temp.length() - 1; i >= 0; i-- )
	{
		if( temp[i] >= '0' && temp[i] <= '9' )
		{
			temp.erase(i + 1, temp.length());
			break;
		}
	}
	
	return temp;
}

String doxyCraft::operator+(const String &str, const float f)
{
	if( f > 9999999 )
	{
		errno = ERANGE;
		perror(strerror(EOVERFLOW));
		return str;
	}
	
	String temp      = str;
	char   array[17] = {0};
	sprintf(array, "%f", f);
	for( int i = 16; i >= 3; i-- )
	{
		if( array[i] == '0' )
		{
			array[i] = ' ';
		}
		else if( array[i] == '.' )
		{
			array[i] = ' ';
			break;
		}
		else if( array[i] > '0' )
		{
			break;
		}
	}
	temp.append(array);
	
	for( int i = temp.length() - 1; i >= 0; i-- )
	{
		if( temp[i] >= '0' && temp[i] <= '9' )
		{
			temp.erase(i + 1, temp.length());
			break;
		}
	}
	
	return temp;
}

String doxyCraft::operator+(const String &str, const unsigned int a)
{
	String temp = str;
	int    b    = a;
	
	short count = (b == 0 ? 1 : 0);
	while( b != 0 )
	{
		count++;
		b /= 10;
	}
	
	if( count == 1 )
	{
		char* c = new char[2];
		c[1] = '\0';
		*c = a + '0';
		temp.append(c);
		delete[] c;
		return temp;
	}
	
	char* buffer = new char[count];
	buffer[count] = '\0';

	b = a;
	while( true )
	{
		
		buffer[--count] = (b%10) + '0';
		b /= 10;
		if( b < 10 )
		{
			buffer[--count] = b + '0';
			break;
		}
	}
	
	temp.append(buffer);
	
	delete[] buffer;
	return temp;
}

String doxyCraft::operator+(const String &str, const unsigned long int a)
{
	String temp = str;
	unsigned long int b = a;
	
	short count = (b == 0 ? 1 : 0);
	while( b != 0 )
	{
		count++;
		b /= 10;
	}
	
	if( count == 1 )
	{
		char* c = new char[2];
		c[1] = '\0';
		*c = a + '0';
		temp.append(c);
		delete[] c;
		return temp;
	}
	
	char* buffer = new char[count];
	buffer[count] = '\0';
	
	b = a;
	while( true )
	{
		
		buffer[--count] = (b%10) + '0';
		b /= 10;
		if( b < 10 )
		{
			buffer[--count] = b + '0';
			break;
		}
	}
	
	temp.append(buffer);
	
	delete[] buffer;
	return temp;
}


String doxyCraft::operator+(const String &str, const int a)
{
	
	String temp = str;
	int    b    = a;
	
	if( a < 0 )
	{
		temp.append("-");
	}
	
	b = abs(a);
	
	short int count = (b == 0 ? 1 : 0);
	while( b != 0 )
	{
		count++;
		b /= 10;
	}
	b               = abs(a);
	
	if( count == 1 )
	{
		char* c = new char[2];
		c[1] = '\0';
		*c = b + '0';
		temp.append(c);
		delete[] c;
		return temp;
	}
	
	char* buffer = new char[count];
	buffer[count] = '\0';
	while( true )
	{
		buffer[--count] = (b%10) + '0';
		b /= 10;
		if( b < 10 )
		{
			buffer[--count] = b + '0';
			break;
		}
	}
	
	temp.append(buffer);
	
	delete[] buffer;
	return temp;
}

size_t doxyCraft::_len(const char *c)
{
	return UTF8Functions::calcLengthStr(c, 0, strlen(c));
}