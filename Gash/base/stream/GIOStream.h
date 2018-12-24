#ifndef __GIOSTREAM_H__
#define __GIOSTREAM_H__

#include <iostream>
#include "GStream.h"

GREFPTR_DEF(GIOStream);

class GIOStream: public GInputStream, public GOutputStream
{
public:
	GIOStream();
	virtual size_t read(void* pData, size_t _size) override;
	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;
private:
	GObjectClassPooled(GIOStream);
};
#endif