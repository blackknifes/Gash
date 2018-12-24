#ifndef __GSCHEDULERMANAGER_H__
#define __GSCHEDULERMANAGER_H__
#include "../block_container/GBlockingQueue.h"
#include "GScheduler.h"

#define _schedulerManager GSchedulerManager::getInstance()

class GSchedulerManager: public GSingleton<GSchedulerManager>
{
public:
	GSchedulerManager();
	~GSchedulerManager();

	bool schedule();
	void post(GSchedulerPtr pSchedule);
private:
	GBlockingQueue<GScheduler> m_queue;
	GBlockingQueue<GScheduler>::queue_type m_loopQueue;
};
#endif