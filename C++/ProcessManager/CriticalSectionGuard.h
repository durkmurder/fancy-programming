#include <windows.h>

#pragma once

class CriticalSectionGuard;

class CriticalSection
{
public:
	friend class CriticalSectionGuard;
	CriticalSection();
	~CriticalSection();
	CriticalSection &operator=(const CriticalSection &right) = delete;
	CriticalSection(const CriticalSection &other) = delete;

private:
	void enter();
	void leave();

private:
	CRITICAL_SECTION mCriticalSection;

};

// Simple guard for CriticalSection with auto release
class CriticalSectionGuard
{
public:
	CriticalSectionGuard(CriticalSection *cs);
	~CriticalSectionGuard();
	CriticalSectionGuard &operator=(const CriticalSectionGuard &right) = delete;
	CriticalSectionGuard(const CriticalSectionGuard &other) = delete;

private:
	CriticalSection *mLock;

};

