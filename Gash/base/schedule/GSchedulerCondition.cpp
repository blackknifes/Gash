#include "GSchedulerCondition.h"
#include "GSchedulerManager.h"

GSchedulerCondition::GSchedulerCondition(GTaskPtr pTask, GConditionPtr pCondition, GConditionPtr pCancelCondition)
	:GScheduler(pTask), m_pCondition(pCondition), m_pCancelCondition(pCancelCondition)
{

}

void GSchedulerCondition::schedule()
{
	bool result = m_pCondition->check();

	if (result)
	{
		m_pCancelCondition->update();
		getTask()->execute();
	}

	if (!m_pCancelCondition->check())
		_schedulerManager->post(this);
}
