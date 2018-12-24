#include "GAsyncLoop.h"

static GAsyncLoop* s_pDefaultLoop = nullptr;

GAsyncLoop* GAsyncLoop::GetDefault()
{
	if (s_pDefaultLoop == nullptr)
		s_pDefaultLoop = new GAsyncLoop();
	return s_pDefaultLoop;
}

GAsyncEnginePtr GAsyncLoop::GetDefaultEngine()
{
	return GetDefault()->m_pAsyncEngine;
}

void GAsyncLoop::DestroyDefault()
{
	if (s_pDefaultLoop)
	{
		delete s_pDefaultLoop;
		s_pDefaultLoop = nullptr;
	}
}

GAsyncLoop::GAsyncLoop()
	:m_loopFlag(true)
{
	m_pAsyncEngine = new GAsyncEngine();
}


void GAsyncLoop::removeIdle(GAsyncIdlePtr idle)
{
	for (auto itor = m_idleList.begin(); itor != m_idleList.end(); ++itor)
	{
		if (*itor == idle)
		{
			m_idleList.erase(itor);
			break;
		}
	}
}

void GAsyncLoop::addIdle(GAsyncIdlePtr idle)
{
	m_idleList.push_back(idle);
}

void GAsyncLoop::addFirstIdle(GAsyncIdlePtr idle)
{
	m_idleList.insert(m_idleList.begin(), idle);
}

void GAsyncLoop::loop()
{
	m_loopFlag = true;
	while (m_loopFlag && (!m_idleList.empty() || GHasPendingIO()))
		doOnceLoop();
}

void GAsyncLoop::doOnceLoop()
{
	GAsyncDevice* deviceList[16];
	GAsyncIO* ioList[16];
	size_t size;
	while ((size = m_pAsyncEngine->getIOTaskList(deviceList, ioList, 16, 200)) != 0)
	{
		for (size_t i = 0; i < size; ++i)
			m_pAsyncEngine->doAsyncIO(deviceList[i], ioList[i]);
	}

	m_loopList.assign(m_idleList.begin(), m_idleList.end());
	for (auto itor = m_loopList.begin(); itor != m_loopList.end(); ++itor)
		(*itor)->call();
	m_loopList.clear();
}

void GAsyncLoop::quit()
{
	m_loopFlag = false;
}

void GAsyncIdle::addThis()
{
	GAsyncLoop::GetDefault()->addIdle(this);
}

void GAsyncIdle::removeThis()
{
	GAsyncLoop::GetDefault()->removeIdle(this);
}
