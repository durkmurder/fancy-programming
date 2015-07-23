#pragma once

#include <string>

template <class T>
struct UniversalString
{
	typedef T StringType;
};

template <>
struct UniversalString<std::string>
{
	typedef std::string StringType;

	template <class U>
	static StringType toString(U arg)
	{
		return std::to_string(arg);
	}

};

template <>
struct UniversalString<std::wstring>
{
	typedef std::wstring StringType;

	template <class U>
	static StringType toString(U arg)
	{
		return std::to_wstring(arg);
	}

};