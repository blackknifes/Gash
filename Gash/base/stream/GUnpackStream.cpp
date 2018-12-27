#include "GUnpackStream.h"

GUnpackStream::GUnpackStream(GOutputStreamPtr output)
	:m_pOutputStream(output), m_expectIndex(0)
{

}

GUnpackStream::~GUnpackStream()
{

}

void GUnpackStream::setPackTail(const void* pData, size_t size)
{
	m_expectData.assign((char*)pData, size);
}

void GUnpackStream::setPackTail(const char* str)
{
	m_expectData = str;
}

void GUnpackStream::reset()
{
	m_expectData.clear();
}

void GUnpackStream::write(const void* pData, size_t _size)
{
	if(m_expectData.empty())
		return m_pOutputStream->write(pData, _size);
	const char* ptr = (const char*)pData;
	size_t preIndex = 0;
	for (size_t i = 0; i < _size; ++i)
	{
		if (ptr[i] != m_expectData[m_expectIndex])
		{
			if (ptr[i] == m_expectData[0] && ++m_expectIndex == m_expectData.size())
			{
				m_expectIndex = 0;
				flush();
				continue;
			}
			if (m_expectIndex != 0)
			{
				m_bufferIO.writeSync(m_expectData.data(), m_expectIndex);
				m_expectIndex = 0;
			}
			m_bufferIO.writeSync(ptr + i, 1);
		}
		else if (++m_expectIndex == m_expectData.size())
		{
			m_expectIndex = 0;
			flush();
		}
	}
}

void GUnpackStream::flush()
{
	char buf[0x4000];
	size_t readsize;
	while ((readsize = m_bufferIO.readSync(buf, sizeof(buf))) != 0)
		m_pOutputStream->write(buf, readsize);
}
