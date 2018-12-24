#include "GConditionEvent.h"



bool GConditionEvent::check()
{
	return ::WaitForSingleObject(m_hEvent, 0) == WAIT_OBJECT_0;
}

void GConditionEvent::active()
{
	SetEvent(m_hEvent);
}

GConditionEvent::GConditionEvent()
	:GCondition(GCondition::Event), m_needDestroy(true)
{
	CreateEventW(nullptr, FALSE, FALSE, nullptr);
}

GConditionEvent::GConditionEvent(const GString& str, bool active /*= false*/)
	: GCondition(GCondition::Event), m_needDestroy(true)
{
	CreateEventW(nullptr, FALSE, active ? TRUE : FALSE, str.data());
}

GConditionEvent::GConditionEvent(HANDLE hEvent)
	: GCondition(GCondition::Event), m_hEvent(hEvent), m_needDestroy(false)
{

}

GConditionEvent::~GConditionEvent()
{
	if (m_needDestroy)
		::CloseHandle(m_hEvent);
}
