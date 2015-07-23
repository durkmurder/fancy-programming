#pragma once
#include "StringBuilder.hpp"

template <class T>
class AbstractLogger
{
public:
	typedef typename T::StringType String;
	AbstractLogger()
	{}
	virtual ~AbstractLogger()
	{}

	virtual void logData(const String &data) = 0;
};

