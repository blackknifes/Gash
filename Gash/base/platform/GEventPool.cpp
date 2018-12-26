#include "GEventPool.h"



GEventPool::GEventPool()
{

}

GEventPool::~GEventPool()
{

}

GEventPtr GEventPool::PopEvent()
{
	return GEventPool::getInstance()->pop();
}

void GEventPool::PushEvent(GEventPtr pEvent)
{
	GEventPool::getInstance()->push(pEvent);
}

GEventPtr GEventPool::pop()
{
	if (m_eventPool.empty())
	{
		std::vector<GEventPtr> pEventList;
		for (size_t i = 0; i < 4; ++i)
		{
			GEventPtr pEvent = GEvent::Create();
			if(pEvent->open())
				pEventList.push_back(pEvent);
		}
		m_eventPool.push(pEventList);
	}
	return m_eventPool.pop();
}

void GEventPool::push(GEventPtr pEvent)
{
	m_eventPool.push(pEvent);
}
