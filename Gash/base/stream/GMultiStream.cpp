#include "GMultiStream.h"

GMultiOutputStream::GMultiOutputStream()
{

}

GMultiOutputStream::GMultiOutputStream(GOutputStreamPtr os, ...)
{
	if (os != nullptr)
	{
		m_outputStreamList.push_back(os);
		va_list ap;
		va_start(ap, os);
		os = va_arg(ap, GOutputStreamPtr);
		while (os != nullptr)
		{
			m_outputStreamList.push_back(os);
			os = va_arg(ap, GOutputStreamPtr);
		}
		va_end(ap);
	}
}

void GMultiOutputStream::add(GOutputStreamPtr os, ...)
{
	if (os != nullptr)
	{
		m_outputStreamList.push_back(os);
		va_list ap;
		va_start(ap, os);
		os = va_arg(ap, GOutputStreamPtr);
		while (os != nullptr)
		{
			m_outputStreamList.push_back(os);
			os = va_arg(ap, GOutputStreamPtr);
		}
		va_end(ap);
	}
}

void GMultiOutputStream::write(const void* pData, size_t _size)
{
	for (GOutputStreamPtr& pOut: m_outputStreamList)
		pOut->write(pData, _size);
}

void GMultiOutputStream::flush()
{
	for (GOutputStreamPtr& pOut : m_outputStreamList)
		pOut->flush();
}

void GMultiOutputStream::end()
{
	for (GOutputStreamPtr& pOut : m_outputStreamList)
		pOut->end();
}
