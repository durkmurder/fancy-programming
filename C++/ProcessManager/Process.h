#include <windows.h>
#include <tchar.h>
#include <functional>

#include "StringBuilder.hpp"
#include "AbstractLogger.h"

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
	void monitor(int pId);
	void restart();

	int processId() const;
	bool isActive() const;

	void setEventCallback(std::function<void()> callback, const EventType evenType);
	void setLogger(AbstractLogger *logger);

private:
	void initInfo();
	void clearInfo();
	void onEvent(const EventType type) const;

	static void NTAPI onTerminated(PVOID lpParameter, BOOLEAN TimerOrWaitFired);


private:
	ustring mCmdLine;
	STARTUPINFO mStartupInfo;
	PROCESS_INFORMATION mProcessInfo;
	std::function<void()> mCallbaks[3];

	AbstractLogger *mLogger;


};

