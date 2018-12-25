#ifndef __GBUFFERIO_H__
#define __GBUFFERIO_H__
#include <vector>
#include "../GDataArray.h"
#include "GIODevice.h"

GREFPTR_DEF(GBufferIO);

class GBufferIO: public GIODevice
{
public:
	static const size_t buffer_size = 0x4000;
	static void* AllocBuffer();
	static void DeallocBuffer(void* pData);


	GBufferIO();
	virtual void close();
	virtual bool isClosed() const;
	virtual size_t readline(void* pBuffer, size_t bufsize);

	size_t size() const;
	size_t capacity() const;
	void reserve(size_t _capacity);

	void clear();
	GDataArray readAll();
	virtual void flush() override;
	virtual void* getNativeHandle() const override;

	virtual bool open() override;

	virtual bool write(const void* pData, size_t size, const WriteCallback& callback) override;
	virtual bool read(size_t size, const ReadCallback& callback) override;
	virtual size_t writeSync(const void* pData, size_t size) override;
	virtual size_t readSync(void* pData, size_t size) override;

private:
	GObjectClassPooled(GBufferIO);
	std::vector<void*> m_bufferList;
	size_t m_readOffset;
	size_t m_writeOffset;
};
#endif