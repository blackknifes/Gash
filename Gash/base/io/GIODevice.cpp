#include "GIODevice.h"



size_t GIODevice::write(const GDataArray& pBuffer)
{
	return this->write(pBuffer.data(), pBuffer.size());
}

GDataArray GIODevice::read(size_t bytes)
{
	GDataArray dataArray(bytes);
	bytes = this->read(dataArray.data(), bytes);
	dataArray.resize(bytes);
	return std::move(dataArray);
}

GDataArray GIODevice::readAll()
{
	if (isClosed())
		return GDataArray();
	__int64 offset = tellRead();
	__int64 _size = seekRead(0, SeekEnd) - offset;
	seekRead(offset, SeekBegin);
	GDataArray buffer(_size);
	read(buffer.data(), _size);
	return std::move(buffer);
}

void GIODevice::resetRead()
{
	seekRead(0, SeekBegin);
}

void GIODevice::resetWrite()
{
	seekWrite(0, SeekBegin);
}

void GIODevice::reset()
{
	resetRead(); resetWrite();
}
