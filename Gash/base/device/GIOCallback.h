#ifndef __GIOCALLBACK_H__
#define __GIOCALLBACK_H__
#include "../GObject.h"
#include "../GCallable.h"
#include "../io/GIODevice.h"

GREFPTR_DEF(GAsyncIO);
GREFPTR_DEF(GIOCallback);

class GIOCallback : public GCallable
{
public:
	GIOCallback() {};
	virtual void call(GIODevicePtr pDevice, GAsyncIOPtr pIO) = 0;
	virtual void callList(int args, va_list ap) override;

private:
	GObjectClassPooled(GIOCallback);
};
#endif