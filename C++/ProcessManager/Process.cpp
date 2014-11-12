#include <string>

#include "Process.h"
#include <exception>



Process::Process(const ustring &pCmdLine) : mCmdLine(pCmdLine)
{
	mLogger = NULL;
	initInfo();
}

Process::Process(const ustring &pCmdLine, AbstractLogger *logger) : Process(pCmdLine)
{
	mLogger = logger;
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
		_tcsdup(TEXT("C:\\Windows\\Temp")),
		&mStartupInfo,            // Pointer to STARTUPINFO structure
		&mProcessInfo)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		throw std::runtime_error(std::string("CreateProcess failed, errcode: ") + std::to_string(GetLastError()) + "\n");
	}
	if (mLogger)
		mLogger->logData(StringBuilder() << TEXT("[LOG] ") << TEXT("Process has started, PID ") 
										 << TO_USTRING(mProcessInfo.dwProcessId) << TEXT("\n"));

	onEvent(EventType::Started);

	monitor(mProcessInfo.dwProcessId);
}

void Process::terminate()
{
	if (!TerminateProcess(mProcessInfo.hProcess, 0))
	{
		throw std::runtime_error(std::string("TerminateProcess failed, errcode: ") + std::to_string(GetLastError()) + "\n");
	}
}

void Process::restart()
{
	if (isActive())
	{
		terminate();
	}
	//Sleep(10);
	run();
}

void Process::initInfo()
{
	ZeroMemory(&mStartupInfo, sizeof(mStartupInfo));
	mStartupInfo.cb = sizeof(mStartupInfo);
	ZeroMemory(&mProcessInfo, sizeof(mProcessInfo));
}

void Process::clearInfo()
{
	if (mProcessInfo.hProcess)
		CloseHandle(mProcessInfo.hProcess); 
	if (mProcessInfo.hThread)
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
		throw std::runtime_error((std::string("GetExitCodeProcess failed, errcode: ") + std::to_string(GetLastError())) + std::string("\n"));
	}
	return exitCode == STILL_ACTIVE ? true : false;
}


void Process::setEventCallback(std::function<void()> callback, const EventType evenType)
{
	mCallbaks[evenType] = callback;
}


void NTAPI Process::onTerminated(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	const Process *sender = static_cast<Process*>(lpParameter); // sender of event
	if (sender)
	{
		DWORD exitCode = 0;
		if (GetExitCodeProcess(sender->mProcessInfo.hProcess, &exitCode))
		{
			if (exitCode == 0)
			{
				if (sender->mLogger)
					sender->mLogger->logData(StringBuilder() << TEXT("[LOG] ") << TEXT("Process has been stoped, PID ")
					<< TO_USTRING(sender->mProcessInfo.dwProcessId) << TEXT("\n"));

				sender->onEvent(EventType::Stoped);
			}
			else
			{
				if (sender->mLogger)
					sender->mLogger->logData(StringBuilder() << TEXT("[LOG] ") << TEXT("Process has crashed, PID ")
					<< TO_USTRING(sender->mProcessInfo.dwProcessId) << TEXT("\n"));


				sender->onEvent(EventType::Crashed);
			}
		}
		
	}
	
}

void Process::setLogger(AbstractLogger *logger)
{
	mLogger = logger;
}

void Process::monitor(int pId)
{
	HANDLE hwndProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
	ustring infoString;

	if (hwndProc == NULL)
	{
		infoString = StringBuilder() << TEXT("[LOG] ") << TEXT("Couldn`t open a process to monitor, PID: ") 
									 << TO_USTRING(pId) << TEXT("\n");

		mLogger->logData(infoString);
		return;
	}

	infoString = StringBuilder() << TEXT("[LOG] ") << TEXT("Opened a process to monitor, PID: ") 
								 << TO_USTRING(pId) << TEXT("\n");

	mLogger->logData(infoString);
	
	void *hRegisterWait = NULL;
	CloseHandle(mProcessInfo.hProcess); // Close previous handle
	mProcessInfo.hProcess = hwndProc;
	mProcessInfo.dwProcessId = pId;

	if (!RegisterWaitForSingleObject(&hRegisterWait, hwndProc, onTerminated, static_cast<void*>(this), INFINITE, WT_EXECUTEONLYONCE))
	{
		throw std::runtime_error((std::string("RegisterWaitForSingleObject failed, errcode: ") + std::to_string(GetLastError())) + std::string("\n"));
	}
}

void Process::onEvent(const EventType type) const
{
	if (!mCallbaks[type]._Empty())
		mCallbaks[type]();
}