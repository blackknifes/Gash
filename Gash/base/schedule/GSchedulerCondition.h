#ifndef __GSCHEDULERCONDITION_H__
#define __GSCHEDULERCONDITION_H__
#include "condition/GCondition.h"
#include "GScheduler.h"

class GSchedulerCondition: public GScheduler
{
public:
	virtual void schedule() override;
private:
	GObjectClassPooled(GSchedulerCondition);
	GSchedulerCondition(GTaskPtr pTask, GConditionPtr pCondition, GConditionPtr pCancelCondition);
	GConditionPtr m_pCondition;
	GConditionPtr m_pCancelCondition;
};
#endif