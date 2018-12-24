#include "GBufferStream.h"
#include "..\io\GBufferIO.h"

GBufferStream::GBufferStream()
{
	setDevice(new GBufferIO());
}

size_t GBufferStream::read(void* pData, size_t _size)
{
	return getDevice()->read(pData, _size);
}

void GBufferStream::write(const void* pData, size_t _size)
{
	getDevice()->write(pData, _size);
}

void GBufferStream::flush()
{
	
}

size_t GBufferStream::readline(void* pBuffer, size_t bufsize)
{
	return static_cast<GBufferIO*>(getDevice().get())->readline(pBuffer, bufsize);
}

size_t GBufferStream::size() const
{
	return static_cast<GBufferIO*>(getDevice().get())->size();
}

size_t GBufferStream::capacity() const
{
	return static_cast<GBufferIO*>(getDevice().get())->capacity();
}

GDataArray GBufferStream::readAll()
{
	return static_cast<GBufferIO*>(getDevice().get())->readAll();
}
