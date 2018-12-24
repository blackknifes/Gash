#include "GConditionCallback.h"



bool GConditionCallback::check()
{
	return m_callback();
}

void GConditionCallback::update()
{
	if (m_updater)
		m_updater();
}

GConditionCallback::GConditionCallback(const Callback& callback, const Updater& updater /*= nullptr*/)
	:GCondition(GCondition::Callback), m_callback(callback), m_updater(updater)
{

}
