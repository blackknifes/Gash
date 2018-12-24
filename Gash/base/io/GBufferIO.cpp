#include "GBufferIO.h"

#pragma warning(disable: 4018)

#define BytesOfBuffer(size) (size << 14)
#define SizeOfBuffer(size) (size >> 14)
#define AlignBuffer(size) (((size) + sizeof(Buffer) - 1) & ~(sizeof(Buffer) - 1))

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

GBufferIO::GBufferIO()
	:m_readOffset(0), m_writeOffset(0)
{

}

size_t GBufferIO::read(void* pData, size_t _size)
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
		size_t readSize = sizeof(Buffer) - offset;
		if (readSize > _size)
			readSize = _size;
		memcpy_s(ptr, readSize, pSrc, readSize);
		ptr += readSize;
		_size -= readSize;
	}
	while (_size != 0)
	{
		pSrc = (unsigned char*)m_bufferList[index++];
		size_t readSize = _size > sizeof(Buffer) ? sizeof(Buffer) : _size;
		memcpy_s(ptr, readSize, pSrc, readSize);
		ptr += readSize;
		_size -= readSize;
	}

	m_readOffset += total;
	if (index > 1)
	{
		m_bufferList.erase(m_bufferList.begin(), m_bufferList.begin() + index - 1);
		size_t removeSize = sizeof(Buffer) * (index - 1);
		m_readOffset -= removeSize;
		m_writeOffset -= removeSize;
	}

	return total;
}

size_t GBufferIO::write(const void* pData, size_t _size)
{
	reserve(m_writeOffset + _size);
	size_t total = _size;
	size_t index = SizeOfBuffer(m_writeOffset);
	size_t offset = m_writeOffset & 0x3FFF;
	m_writeOffset += _size;
	void* pDest = (unsigned char*)m_bufferList[index++] + offset;
	const unsigned char* ptr = (const unsigned char*)pData;
	{
		size_t writeSize = sizeof(Buffer) - offset;
		if (writeSize > _size)
			writeSize = _size;
		memcpy_s(pDest, writeSize, ptr, writeSize);
		ptr += writeSize;
		_size -= writeSize;
	}
	while (_size != 0)
	{
		pDest = (unsigned char*)m_bufferList[index++];
		size_t writeSize = _size > sizeof(Buffer) ? sizeof(Buffer) : _size;
		memcpy_s(pDest, writeSize, ptr, writeSize);
		ptr += writeSize;
		_size -= writeSize;
	}
	return total;
}

void GBufferIO::flush()
{

}

__int64 GBufferIO::seekRead(__int64 offset, SeekType pos /*= SeekCur*/)
{
	__int64 readOffset = m_readOffset;
	switch (pos)
	{
	case GIODevice::SeekCur:
		readOffset += offset;
		break;
	case GIODevice::SeekBegin:
		readOffset = offset;
		break;
	case GIODevice::SeekEnd:
		readOffset -= offset;
		break;
	default:
		break;
	}
	if (readOffset > m_writeOffset)
		readOffset = m_writeOffset;
	else if (readOffset < 0)
		readOffset = 0;
	return m_readOffset;
}

__int64 GBufferIO::seekWrite(__int64 offset, SeekType pos /*= SeekCur*/)
{
	return m_writeOffset;
}

__int64 GBufferIO::tellRead() const
{
	return m_readOffset;
}

__int64 GBufferIO::tellWrite() const
{
	return m_writeOffset;
}

void GBufferIO::close()
{
	clear();
}

bool GBufferIO::isClosed() const
{
	return false;
}

int GBufferIO::getErrorCode() const
{
	return 0;
}

GString GBufferIO::getErrorMessage() const
{
	return L"";
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
			readsize = read(pBuffer, total);
			m_readOffset += 2;
			return readsize;
		}

		--readsize;
		++total;
		if (++offset == sizeof(Buffer))
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
	if (capacity() > _capacity)
		return;
	size_t count = SizeOfBuffer(AlignBuffer(_capacity)) - m_bufferList.size();
	for (size_t i = 0; i < count; ++i)
		m_bufferList.push_back((Buffer*)malloc(sizeof(Buffer)));
}

void GBufferIO::clear()
{
	for (Buffer* pBuffer : m_bufferList)
		free(pBuffer);
	m_writeOffset = m_readOffset = 0;
	m_bufferList.clear();
}

GDataArray GBufferIO::readAll()
{
	GDataArray dataArray;
	dataArray.resize(this->size());
	read(dataArray.data(), this->size());
	return std::move(dataArray);
}
