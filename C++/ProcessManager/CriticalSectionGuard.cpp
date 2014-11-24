#include "CriticalSectionGuard.h"


CriticalSectionGuard::CriticalSectionGuard(CriticalSection *cs)
{
	mLock = cs;
	mLock->enter();
}


CriticalSectionGuard::~CriticalSectionGuard()
{
	mLock->leave();
}

CriticalSection::CriticalSection()
{
	::InitializeCriticalSection(&mCriticalSection);
}

CriticalSection::~CriticalSection()
{
	::DeleteCriticalSection(&mCriticalSection);
}

void CriticalSection::enter()
{
	::EnterCriticalSection(&mCriticalSection);
}

void CriticalSection::leave()
{
	::LeaveCriticalSection(&mCriticalSection);
}