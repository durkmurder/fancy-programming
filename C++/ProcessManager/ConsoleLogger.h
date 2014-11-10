#pragma once

#include "AbstractLogger.h"

class ConsoleLogger : public AbstractLogger
{
public:
	ConsoleLogger();
	~ConsoleLogger();
	virtual void logData(const ustring &data);
};

