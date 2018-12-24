#include "GSchedulerManager.h"



GSchedulerManager::GSchedulerManager()
{

}

GSchedulerManager::~GSchedulerManager()
{

}

bool GSchedulerManager::schedule()
{
	if (m_queue.empty())
		return false;
	m_queue.swap(m_loopQueue);
	while (!m_loopQueue.empty())
	{
		GSchedulerPtr pScheduler = m_loopQueue.back();
		m_loopQueue.pop();
		pScheduler->schedule();
	}
	return true;
}

void GSchedulerManager::post(GSchedulerPtr pSchedule)
{
	m_queue.push(pSchedule);
}
