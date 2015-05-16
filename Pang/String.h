#ifndef __String_H__
#define __String_H__

#include <stdlib.h>
#include <string.h>
class String
{
	/////////////
	//Atributes//
	/////////////
public:
	char* str;
	int memorySize;


	////////////////
	//Constructors//
	////////////////

	String();

	String(const char*format, ...);

	String(const String& string);


	///////////
	//Methods//
	///////////

	bool operator == (const char* string) const;

	bool operator == (const String& string) const;

	bool operator != (const char* string) const;

	bool operator != (const String& string) const;

	String operator = (const char* string);

	String operator = (const String& string);

	String operator += (const char* string);

	String operator += (const String& string);

	String String::prefix(const String& string)
	{
		String tmp(*this);
		delete[]str;
		memorySize = (tmp.memorySize + string.memorySize - 1);
		str = new char[memorySize];

		strcpy_s(str, string.memorySize, string.str);
		strcat_s(str, memorySize, tmp.str);

		return str;
	}

	String String::prefix(const char* string)
	{
		String tmp(*this);
		delete[]str;
		memorySize = (tmp.memorySize + strlen(string));
		str = new char[memorySize];

		strcpy_s(str, strlen(string) + 1, string);
		strcat_s(str, memorySize, tmp.str);

		return str;
	}

	void Clear();

	int GetLenght() const;

	int GetCapacity() const;

	char* GetString() const;

	void Alloc(const int memory);

	void Trim();

	//////////////
	//Destructor//
	//////////////

	~String();


};

#endif //__String_H__