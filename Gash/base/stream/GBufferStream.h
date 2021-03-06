#ifndef __GBUFFERSTREAM_H__
#define __GBUFFERSTREAM_H__

#include "GStream.h"

GREFPTR_DEF(GBufferStream);

class GBufferStream: public GInputStream, public GOutputStream, public virtual GStreamDevice
{
public:
	GBufferStream();
	virtual size_t read(void* pData, size_t _size) override;
	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;

	virtual size_t readline(void* pBuffer, size_t bufsize);

	size_t size() const;
	size_t capacity() const;
	GDataArray readAll();
private:
	GObjectClassPooled(GBufferStream);
};
#endif