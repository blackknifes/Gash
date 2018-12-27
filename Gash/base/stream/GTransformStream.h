#ifndef __GTRANSFORMSTREAM_H__
#define __GTRANSFORMSTREAM_H__
#include <functional>
#include "GStream.h"
#include "../io/GBufferIO.h"

GREFPTR_DEF(GTransformStream);

class GTransformStream: public GOutputStream
{
public:
	typedef std::function<void(const void*, size_t, GBufferIO& dataArray)> Callback;

	static GTransformStreamPtr Create(const Callback& callback);

	GTransformStream(const Callback& callback);
	~GTransformStream();

	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;
private:
	GObjectClassPooled(GTransformStream);
	Callback m_callback;
	GBufferIO m_bufferIO;
};
#endif