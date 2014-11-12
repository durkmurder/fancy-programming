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
	LoggerType loggerType = Console;
	std::unique_ptr<AbstractLogger> logger = nullptr;
	switch (loggerType)
	{
	case Console: logger.reset(new ConsoleLogger); break;
	case File: logger.reset(new FileLogger(TEXT("log.txt"))); break;
	default: break;
	}
	const ustring cmdLine = TEXT("notepad.exe 'newfile'");
	Process newProcess(cmdLine, logger.get());
	Process monitorProcess(cmdLine, logger.get());


	newProcess.setEventCallback([](){ std::cout << "I have started" << std::endl; }, Process::Started);
	monitorProcess.setEventCallback([&]() { newProcess.restart(); }, Process::Crashed); // auto-restart on crash

	try
	{
		newProcess.run();
		monitorProcess.monitor(newProcess.processId());
	}
	catch (std::exception &exc)
	{
		std::cout << exc.what();
	}
	std::string str;
	std::cin >> str;

}