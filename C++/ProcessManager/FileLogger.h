#pragma once
#include "AbstractLogger.h"

#include <string>
#include <fstream>
#include <iostream>

template < class T >
class FileLogger : public AbstractLogger<T>
{
};

template < >
class FileLogger<UniversalString<std::string>> : public AbstractLogger<UniversalString<std::string>>
{
	virtual void logData(const std::string &data)
	{
		std::cout << data;
	}
};

template < >
class FileLogger<UniversalString<std::wstring>> : public AbstractLogger<UniversalString<std::wstring>>
{
	virtual void logData(const std::wstring &data)
	{
		std::wcout << data;
	}
};
