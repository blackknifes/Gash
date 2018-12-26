#ifndef __GIODEVICESYNC_H__
#define __GIODEVICESYNC_H__
#include "../../GObject.h"
#include "../GIOEnums.h"

class GIODeviceSync: public virtual GObject
{
public:
	virtual size_t read(void* pData, size_t size) = 0;
	virtual size_t write(const void* pData, size_t size) = 0;
	virtual void flush() = 0;
	virtual bool isClosed() const = 0;
	virtual void close() = 0;
	virtual void* getNativeHandle() const = 0;
};
#endif