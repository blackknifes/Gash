#include "GConditionOnce.h"



GConditionOnce::GConditionOnce()
	:GCondition(GCondition::Once), m_once(true)
{

}

bool GConditionOnce::check()
{
	return m_once;
}

void GConditionOnce::update()
{
	m_once = false;
}
