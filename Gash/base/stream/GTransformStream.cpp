#include "GTransformStream.h"



GTransformStreamPtr GTransformStream::Create(const Callback& callback)
{
	return GTransformStream::CreatePooled(callback);
}

GTransformStream::GTransformStream(const Callback& callback)
	:m_callback(callback)
{

}

GTransformStream::~GTransformStream()
{

}

void GTransformStream::write(const void* pData, size_t _size)
{
	m_callback(pData, _size, m_bufferIO);
}

void GTransformStream::flush()
{
	
}
