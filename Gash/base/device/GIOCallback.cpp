#include "GIOCallback.h"
#include "GAsyncIO.h"

void GIOCallback::callList(int args, va_list ap)
{
	if (args < 2)
		return;
	GIODevicePtr pDevice = va_arg(ap, GIODevice*);
	GAsyncIOPtr pIO = va_arg(ap, GAsyncIO*);
	call(pDevice, pIO);
}
