#pragma once
#include "StringBuilder.hpp"

class AbstractLogger
{
public:
	AbstractLogger();
	virtual ~AbstractLogger();

	virtual void logData(const ustring &data) = 0;
};

