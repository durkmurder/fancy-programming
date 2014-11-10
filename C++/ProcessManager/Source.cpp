#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <memory>

#include "Process.h"
#include "ConsoleLogger.h"
#include "FileLogger.h"

enum LoggerType { Console, File };

void _tmain(int argc, TCHAR *argv[])
{
	std::mutex gMutex;
	std::vector<std::exception_ptr> gExceptions;
	LoggerType loggerType = Console;
	std::unique_ptr<AbstractLogger> logger = nullptr;
	switch (loggerType)
	{
	case Console: logger.reset(new ConsoleLogger); break;
	case File: logger.reset(new FileLogger(TEXT("log.txt"))); break;
	default: break;
	}
	ustring cmdLine = TEXT("notepad.exe");
	Process newProcess(cmdLine, logger.get());


	newProcess.setOnStartCallback([](){ printf("I have started\n"); });
	newProcess.setOnStopCallback([](){ printf("I have manualy stopped\n"); });
	newProcess.setLogger(logger.get());
	int pid;

	auto runFunc = [&]()
	{
		try
		{
			newProcess.run();
			pid = newProcess.processId();
		}
		catch (...)
		{
			std::lock_guard<std::mutex> lock(gMutex);
			gExceptions.push_back(std::current_exception());
		}
	};
	Process monitorProcess(cmdLine, logger.get());
	auto monitorFunc = [&]()
	{
		try
		{
			monitorProcess.monitor(pid);
		}
		catch (...)
		{
			std::lock_guard<std::mutex> lock(gMutex);
			gExceptions.push_back(std::current_exception());
		}
	};
	std::thread newThread(runFunc);
	std::string str;
	std::cin >> str;

	std::thread monitorThread(monitorFunc);


	newThread.join();
	monitorThread.join();

	for (auto &exception : gExceptions)
	{
		try
		{
			exception._RethrowException();
		}
		catch (std::runtime_error &exc)
		{
			std::cout << exc.what();
		}
	}

}