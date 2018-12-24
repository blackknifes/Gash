#ifndef __GOVERLAPPEDCALLBACK_H__
#define __GOVERLAPPEDCALLBACK_H__
#include "../GObject.h"
#include "../device/GDevice.h"

class GOverlappedCallback: public GObject
{
public:
	virtual void call(GDevice* pDevice) = 0;
private:
	GObjectClassPooled(GOverlappedCallback);
};
#endif