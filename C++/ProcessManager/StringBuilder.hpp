#pragma once
#include <string>
#include <sstream>

template < class T >
class StringBuilder 
{
};

template < >
class StringBuilder <std::string>
{
public:
	template<class U>
	StringBuilder& operator<< (const U &arg)
	{
		mStream << arg;
		return *this;
	}
	operator std::string() const
	{
		return mStream.str();
	}

protected:
	std::stringstream mStream;

};

template < >
class StringBuilder <std::wstring>
{
public:
	template<class U>
	StringBuilder& operator<< (const U &arg)
	{
		mStream << arg;
		return *this;
	}
	operator std::wstring() const
	{
		return mStream.str();
	}

protected:
	std::wstringstream mStream;

};