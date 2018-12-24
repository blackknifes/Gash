#ifndef __GSCHEDULER_H__
#define __GSCHEDULER_H__
#include <queue>
#include "GTask.h"
#include "../sync/GLocker.h"

GREFPTR_DEF(GScheduler);

class GScheduler: public virtual GObject
{
public:	
	GScheduler(GTaskPtr pTask): m_id(GenerateId<GScheduler, int>()), m_task(pTask) {}
	virtual void schedule() = 0;

	GTaskPtr getTask() const
	{
		return m_task;
	}

	int getId() const
	{
		return m_id;
	}
private:
	int m_id;
	GTaskPtr m_task;
};
#endif