#pragma once

#include "AbstractLogger.h"

#include <string>
#include <iostream>

template < class T >
class ConsoleLogger : public AbstractLogger<T>
{
};

template < >
class ConsoleLogger<UniversalString<std::string>> : public AbstractLogger<UniversalString<std::string>>
{
	virtual void logData(const std::string &data)
	{
		std::cout << data;
	}
};

template < >
class ConsoleLogger<UniversalString<std::wstring>> : public AbstractLogger<UniversalString<std::wstring>>
{
	virtual void logData(const std::wstring &data)
	{
		std::wcout << data;
	}
};

