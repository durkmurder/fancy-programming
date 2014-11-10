#include "ConsoleLogger.h"

#include <iostream>
#include <memory>

#ifdef UNICODE 
#define UCOUT std::wcout
#else
#define UCOUT std::cout
#endif

std::mutex gMutex;

ConsoleLogger::ConsoleLogger()
{
}


ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::logData(const ustring &data) const
{
	std::lock_guard<std::mutex> lock(gMutex);
	UCOUT << data;
}
