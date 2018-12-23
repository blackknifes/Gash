#include "GTextStream.h"

template<typename _Ty>
static size_t ConvertToString(void* pData, const void* format, const _Ty& val, 
	GDataArray::Encoding encoding)
{
	size_t size = 0;
	switch (encoding)
	{
	case GDataArray::EncodingUtf8:
	case GDataArray::EncodingAnsi:
		size = sprintf_s((char*)pData, 64, (const char*)format, val);
		break;
	case GDataArray::EncodingUnicode:
		size = swprintf_s((wchar_t*)pData, 64, (const wchar_t*)format, val) << 1;
		break;
	default:
		break;
	}
	return size;
}

template<typename _Ty>
static void WriteValue(GOutputStreamPtr pOut, const char* fmt, const _Ty& val, 
	GDataArray::Encoding encoding)
{
	unsigned char buf[128];
	size_t size = 0;
	switch (encoding)
	{
	case GDataArray::EncodingUtf8:
	case GDataArray::EncodingAnsi:
		size = ConvertToString(buf, fmt, val);
		break;
	case GDataArray::EncodingUnicode:
	{
		wchar_t wfmt[16] = {};
		for (size_t i = 0; i < strlen(fmt); ++i)
			wfmt[i] = fmt[i];
		size = ConvertToString(buf, wfmt, val);
	}
		break;
	default:
		break;
	}
	pOut->write(buf, size);
}

GTextInputStream::GTextInputStream(GInputStreamPtr is, Encoding encoding /*= Encoding::EncodingUtf8*/)
	:m_pInputStream(is), m_encoding(encoding)
{

}

size_t GTextInputStream::read(void* pData, size_t _size)
{
	
}

const GInputStream& GTextInputStream::operator>>(unsigned char& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(char& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(unsigned short& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(short& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(unsigned int& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(int& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(unsigned __int64& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(__int64& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(float& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(double& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(GString& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(std::string& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(std::wstring& val)
{
	
}

const GInputStream& GTextInputStream::operator>>(GDataArray& val)
{
	
}

GTextOutputStream::GTextOutputStream(GOutputStreamPtr out, Encoding encoding /*= Encoding::EncodingUtf8*/)
	:m_pOutputStream(out), m_encoding(encoding)
{

}

void GTextOutputStream::write(const void* pData, size_t _size)
{
	
}

void GTextOutputStream::flush()
{
	
}

const GOutputStream& GTextOutputStream::operator<<(unsigned char val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(char val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(unsigned short val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(short val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(unsigned int val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(int val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(unsigned __int64 val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(__int64 val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(float val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(double val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(const GString& val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(const std::string& val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(const std::wstring& val)
{
	
}

const GOutputStream& GTextOutputStream::operator<<(const GDataArray& val)
{
	m_pOutputStream->write();
}