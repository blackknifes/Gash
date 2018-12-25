#ifndef __GCALLABLE_H__
#define __GCALLABLE_H__
#include "GObject.h"
#include <stdarg.h>

class GCallable: public virtual GObject
{
public:
	void call(int args, ...)
	{
		va_list ap;
		va_start(ap, args);
		callList(args, ap);
		va_end(ap);
	}
	virtual void callList(int args, va_list ap) = 0;
};
#endif