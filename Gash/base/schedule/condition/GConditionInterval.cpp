#include "GConditionInterval.h"

GConditionInterval::GConditionInterval(__int64 inteval)
	:GCondition(GCondition::Inteval), m_inteval(inteval), m_pretime(GetTickCount64())
{

}

bool GConditionInterval::check()
{
	__int64 curtime = GetTickCount64();
	if (curtime - m_pretime < m_inteval)
		return false;
	m_pretime = curtime;
	return true;
}