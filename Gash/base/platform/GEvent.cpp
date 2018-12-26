#include "GEvent.h"
#include "win32/GWin32Event.h"



GEventPtr GEvent::Create()
{
	return GWin32Event::CreatePooled();
}

GEventPtr GEvent::Create(const GString& name)
{
	GEventPtr pEvent = GWin32Event::CreatePooled();
	pEvent->setName(name);
	return pEvent;
}

bool GEvent::open(const GString& name)
{
	close();
	setName(name);
	return open();
}
