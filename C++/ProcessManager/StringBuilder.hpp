#pragma once
#include <string>
#include <sstream>

#ifdef UNICODE
typedef std::wstring ustring;
#define TO_USTRING(data) std::to_wstring(data)
typedef std::wofstream uofstream;
typedef std::wstringstream ustringstream;
#else
typedef std::string ustring;
#define TO_USTRING(data) std::to_string(data)
typedef std::ofstream uofstream;
typedef std::stringstream ustringstream;
#endif

class StringBuilder 
{
public:
	template<class T>
	StringBuilder& operator<< (const T &arg) 
	{
		mStream << arg;
		return *this;
	}
	operator ustring() const 
	{
		return mStream.str();
	}

protected:
	ustringstream mStream;

};