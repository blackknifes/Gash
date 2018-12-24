#include "GConditionAnd.h"


GConditionAnd::GConditionAnd(GConditionPtr pCondition, ...)
	:GCondition(And)
{
	if (pCondition != nullptr)
	{
		va_list ap;
		va_start(ap, pCondition);
		m_conditionList.push_back(pCondition);
		while ((pCondition = va_arg(ap, GConditionPtr)) != nullptr)
			m_conditionList.push_back(pCondition);
		va_end(ap);
	}
}

bool GConditionAnd::check()
{
	for (auto itor = m_conditionList.begin(); itor != m_conditionList.end(); ++itor)
	{
		if (!(*itor)->check())
			return false;
	}
	return true;
}

void GConditionAnd::update()
{
	for (auto itor = m_conditionList.begin(); itor != m_conditionList.end(); ++itor)
		(*itor)->update();
}
