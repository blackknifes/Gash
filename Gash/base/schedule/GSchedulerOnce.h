#ifndef __GSCHEDULERONCE_H__
#define __GSCHEDULERONCE_H__
#include "GScheduler.h"

class GSchedulerOnce: public GScheduler
{
public:
	GSchedulerOnce(GTaskPtr pTask);
	virtual void schedule() override;
private:
	GObjectClassPooled(GSchedulerOnce);
};
#endif