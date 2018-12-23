#include "GBufferIO.h"

#pragma warning(disable: 4018)

#define ALIGN_4096(x) (((x) + 4096 - 1) & ~(4096 - 1))

GBufferIO::GBufferIO()
	:m_dataSize(0), m_readOffset(0), m_writeOffset(0)
{
	m_dataArrayList.push_back(GDataArray(4096));
}

size_t GBufferIO::read(void* pData, size_t bytes)
{
	if (m_readOffset + bytes > m_dataSize)
		bytes = m_dataSize - m_readOffset;
	if (bytes == 0)
		return 0;
	size_t index = m_readOffset >> 12;
	size_t offset = m_readOffset & 0xFFF;
	GDataArray* pDataArray = &m_dataArrayList[index++];
	unsigned char* pDataBytes = (unsigned char*)pData;
	size_t total = bytes;
	{
		size_t readSize = 4096 - offset;
		if (readSize > bytes)
			readSize = bytes;
		pDataArray->read(pDataBytes, readSize, offset);
		bytes -= readSize;
		pDataBytes += readSize;
	}

	while (bytes != 0)
	{
		size_t readSize = 4096;
		if (readSize > bytes)
			readSize = bytes;
		pDataArray = &m_dataArrayList[index++];
		pDataArray->read(pDataBytes, readSize, 0);
		bytes -= readSize;
		pDataBytes += readSize;
	}
	m_readOffset += total;

	return total;
}

size_t GBufferIO::write(const void* pData, size_t bytes)
{
	size_t index = m_writeOffset >> 12;
	reserve(m_writeOffset + bytes);
	GDataArray* pDataArray = &m_dataArrayList[index++];
	const unsigned char* pDataBytes = (const unsigned char*)pData;
	size_t total = bytes;
	{
		size_t writeSize = 4096 - pDataArray->size();
		if (writeSize > bytes)
			writeSize = bytes;
		pDataArray->write(pData, writeSize, pDataArray->size());
		bytes -= writeSize;
		pDataBytes += writeSize;
	}

	while (bytes != 0)
	{
		size_t writeSize = 4096;
		if (writeSize > bytes)
			writeSize = bytes;
		pDataArray = &m_dataArrayList[index++];
		pDataArray->write(pData, writeSize, pDataArray->size());
		bytes -= writeSize;
		pDataBytes += writeSize;
	}
	m_writeOffset += total;
	m_dataSize += total;

	return total;
}

void GBufferIO::flush()
{
	
}

__int64 GBufferIO::seekRead(__int64 offset, SeekType pos /*= SeekCur*/)
{
	switch (pos)
	{
	case GIODevice::SeekCur:
		m_readOffset = (offset + m_readOffset) > m_dataSize ? m_readOffset : offset + m_readOffset;
		break;
	case GIODevice::SeekBegin:
		m_readOffset = offset > m_dataSize ? m_dataSize : offset;
		break;
	case GIODevice::SeekEnd:
		m_readOffset = offset > m_dataSize ? 0 : m_dataSize - offset;
		break;
	default:
		break;
	}
	return m_readOffset;
}

__int64 GBufferIO::seekWrite(__int64 offset, SeekType pos /*= SeekCur*/)
{
	switch (pos)
	{
	case GIODevice::SeekCur:
		m_writeOffset = (offset + m_writeOffset) > m_dataSize ? m_writeOffset : offset + m_writeOffset;
		break;
	case GIODevice::SeekBegin:
		m_writeOffset = offset > m_dataSize ? m_dataSize : offset;
		break;
	case GIODevice::SeekEnd:
		m_writeOffset = offset > m_dataSize ? 0 : m_dataSize - offset;
		break;
	default:
		break;
	}
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

size_t GBufferIO::size() const
{
	return m_dataSize;
}

size_t GBufferIO::capacity() const
{
	return m_dataArrayList.size() << 12;
}

void GBufferIO::reserve(size_t _size)
{
	size_t bufCount = (ALIGN_4096(_size) >> 12);
	if (m_dataArrayList.size() < bufCount)
	{
		bufCount -= m_dataArrayList.size();
		for (size_t i = 0; i < bufCount; ++i)
			m_dataArrayList.push_back(GDataArray(4096));
	}
}

void GBufferIO::resize(size_t _size, unsigned char val /*= 0*/)
{
	if (m_dataSize > _size)
	{
		m_dataSize = _size;
		return;
	}
	reserve(_size);
	size_t writeSize = _size - m_dataSize;
	for (size_t i = 0; i < writeSize; ++i)
		write(&val, 1);
}

GDataArray GBufferIO::readAll()
{
	size_t readSize = size() - m_readOffset;
	GDataArray dataArray(readSize);
	read(dataArray.data(), readSize);
	return std::move(dataArray);
}
