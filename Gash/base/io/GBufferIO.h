#ifndef __GBUFFERIO_H__
#define __GBUFFERIO_H__

#include "GIODevice.h"
#include <vector>

class GBufferIO: public GIODevice
{
public:
	GBufferIO();
	virtual size_t read(void* pData, size_t bytes) override;
	virtual size_t write(const void* pData, size_t bytes) override;
	virtual void flush() override;
	virtual __int64 seekRead(__int64 offset, SeekType pos = SeekCur) override;
	virtual __int64 seekWrite(__int64 offset, SeekType pos = SeekCur) override;
	virtual __int64 tellRead() const override;
	virtual __int64 tellWrite() const override;
	virtual void close() override;
	virtual bool isClosed() const override;
	virtual int getErrorCode() const override;
	virtual GString getErrorMessage() const override;

	size_t size() const;
	size_t capacity() const;
	void reserve(size_t _size);
	void resize(size_t _size, unsigned char val = 0);

	GDataArray readAll();
private:
	GObjectImplements(GBufferIO);
	GObjectClass(GBufferIO);
	std::vector<GDataArray> m_dataArrayList;
	size_t m_dataSize;
	size_t m_readOffset;
	size_t m_writeOffset;
};
#endif