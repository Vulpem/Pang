#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include"String.h"

#define TMP_STRING_SIZE 4096


String::String()
{
	memorySize = 1;
	str = new char[memorySize];
	strcpy_s(str, 1, "");
}

String::String(const char* format, ...)
{
	memorySize = 0;
	if (format != NULL)
	{
		char tmp[TMP_STRING_SIZE];
		va_list ap;


		va_start(ap, format);
		int memory = vsprintf_s(tmp, TMP_STRING_SIZE, format, ap);
		va_end(ap);
		if (memory > 0)
		{
			memorySize = memory + 1;
			str = new char[memorySize];
			strcpy_s(str, memorySize, tmp);
		}
	}
	if (memorySize == 0)
	{
		memorySize = 1;
		str = new char[memorySize];
		Clear();
	}
}

String::String(const String& string)
{
	memorySize = string.memorySize;
	str = new char[memorySize];
	strcpy_s(str, memorySize, string.str);
}



bool String::operator == (const char* string) const
{
	if (string != NULL)
		return(strcmp(str, string)) == 0;
	else return 0;
}

bool String::operator == (const String& string) const
{
	return(strcmp(str, string.str)) == 0;
}

bool String::operator != (const char* string) const
{
	if (string != NULL)
		return(strcmp(str, string)) != 0;
	else return 0;
}

bool String::operator != (const String& string) const
{
	return(strcmp(str, string.str)) != 0;
}

String String::operator = (const char* string)
{
	if (string != NULL)
	{
		Alloc(strlen(string) + 1);
		strcpy_s(str, memorySize, string);
		return *this;
	}
	else
	{
		delete[] str;
		memorySize = 1;
		str = new char[1];
		strcpy_s(str, 1, "");
		return *this;
	}

}

String String::operator = (const String& string)
{
	if (string.str != NULL)
	{
		Alloc(string.memorySize);
		strcpy_s(str, memorySize, string.str);
		return *this;
	}
	else
	{
		delete[]str;
		memorySize = string.memorySize;
		str = new char[1];
		strcpy_s(str, 1, "");
		return *this;
	}
}

String String::operator += (const char* string)
{
	String tmp(*this);
	delete[] str;
	memorySize = (tmp.memorySize + strlen(string) + 1);
	str = new char[memorySize];

	strcpy_s(str, tmp.memorySize, tmp.str);

	strcat_s(str, memorySize, string);

	return *this;
}

String String::operator += (const String& string)
{
	String tmp(*this);
	delete[] str;
	memorySize = (tmp.memorySize + string.memorySize + 1);
	str = new char[memorySize];

	strcpy_s(str, tmp.memorySize, tmp.str);

	strcat_s(str, memorySize, string.str);

	return *this;
}



int String::GetLenght() const
{
	return strlen(str);
}

int String::GetCapacity() const
{
	return memorySize;
}

char* String::GetString() const
{
	return str;
}

void String::Clear()
{
	strcpy_s(str, 1, "");
}

void String::Alloc(const int memory)
{
	if (str) delete[] str;
	memorySize = memory;
	str = new char[memorySize];
}

void String::Trim()
{
	/*
	for (int i = 0; i < GetLenght(); i++)
	{
	if (str[i] == ' ')
	str.erase(i, 1);
	}
	*/
	unsigned int startingSpaces = 0;
	unsigned int endingSpaces = 0;
	for (int i = 0; i < GetLenght() && str[i] == ' '; i++)
	{
		startingSpaces++;
	}
	for (int i = GetLenght() - 1; i >= 0 && str[i] == ' '; i--)
	{
		endingSpaces++;
	}
	for (int i = 0; i < GetLenght() - startingSpaces - endingSpaces; i++)
	{
		str[i] = str[startingSpaces + i];
	}
	str[GetLenght() - startingSpaces - endingSpaces] = '\0';
}

String::~String()
{
	delete[] str;
}