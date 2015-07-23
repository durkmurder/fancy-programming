#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <exception>

#include "UniversalString.hpp"
#include "Process.hpp"
#include "ConsoleLogger.hpp"
#include "FileLogger.h"

enum LoggerType { Console, File };

void _tmain(int argc, TCHAR *argv[])
{

	typedef UniversalString<std::wstring> UString;
	const int N = 3;
	std::thread th[N];
	std::unique_ptr<AbstractLogger<UString>> cLogger(new ConsoleLogger<UString>);
	//std::unique_ptr<AbstractLogger<std::wstring>> fLogger(new FileLogger<std::wstring>(std::wstring(TEXT("log.txt"))));
	const std::wstring cmdLine = TEXT("notepad.exe");
	Process<UString> newProcess(cmdLine, cLogger.get());
	//Process monitorProcess(TEXT(""), fLogger.get()); // writing log to file

	newProcess.setEventCallback([](){ std::cout << "I have started" << std::endl; }, Process<UString>::Started);
	newProcess.setEventCallback([](){ std::cout << "I have stoped" << std::endl; }, Process<UString>::Stoped);
	std::mutex lock;
	std::vector<std::exception_ptr>  exceptions;

	auto func = [&]()
	{ 
		try
		{
			newProcess.run();
			//monitorProcess.monitor(newProcess.processId());
		}
		catch (...)
		{
			std::lock_guard<std::mutex> lock(lock);
			exceptions.push_back(std::current_exception());
		}
	};

	for (int i = 0; i < N; ++i)
	{
		th[i] = std::thread(func);
	}

	for (int i = 0; i < N; ++i)
		th[i].join();

	for (auto &e : exceptions) // Handle exceptions that were thrown in thread
	{
		try
		{
			if (e != nullptr)
				std::rethrow_exception(e);
		}
		catch (const std::exception &e)
		{
			std::cout << e.what();
		}
	}

	/*try
	{
		newProcess.run();
		monitorProcess.monitor(newProcess.processId());

	}
	catch (std::exception &exc)
	{
		std::cout << exc.what();
	}*/

	std::string str;
	std::cin >> str;
}