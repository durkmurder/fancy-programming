#include <string>

#include "Process.h"
#include <exception>



Process::Process(const ustring &pCmdLine) : mCmdLine(pCmdLine)
{
	mLogger = NULL;
	initInfo();
}


Process::~Process()
{
	clearInfo();
}


void Process::run()
{
	clearInfo();
	initInfo();
	
	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		_tcsdup(mCmdLine.c_str()),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		_tcsdup(TEXT("C:\\Windows\\Temp")),           // Use parent's starting directory 
		&mStartupInfo,            // Pointer to STARTUPINFO structure
		&mProcessInfo)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		//throw std::runtime_error((std::string("CreateProcess failed (%d).\n") + std::to_string(GetLastError())).c_str());
		printf("CreateProcess failed (%d).\n", GetLastError());
	}
	if (mLogger)
		mLogger->logData(TEXT("Process has been started\n"));
	if (!onProcStarted._Empty())
		onProcStarted();

	void *m_hRegisterWait = NULL;
	//WaitForSingleObject(mProcessInfo.hProcess, INFINITE);
	monitor(mProcessInfo.dwProcessId);
	//RegisterWaitForSingleObject(&m_hRegisterWait, mProcessInfo.hProcess, onTerminated, 
		//static_cast<void*>(this), INFINITE, WT_EXECUTEONLYONCE);
}

void Process::terminate()
{
	TerminateProcess(mProcessInfo.hProcess, 0);
}

void Process::initInfo()
{
	ZeroMemory(&mStartupInfo, sizeof(mStartupInfo));
	mStartupInfo.cb = sizeof(mStartupInfo);
	ZeroMemory(&mProcessInfo, sizeof(mProcessInfo));
}

void Process::clearInfo()
{
	if (mProcessInfo.hProcess != NULL)
		CloseHandle(mProcessInfo.hProcess);
	if (mProcessInfo.hThread != NULL)
		CloseHandle(mProcessInfo.hThread);
}

int Process::processId() const
{
	return GetProcessId(mProcessInfo.hProcess);
}

bool Process::isActive() const
{
	DWORD exitCode = 0;
	if (GetExitCodeProcess(mProcessInfo.hProcess, &exitCode) == 0)
	{
		throw std::runtime_error((std::string("GetExitCodeProcess failed ") + std::to_string(GetLastError())) + std::string("\n"));
	}
	return exitCode == STILL_ACTIVE ? true : false;
}


void Process::setOnStartCallback(std::function<void()> callback)
{
	onProcStarted = callback;
}

void Process::setOnStopCallback(std::function<void()> callback)
{
	onProcStoped = callback;
}

void Process::setOnCrashCallback(std::function<void()> callback)
{
	onProcCrashed = callback;
}

void NTAPI Process::onTerminated(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	const Process *sender = static_cast<Process*>(lpParameter); // sender of event
	if (sender)
	{
		DWORD exitCode = 0;
		if (GetExitCodeProcess(sender->mProcessInfo.hProcess, &exitCode) == 0)
		{
			throw std::runtime_error((std::string("GetExitCodeProcess failed ") + std::to_string(GetLastError())) + std::string("\n"));
		}
		if (exitCode == 0)
		{
			if (sender->mLogger)
				sender->mLogger->logData(TEXT("Process has been stoped\n"));

			if (!sender->onProcStoped._Empty())
				sender->onProcStoped();
		}
		else
		{
			if (sender->mLogger)
				sender->mLogger->logData(TEXT("Process has crashed\n"));

			if (!sender->onProcCrashed._Empty())
				sender->onProcCrashed();
		}
	}
	
}

void Process::setLogger(AbstractLogger *logger)
{
	mLogger = logger;
}

void Process::monitor(int pId)
{
	HANDLE hwndProc = OpenProcess(SYNCHRONIZE, true, pId);
	if (hwndProc == NULL)
	{
		mLogger->logData(TEXT("Couldn`t open a process to monitor\n"));
		return;
	}
	
	mLogger->logData(TEXT("Opened a process to monitor\n"));
	void *m_hRegisterWait = NULL;
	//WaitForSingleObject(mProcessInfo.hProcess, INFINITE);
	RegisterWaitForSingleObject(&m_hRegisterWait, hwndProc, onTerminated,
		static_cast<void*>(this), INFINITE, WT_EXECUTEONLYONCE);
}