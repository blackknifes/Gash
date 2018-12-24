#include "GConditionNot.h"


GConditionNot::GConditionNot(GConditionPtr pCondition)
	:GCondition(GCondition::Not), m_pCondition(pCondition)
{

}


bool GConditionNot::check()
{
	return !m_pCondition->check();
}
void GConditionNot::update()
{
	m_pCondition->update();
}
