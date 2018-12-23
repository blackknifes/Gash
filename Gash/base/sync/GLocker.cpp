#include "GLocker.h"



GThreadLocker::GThreadLocker(DWORD spainCount)
{
	InitializeCriticalSectionAndSpinCount(&m_section, spainCount);
}

GThreadLocker::GThreadLocker()
{
	InitializeCriticalSection(&m_section);
}

GThreadLocker::~GThreadLocker()
{
	DeleteCriticalSection(&m_section);
}

void GThreadLocker::lock() const
{
	EnterCriticalSection((CRITICAL_SECTION*)&m_section);
}

void GThreadLocker::unlock() const
{
	LeaveCriticalSection((CRITICAL_SECTION*)&m_section);
}
