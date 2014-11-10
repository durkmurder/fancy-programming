#include <windows.h>
#include <tchar.h>
#include <functional>

#include "AbstractLogger.h"

#pragma once
class Process
{
public:
	Process(const ustring &pCmdLine);
	~Process();
	void run();
	void terminate();
	int processId() const;
	bool isActive() const;
	void setOnStartCallback(std::function<void()> callback);
	void setOnStopCallback(std::function<void()> callback);
	void setOnCrashCallback(std::function<void()> callback);
	void setLogger(AbstractLogger *logger);
	void monitor(int pId);

private:
	void initInfo();
	void clearInfo();
	static void NTAPI onTerminated(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

private:
	ustring mCmdLine;
	STARTUPINFO mStartupInfo;
	PROCESS_INFORMATION mProcessInfo;
	std::function<void()> onProcStarted;
	std::function<void()> onProcStoped;
	std::function<void()> onProcCrashed;
	AbstractLogger *mLogger;
};
