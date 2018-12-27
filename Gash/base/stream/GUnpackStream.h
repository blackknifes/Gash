#ifndef __GUNPACKSTREAM_H__
#define __GUNPACKSTREAM_H__
#include "GStream.h"
#include "../io/GBufferIO.h"

GREFPTR_DEF(GUnpackStream);

class GUnpackStream: public GOutputStream
{
public:
	GUnpackStream(GOutputStreamPtr output);
	~GUnpackStream();

	void setPackTail(const void* pData, size_t size);
	void setPackTail(const char* str);
	void reset();
	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;
private:
	GObjectClassPooled(GUnpackStream);
	GOutputStreamPtr m_pOutputStream;
	std::string m_expectData;
	size_t m_expectIndex;
	GBufferIO m_bufferIO;
};
#endif