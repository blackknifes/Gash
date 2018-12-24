#include "GConditionOvercount.h"



GConditionOvercount::GConditionOvercount(size_t maxCount)
	:GCondition(GCondition::Overcount), m_count(0), m_maxCount(maxCount)
{

}

bool GConditionOvercount::check()
{
	return m_count >= m_maxCount;
}

void GConditionOvercount::update()
{
	++m_count;
}
