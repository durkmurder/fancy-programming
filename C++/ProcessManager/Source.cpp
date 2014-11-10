#include "Process.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <exception>
#include <string>
#include <memory>
#include <stdio.h>
#include "ConsoleLogger.h"

void _tmain(int argc, TCHAR *argv[])
{

	ustring cmdline = TEXT("notepad.exe");
	std::unique_ptr<AbstractLogger> logger(new ConsoleLogger);
	Process mngr(cmdline);
	mngr.setOnStartCallback([](){ printf("I have started\n"); });
	mngr.setOnStopCallback([](){ printf("I have manualy stopped\n"); });
	mngr.setLogger(logger.get());
	std::mutex                       gMutex;
	std::vector<std::exception_ptr>  gExceptions;
	auto threadFunc = [&]() 
	{
		try
		{
			mngr.run();
			//mngr.terminate();
			//mngr.monitor(mngr.processId());
		}
		catch (...)
		{
			std::lock_guard<std::mutex> lock(gMutex);
			gExceptions.push_back(std::current_exception());
		}
	};

	std::thread th(threadFunc);
	std::string str;
	std::cin >> str;
	th.join();

	for (auto &e : gExceptions)
	{
		try
		{
			if (e != nullptr)
				std::rethrow_exception(e);
		}
		catch (const std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	
}