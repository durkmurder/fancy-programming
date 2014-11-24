#include <windows.h>
#include <tchar.h>
#include <functional>
#include <mutex>

#include "StringBuilder.hpp"
#include "AbstractLogger.h"
#include "CriticalSectionGuard.h"

#pragma once
class Process
{
public:
	enum EventType { Started, Stoped, Crashed };

	Process(const ustring &pCmdLine);
	Process(const ustring &pCmdLine, AbstractLogger *logger);

	~Process();

	void run();
	void terminate();
	void monitor(const int pId);
	void restart();
	bool isRunning() const;

	int processId() const;

	void setEventCallback(std::function<void()> callback, const EventType evenType);
	void setLogger(AbstractLogger *logger);

private:
	void initInfo();
	void clearInfo();

	void onEvent(const EventType type) const;
	void logDataSafe(const ustring &str, bool isLocked);

	static void NTAPI onTerminated(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

	static ustring NTAPI processCommandLine(void *hProcess);

private:
	ustring mCmdLine;
	bool isMonitoring;
	bool isActive;

	STARTUPINFO mStartupInfo;
	PROCESS_INFORMATION mProcessInfo;

	CriticalSection mLock;

	void *hRegisterWait;
	void *hRegisterWaitMonitor;

	std::function<void()> mCallbacks[3];

	AbstractLogger *mLogger;


};

