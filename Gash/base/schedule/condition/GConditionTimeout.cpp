#include "GConditionTimeout.h"
#include <time.h>



GConditionTimeout::GConditionTimeout()
	:GCondition(GCondition::Timeout)
{

}

GConditionTimeoutPtr GConditionTimeout::CreateByMilliSeconds(DWORD millisecond)
{
	GConditionTimeoutPtr pCond = new GConditionTimeout();
	pCond->m_millisecond = ::GetTickCount64() + millisecond;
	return pCond;
}

GConditionTimeoutPtr GConditionTimeout::CreateBySeconds(DWORD second, DWORD millisecond /*= 0*/)
{
	GConditionTimeoutPtr pCond = new GConditionTimeout();
	pCond->m_millisecond = ::GetTickCount64() + millisecond % 1000 + second * 1000;
	return pCond;
}

GConditionTimeoutPtr GConditionTimeout::CreateByMinutes(DWORD minutes, DWORD second /*= 0*/, DWORD millisecond /*= 0*/)
{
	GConditionTimeoutPtr pCond = new GConditionTimeout();
	pCond->m_millisecond = ::GetTickCount64() + millisecond % 1000 + second * 1000 + minutes * 60 * 1000;
	return pCond;
}

GConditionTimeoutPtr GConditionTimeout::CreateByHours(DWORD hours, DWORD minutes /*= 0*/, DWORD second /*= 0*/, DWORD millisecond /*= 0*/)
{
	GConditionTimeoutPtr pCond = new GConditionTimeout();
	pCond->m_millisecond = ::GetTickCount64() + millisecond % 1000 + second * 1000 + minutes * 60 * 1000 + hours * 60 * 60 * 1000;
	return pCond;
}

GConditionTimeoutPtr GConditionTimeout::CreateByDays(DWORD days, DWORD hours /*= 0*/, DWORD minutes /*= 0*/, DWORD second /*= 0*/, DWORD millisecond /*= 0*/)
{
	GConditionTimeoutPtr pCond = new GConditionTimeout();
	pCond->m_millisecond = ::GetTickCount64() + millisecond % 1000 + second * 1000 + minutes * 60 * 1000 + hours * 60 * 60 * 1000 +
		days * 24 * 60 * 60 * 1000;
	return pCond;
}

bool GConditionTimeout::check()
{
	return GetTickCount64() >= m_millisecond;
}