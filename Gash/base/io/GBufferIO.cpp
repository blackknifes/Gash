#include "GBufferIO.h"
#include "GBufferPool.h"

#pragma warning(disable: 4018)

#define BytesOfBuffer(size) (size << 14)
#define SizeOfBuffer(size) (size >> 14)
#define AlignBuffer(size) (((size) + GBufferIO::buffer_size - 1) & ~(GBufferIO::buffer_size - 1))

static GBufferPool<GBufferIO::buffer_size> s_bufferPool;

static bool IsNewLine(char ch, int& flag)
{
	switch (flag)
	{
	case 0:
		if (ch == '\r')
			++flag;
		break;
	case 1:
		if (ch == '\n')
			return true;
		flag = 0;
		break;
	default:
		break;
	}
	return false;
}

void* GBufferIO::AllocBuffer()
{
	return s_bufferPool.pop();
}

void GBufferIO::DeallocBuffer(void* pData)
{
	s_bufferPool.push(pData);
}

GBufferIO::GBufferIO()
	:m_readOffset(0), m_writeOffset(0)
{

}

bool GBufferIO::read(size_t size, const ReadCallback& callback)
{
	return false;
}

size_t GBufferIO::writeSync(const void* pData, size_t _size)
{
	if (_size == 0)
		return 0;
	reserve(m_writeOffset + _size);
	size_t total = _size;
	size_t index = SizeOfBuffer(m_writeOffset);
	size_t offset = m_writeOffset & 0x3FFF;
	m_writeOffset += _size;
	void* pDest = (unsigned char*)m_bufferList[index++] + offset;
	const unsigned char* ptr = (const unsigned char*)pData;
	{
		size_t writeSize = GBufferIO::buffer_size - offset;
		if (writeSize > _size)
			writeSize = _size;
		memcpy_s(pDest, writeSize, ptr, writeSize);
		ptr += writeSize;
		_size -= writeSize;
	}
	while (_size != 0)
	{
		pDest = (unsigned char*)m_bufferList[index++];
		size_t writeSize = _size > GBufferIO::buffer_size ? GBufferIO::buffer_size : _size;
		memcpy_s(pDest, writeSize, ptr, writeSize);
		ptr += writeSize;
		_size -= writeSize;
	}
	return _size;
}

size_t GBufferIO::readSync(void* pData, size_t _size)
{
	{
		size_t readableSize = this->size();
		if (readableSize == 0)
			return 0;
		if (readableSize < _size)
			_size = readableSize;
	}
	size_t total = _size;
	size_t index = SizeOfBuffer(m_readOffset);
	size_t offset = m_readOffset & 0x3FFF;
	void* pSrc = (unsigned char*)m_bufferList[index++] + offset;
	unsigned char* ptr = (unsigned char*)pData;

	{
		size_t readSize = GBufferIO::buffer_size - offset;
		if (readSize > _size)
			readSize = _size;
		memcpy_s(ptr, readSize, pSrc, readSize);
		ptr += readSize;
		_size -= readSize;
	}
	while (_size != 0)
	{
		pSrc = (unsigned char*)m_bufferList[index++];
		size_t readSize = _size > GBufferIO::buffer_size ? GBufferIO::buffer_size : _size;
		memcpy_s(ptr, readSize, pSrc, readSize);
		ptr += readSize;
		_size -= readSize;
	}

	m_readOffset += total;
	if (index > 1)
	{
		m_bufferList.erase(m_bufferList.begin(), m_bufferList.begin() + index - 1);
		size_t removeSize = GBufferIO::buffer_size * (index - 1);
		m_readOffset -= removeSize;
		m_writeOffset -= removeSize;
	}
	return total;
}

void GBufferIO::flush()
{

}
void* GBufferIO::getNativeHandle() const
{
	return nullptr;
}

bool GBufferIO::write(const void* pData, size_t size, const WriteCallback& callback)
{
	return false;
}

void GBufferIO::close()
{
	clear();
}

bool GBufferIO::isClosed() const
{
	return false;
}

size_t GBufferIO::readline(void* pBuffer, size_t bufsize)
{
	if (size() < 2)
		return -1;

	int flag = 0;

	size_t readsize = this->size();
	size_t total = 0;

	size_t index = SizeOfBuffer(m_readOffset);
	size_t offset = m_readOffset & 0x3FFF;
	char* pDest = (char*)m_bufferList[index++] + offset;
	while (readsize != 0)
	{
		if (IsNewLine(*pDest, flag))
		{
			--total;
			if (total > bufsize)
				return total;
			readsize = readSync(pBuffer, total);
			m_readOffset += 2;
			return readsize;
		}

		--readsize;
		++total;
		if (++offset == GBufferIO::buffer_size)
			pDest = (char*)m_bufferList[index++];
		else
			++pDest;
	}

	return -1;
}

size_t GBufferIO::size() const
{
	return m_writeOffset - m_readOffset;
}

size_t GBufferIO::capacity() const
{
	return BytesOfBuffer(m_bufferList.size());
}

void GBufferIO::reserve(size_t _capacity)
{
	if (capacity() >= _capacity)
		return;
	size_t count = SizeOfBuffer(AlignBuffer(_capacity)) - m_bufferList.size();
	for (size_t i = 0; i < count; ++i)
		m_bufferList.push_back(s_bufferPool.pop());
}

void GBufferIO::clear()
{
	for (void* pBuffer : m_bufferList)
		s_bufferPool.push(pBuffer);
	m_writeOffset = m_readOffset = 0;
	m_bufferList.clear();
}

GDataArray GBufferIO::readAll()
{
	GDataArray dataArray;
	dataArray.resize(this->size());
	readSync(dataArray.data(), this->size());
	return std::move(dataArray);
}

bool GBufferIO::open()
{
	return true;
}