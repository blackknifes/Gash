#include "GConditionOr.h"


GConditionOr::GConditionOr(GConditionPtr pCondition, ...)
	:GCondition(GCondition::Or)
{
	if (pCondition != nullptr)
	{
		m_conditionList.push_back(pCondition);
		va_list ap;
		va_start(ap, pCondition);
		while ((pCondition = va_arg(ap, GConditionPtr)) != nullptr)
			m_conditionList.push_back(pCondition);
		va_end(ap);
	}
}

bool GConditionOr::check()
{
	for (GConditionPtr& pCond: m_conditionList)
	{
		if (pCond->check())
			return true;
	}
	return false;
}

void GConditionOr::update()
{
	for (GConditionPtr& pCond : m_conditionList)
		pCond->update();	
}
