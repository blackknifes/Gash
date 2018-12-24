#ifndef __GASYNCLOOP_H__
#define __GASYNCLOOP_H__
#include "GAsyncEngine.h"
#include <functional>
#include <atomic>

GREFPTR_DEF(GAsyncIdle);

class GAsyncIdle: public GObject
{
public:
	void addThis();
	void removeThis();
	virtual void call() = 0;
};

class GAsyncLoop
{
public:
	static GAsyncLoop* GetDefault();
	static GAsyncEnginePtr GetDefaultEngine();
	static void DestroyDefault();
	GAsyncLoop();
	
	void removeIdle(GAsyncIdlePtr idle);
	void addIdle(GAsyncIdlePtr idle);
	void addFirstIdle(GAsyncIdlePtr idle);
	void loop();
	void doOnceLoop();
	void quit();
private:
	GAsyncEnginePtr m_pAsyncEngine;
	std::vector<GAsyncIdlePtr> m_idleList;
	std::vector<GAsyncIdlePtr> m_loopList;
	std::atomic<bool> m_loopFlag;
};
#endif