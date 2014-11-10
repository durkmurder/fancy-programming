#pragma once
#include "AbstractLogger.h"

#include <fstream>

class FileLogger : public AbstractLogger
{
public:
	FileLogger(const ustring &fileName);
	~FileLogger();
	void logData(const ustring &data);

private:
	uofstream mStream;
};

