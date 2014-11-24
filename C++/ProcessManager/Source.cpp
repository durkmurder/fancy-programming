#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <exception>

#include "Process.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"

enum LoggerType { Console, File };

void _tmain(int argc, TCHAR *argv[])
{
	const int N = 3;
	std::thread th[N];
	std::unique_ptr<AbstractLogger> cLogger(new ConsoleLogger);
	std::unique_ptr<AbstractLogger> fLogger(new FileLogger(TEXT("log.txt")));
	const ustring cmdLine = TEXT("notepad.exe");
	Process newProcess(cmdLine, cLogger.get());
	Process monitorProcess(TEXT(""), fLogger.get()); // writing log to file

	newProcess.setEventCallback([](){ std::cout << "I have started" << std::endl; }, Process::Started);
	newProcess.setEventCallback([](){ std::cout << "I have stoped" << std::endl; }, Process::Stoped);
	std::mutex lock;
	std::vector<std::exception_ptr>  exceptions;

	auto func = [&]()
	{ 
		try
		{
			newProcess.run();
			monitorProcess.monitor(newProcess.processId());
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