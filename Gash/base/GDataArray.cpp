#include "GDataArray.h"
#include <memory>
#include <Windows.h>

static unsigned char* ConvertEncoding(
	const wchar_t* str, 
	DWORD codePage,
	size_t size, 
	size_t* _size, 
	size_t* _capacity)
{
	if (codePage == -1)
	{
		size_t datasize = wcslen(str) << 1;
		unsigned char* pData = (unsigned char*)malloc(datasize);
		*_capacity = *_size = datasize;
		return pData;
	}
	unsigned char* pData = nullptr;
	*_capacity = *_size = WideCharToMultiByte(codePage, 0, str, size, 0, 0, 0, 0);
	pData = (unsigned char*)malloc(*_size);
	WideCharToMultiByte(codePage, 0, str, size, (char*)pData, *_size, 0, 0);
	return pData;
}

static GString EncodingString(const void* pData, size_t size, DWORD codePage)
{
	GString str;
	int bufsize = MultiByteToWideChar(codePage, 0, (char*)pData, size, nullptr, 0);
	if (bufsize <= 0)
		return GString();
	str.resize(bufsize);
	MultiByteToWideChar(codePage, 0, (char*)pData, size, (wchar_t*)str.data(), str.size());
	return std::move(str);
}

GDataArray::GDataArray()
	:m_pData(nullptr), m_size(0), m_capacity(0)
{

}

GDataArray::GDataArray(size_t _size)
	: m_pData((byte_t*)malloc(_size)), m_size(0), m_capacity(_size)
{
}

GDataArray::GDataArray(const GDataArray& dataArray)
	: m_pData(nullptr), m_size(0), m_capacity(0)
{
	if (dataArray.m_pData != nullptr)
	{
		m_pData = (byte_t*)malloc(dataArray.size());
		m_capacity = m_size = dataArray.m_size;
		memcpy_s(m_pData, m_size, dataArray.m_pData, m_size);
	}
}

GDataArray::GDataArray(GDataArray&& dataArray)
	: m_pData(dataArray.m_pData), m_size(dataArray.m_size), m_capacity(dataArray.m_capacity)
{
	dataArray.m_pData = nullptr;
	dataArray.m_size = dataArray.m_capacity = 0;
}

GDataArray::GDataArray(const void* pData, size_t _size)
{
	m_pData = (byte_t*)malloc(_size);
	memcpy_s(m_pData, _size, pData, _size);
	m_capacity = m_size = _size;
}

GDataArray::GDataArray(const wchar_t* str, Encoding encoding /*= EncodingUnicode*/)
	:m_pData(nullptr), m_size(0), m_capacity(0)
{
	DWORD codePage = 0;
	switch (encoding)
	{
	case GDataArray::EncodingAnsi:
		codePage = CP_ACP;
		break;
	case GDataArray::EncodingUnicode:
		codePage = -1;
		break;
	case GDataArray::EncodingUtf8:
		codePage = CP_UTF8;
		break;
	default:
		break;
	}
	m_pData = ConvertEncoding(str, codePage, wcslen(str), &m_size, &m_capacity);
}

GDataArray::~GDataArray()
{
	if (m_pData)
		free(m_pData);
}

GDataArray::byte_t* GDataArray::data()
{
	return m_pData;
}

const GDataArray::byte_t* GDataArray::data() const
{
	return m_pData;
}

GDataArray::byte_t& GDataArray::operator[](size_t index)
{
	return m_pData[index];
}

GDataArray::byte_t GDataArray::operator[](size_t index) const
{
	return m_pData[index];
}

GDataArray::byte_t& GDataArray::at(size_t index)
{
	return m_pData[index];
}

GDataArray::byte_t GDataArray::at(size_t index) const
{
	return m_pData[index];
}

GDataArray::byte_t* GDataArray::begin()
{
	return m_pData;
}

const GDataArray::byte_t* GDataArray::begin() const
{
	return m_pData;
}

GDataArray::byte_t* GDataArray::end()
{
	return m_pData + m_size;
}

const GDataArray::byte_t* GDataArray::end() const
{
	return m_pData + m_size;
}

size_t GDataArray::size() const
{
	return m_size;
}

size_t GDataArray::capacity() const
{
	return m_capacity;
}

void GDataArray::reserve(size_t _size)
{
	if (_size <= m_capacity)
		return;
	if (m_pData == nullptr)
		m_pData = (byte_t*)malloc(_size);
	else
		m_pData = (byte_t*)realloc(m_pData, _size);
	m_capacity = _size;
}

void GDataArray::resize(size_t _size)
{
	reserve(_size);
	m_size = _size;
}

void GDataArray::append(const void* pData, size_t bytes)
{
	size_t startIndex = m_size;
	resize(m_size + bytes);
	memcpy_s(m_pData + startIndex, bytes, pData, bytes);
}

void GDataArray::assign(const void* pData, size_t bytes)
{
	resize(bytes);
	memcpy_s(m_pData, bytes, pData, bytes);
}

size_t GDataArray::read(void* pData, size_t bytes, size_t offset) const
{
	if (m_size == 0)
		return 0;
	if (offset >= m_size)
		offset = m_size - 1;
	if (offset + bytes > m_size)
		bytes = m_size - offset;
	memcpy_s(pData, bytes, m_pData + offset, bytes);
	return bytes;
}

void GDataArray::write(const void* pData, size_t bytes, size_t offset)
{
	if (offset > m_size)
		offset = m_size;
	resize(offset + bytes);
	memcpy_s(m_pData + offset, m_size, pData, bytes);
}

GString GDataArray::toString(
	Encoding from /*= EncodingUtf8*/) const
{
	switch (from)
	{
	case GDataArray::EncodingAnsi:
		return EncodingString(m_pData, m_size, CP_ACP);
	case GDataArray::EncodingUnicode:
	{
		GString str;
		str.resize(m_size >> 1);
		memcpy_s((void*)str.data(), m_size, m_pData, m_size);
		return std::move(str);
	}
	case GDataArray::EncodingUtf8:
		return EncodingString(m_pData, m_size, CP_UTF8);
	default:
		break;
	}
	return GString(L"");
}

const GDataArray& GDataArray::operator=(GDataArray&& dataArray)
{
	if (m_pData)
		free(m_pData);
	m_pData = dataArray.m_pData;
	m_size = dataArray.m_size;
	m_capacity = dataArray.m_capacity;
	dataArray.m_pData = nullptr;
	dataArray.m_size = dataArray.m_capacity = 0;
	return *this;
}

const GDataArray& GDataArray::operator=(const GDataArray& dataArray)
{
	resize(dataArray.m_size);
	memcpy_s(m_pData, m_size, dataArray.m_pData, dataArray.m_size);
	return *this;
}
