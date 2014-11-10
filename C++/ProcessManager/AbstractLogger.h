#pragma once

#include <string>

#ifdef UNICODE
typedef std::wstring ustring;
#define TO_USTRING(data) std::to_wstring(data)
typedef std::wostream uostream;
#else
typedef std::string ustring;
#define TO_USTRING(data) std::to_string(data)
typedef std::ostream ustream;
#endif

class AbstractLogger
{
public:
	AbstractLogger();
	virtual ~AbstractLogger();

	virtual void logData(const ustring &data) const = 0;
};

