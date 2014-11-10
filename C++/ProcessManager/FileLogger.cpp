#include "FileLogger.h"


FileLogger::FileLogger(const ustring &fileName)
{
	mStream.open(fileName);
}


FileLogger::~FileLogger()
{
	mStream.close();
}

void FileLogger::logData(const ustring &data)
{
	if (mStream.is_open())
		mStream << data;
}