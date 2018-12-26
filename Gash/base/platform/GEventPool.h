#ifndef __GEVENTPOOL_H__
#define __GEVENTPOOL_H__
#include "../block_container/GBlockingStack.h"
#include "../GSingleton.h"
#include "GEvent.h"

class GEventPool: public GSingleton<GEventPool>
{
public:
	GEventPool();
	~GEventPool();

	static GEventPtr PopEvent();
	static void PushEvent(GEventPtr pEvent);

	GEventPtr pop();
	void push(GEventPtr pEvent);
private:
	GBlockingStack<GEventPtr> m_eventPool;
};
#endif