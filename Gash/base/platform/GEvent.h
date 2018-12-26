#ifndef __GEVENT_H__
#define __GEVENT_H__
#include "../GObject.h"

GREFPTR_DEF(GEvent);

class GEvent: public virtual GObject
{
public:
	static GEventPtr Create();
	static GEventPtr Create(const GString& name);

	virtual GString getName() const = 0;
	virtual void setName(const GString& name) = 0;
	virtual bool open() = 0;
	virtual void close() = 0;
	virtual bool wait(DWORD timeout = INFINITE) = 0;
	virtual void signal() = 0;
	virtual bool isClosed() const = 0;
	virtual void* getNativeHandle() const = 0;


	bool open(const GString& name);
};
#endif