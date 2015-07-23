#include <windows.h>
#include <tchar.h>
#include <functional>
#include <mutex>
#include <string>
#include <exception>
#include <iostream>

#include <winternl.h>
#pragma comment(lib, "Ntdll")

#include "StringBuilder.hpp"
#include "AbstractLogger.h"
#include "CriticalSectionGuard.h"

#pragma once
template <class T>
class Process
{
public:
	enum EventType { Started, Stoped, Crashed };

	typedef typename T::StringType StringT;
	typedef typename AbstractLogger<T> LoggerT;
	

	Process(const StringT &pCmdLine)
	{
		mLogger = nullptr;
		hRegisterWait = nullptr;
		hRegisterWaitMonitor = nullptr;
		mCmdLine = pCmdLine;
		initInfo();
		isActive = false;
		isMonitoring = false;
	}
	Process(const StringT &pCmdLine, LoggerT *logger)
	{
		InterlockedExchangePointer(reinterpret_cast<volatile PVOID*>(&mLogger), logger);
		hRegisterWait = nullptr;
		hRegisterWaitMonitor = nullptr;
		mCmdLine = pCmdLine;
		initInfo();
		isActive = false;
		isMonitoring = false;
	}

	~Process()
	{
		clearInfo();
	}

	void run()
	{
		CriticalSectionGuard autoLock(&mLock);
		if (!isActive)
		{
			clearInfo();
			initInfo();

			if (mCmdLine.empty())
				throw std::runtime_error((std::string("Cmdline empty") + std::to_string(GetLastError())) + std::string("\n"));

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
				mLogger->logData(StringBuilder<StringT>() << TEXT("[LOG] ") << TEXT("Process has started, PID ")
				<< T::toString(mProcessInfo.dwProcessId) << TEXT("\n"));

			onEvent(EventType::Started);
			isActive = true;

			if (!RegisterWaitForSingleObject(&hRegisterWait, mProcessInfo.hProcess, onTerminated, static_cast<void*>(this), INFINITE, WT_EXECUTEONLYONCE))
			{
				throw std::runtime_error((std::string("RegisterWaitForSingleObject failed, errcode: ") + std::to_string(GetLastError())) + std::string("\n"));
			}

		}
		else
		{
			if (mLogger)
				mLogger->logData(StringBuilder<StringT>() << TEXT("[LOG] Process is already running, PID ")
				<< T::toString(mProcessInfo.dwProcessId) << TEXT("\n"));
		}
	}

	void terminate()
	{
		CriticalSectionGuard autoLock(&mLock);
		if (mProcessInfo.hProcess)
		if (!TerminateProcess(mProcessInfo.hProcess, 0))
			throw std::runtime_error(std::string("TerminateProcess failed, errcode: ") + std::to_string(GetLastError()) + "\n");
	}
	void monitor(const int pId)
	{
		CriticalSectionGuard autoLock(&mLock);
		if (!hRegisterWaitMonitor)
		{
			HANDLE hwndProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
			ustring infoString;

			if (!hwndProc)
			{
				infoString = StringBuilder() << TEXT("[LOG] ") << TEXT("Couldn`t open a process to monitor, PID: ")
					<< TO_USTRING(pId) << TEXT("\n");

				if (mLogger)
					mLogger->logData(infoString);

				return;
			}

			infoString = StringBuilder() << TEXT("[LOG] ") << TEXT("Opened a process to monitor, PID: ")
				<< TO_USTRING(pId) << TEXT("\n");

			mLogger->logData(infoString);

			clearInfo(); // Close handles which were used before. Because we will use handle that we got from OpenProcess function.
			mProcessInfo.hProcess = hwndProc;
			mProcessInfo.dwProcessId = pId;

			mCmdLine = processCommandLine(mProcessInfo.hProcess);

			if (!RegisterWaitForSingleObject(&hRegisterWaitMonitor, hwndProc, onTerminated, static_cast<void*>(this), INFINITE, WT_EXECUTEONLYONCE))
			{
				throw std::runtime_error((std::string("RegisterWaitForSingleObject failed, errcode: ") + std::to_string(GetLastError())) + std::string("\n"));
			}
			isMonitoring = true;
		}
		else
		{
			if (mLogger)
				mLogger->logData(StringBuilder() << TEXT("[LOG] Process is already monitoring, PID ")
				<< TO_USTRING(mProcessInfo.dwProcessId) << TEXT("\n"));
		}
	}
	void restart()
	{
		if (isActive)
		{
			terminate();
		}
		run();
	}
	bool isRunning() const
	{
		return isActive | isMonitoring;
	}

	int processId() const
	{
		return mProcessInfo.dwProcessId;
	}

	void setEventCallback(std::function<void()> callback, const EventType evenType)
	{
		mCallbacks[evenType] = callback;
	}
	void setLogger(LoggerT *logger)
	{
		InterlockedExchangePointer(reinterpret_cast<volatile PVOID*>(&mLogger), logger);
	}

private:
	void initInfo()
	{
		ZeroMemory(&mStartupInfo, sizeof(mStartupInfo));
		mStartupInfo.cb = sizeof(mStartupInfo);
		ZeroMemory(&mProcessInfo, sizeof(mProcessInfo));
	}
	void clearInfo()
	{
		if (mProcessInfo.hProcess)
			CloseHandle(mProcessInfo.hProcess);
		if (mProcessInfo.hThread)
			CloseHandle(mProcessInfo.hThread);
	}

	void onEvent(const EventType type) const
	{
		if (!mCallbacks[type]._Empty())
			mCallbacks[type]();
	}
	void logDataSafe(const StringT &str, bool isLocked);

	static void NTAPI onTerminated(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
	{
		Process *sender = static_cast<Process*>(lpParameter); // sender of event
		if (sender)
		{
			DWORD exitCode = 0;

			if (GetExitCodeProcess(sender->mProcessInfo.hProcess, &exitCode))
			{
				if (exitCode == 0)
				{
					if (sender->mLogger)
						sender->mLogger->logData(StringBuilder<StringT>() << TEXT("[LOG] ") << TEXT("Process has been stoped, PID ")
						<< T::toString(sender->mProcessInfo.dwProcessId) << TEXT("\n"));

					sender->onEvent(EventType::Stoped);
				}
				else
				{
					if (sender->mLogger)
						sender->mLogger->logData(StringBuilder<StringT>() << TEXT("[LOG] ") << TEXT("Process has crashed, PID ")
						<< T::toString(sender->mProcessInfo.dwProcessId) << TEXT("\n"));

					sender->onEvent(EventType::Crashed);
				}
			}

			UnregisterWait(sender->hRegisterWait);
			sender->hRegisterWait = nullptr;
			sender->isActive = false;
			if (sender->isMonitoring)
			{
				sender->run();
			}


		}
	}

	static StringT NTAPI processCommandLine(void *hProcess)
	{
		PROCESS_BASIC_INFORMATION  pInfo;
		PVOID rtlUserProcParamsAddress;
		UNICODE_STRING commandLine;
		WCHAR *commandLineContents;

		LONG status = NtQueryInformationProcess(handle,
			ProcessBasicInformation,
			&pInfo,
			sizeof(pInfo),
			NULL);

		bool check;

		void *pedAddress = pInfo.PebBaseAddress;
		/* get the address of ProcessParameters */
		if (!ReadProcessMemory(handle, (PCHAR)pedAddress + 0x10,
			&rtlUserProcParamsAddress, sizeof(PVOID), NULL))
		{
			check = false;
		}

		/* read the CommandLine UNICODE_STRING structure */
		if (!ReadProcessMemory(handle, (PCHAR)rtlUserProcParamsAddress + 0x40,
			&commandLine, sizeof(commandLine), NULL))
		{
			check = false;
		}

		/* allocate memory to hold the command line */
		commandLineContents = (WCHAR *)malloc(commandLine.Length);

		/* read the command line */
		if (!ReadProcessMemory(handle, commandLine.Buffer,
			commandLineContents, commandLine.Length, NULL))
		{
			check = false;
		}

		return StringT(check ? commandLineContents : TEXT(""));
	}

private:
	StringT mCmdLine;
	bool isMonitoring;
	bool isActive;

	STARTUPINFO mStartupInfo;
	PROCESS_INFORMATION mProcessInfo;

	CriticalSection mLock;

	void *hRegisterWait;
	void *hRegisterWaitMonitor;

	std::function<void()> mCallbacks[3];

	LoggerT *mLogger;
};

