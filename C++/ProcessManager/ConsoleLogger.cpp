#include "ConsoleLogger.h"

#include <iostream>
#include <memory>

#ifdef UNICODE 
#define UCOUT std::wcout
#else
#define UCOUT std::cout
#endif

ConsoleLogger::ConsoleLogger()
{
}


ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::logData(const ustring &data)
{
	UCOUT << data;
}
