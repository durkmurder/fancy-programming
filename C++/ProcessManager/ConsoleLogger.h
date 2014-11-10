#pragma once

#include "AbstractLogger.h"

#include <mutex>



class ConsoleLogger : public AbstractLogger
{
public:
	ConsoleLogger();
	~ConsoleLogger();
	virtual void logData(const ustring &data) const;
};

