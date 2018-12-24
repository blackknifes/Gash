#include "GDataArray.h"

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

static std::string ConvertToMulti(const void* pData, size_t size, GEncoding from, GEncoding to)
{
	std::string str;
	str.reserve(size);

	Encoder* pEncoder = FindEncoder(from, to);
	const void* ptr = pData;
	char buf[4096];
	size_t bufsize = sizeof(buf);
	void* pOut = buf;
	while (size != 0 && !EncodeIsCritical(EncodeString(pEncoder, &ptr, &size, &pOut, &bufsize)))
	{
		size_t strsize = sizeof(buf) - bufsize;
		str.append(buf, strsize);
		pOut = buf;
		bufsize = sizeof(buf);
	}
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

GDataArray::GDataArray(const wchar_t* str)
	:m_pData(nullptr), m_size(0), m_capacity(0)
{
	size_t len = (wcslen(str) + 1) << 1;
	resize(len);
	memcpy_s(m_pData, len, str, len);
}


GDataArray::GDataArray(const wchar_t* str, size_t len)
	:m_pData(nullptr), m_size(0), m_capacity(0)
{
	len = (len + 1) << 1;
	resize(len);
	memcpy_s(m_pData, len, str, len);
	((wchar_t*)m_pData)[(len >> 1)] = 0;
}

GDataArray::GDataArray(const char* str, GEncoding encoding /*= GEncoding::EncodingAnsi*/)
	:m_pData(nullptr), m_size(0), m_capacity(0)
{
	size_t len = strlen(str) + 1;
	resize(len);
	memcpy_s(m_pData, len, str, len);
	*this = encode(encoding, GEncoding::EncodingUtf16LE);
}

GDataArray::GDataArray(const char* str, size_t len, GEncoding encoding /*= GEncoding::EncodingAnsi*/)
	: m_pData(nullptr), m_size(0), m_capacity(0)
{
	resize(len);
	memcpy_s(m_pData, len, str, len);
	*this = encode(encoding, GEncoding::EncodingUtf16LE);
}

GDataArray::~GDataArray()
{
	if (m_pData)
		free(m_pData);
}

bool GDataArray::operator==(const GDataArray& dataArray) const
{
	if (this->size() != dataArray.size())
		return false;
	for (size_t i = 0; i < this->size(); ++i)
	{
		if (m_pData[i] != dataArray[i])
			return false;
	}
	return true;
}

bool GDataArray::operator!=(const GDataArray& dataArray) const
{
	return !(operator==(dataArray));
}

bool GDataArray::operator>(const GDataArray& dataArray) const
{
	if (this->size() != dataArray.size())
		return this->size() > dataArray.size();
	for (size_t i = 0; i < this->size(); ++i)
	{
		if (m_pData[i] != dataArray[i])
			return m_pData[i] > dataArray[i];
	}
	return false;
}


bool GDataArray::operator>=(const GDataArray& dataArray) const
{
	if (this->size() != dataArray.size())
		return this->size() > dataArray.size();
	for (size_t i = 0; i < this->size(); ++i)
	{
		if (m_pData[i] != dataArray[i])
			return m_pData[i] > dataArray[i];
	}
	return true;
}


bool GDataArray::operator<(const GDataArray& dataArray) const
{
	return !(operator>=(dataArray));
}


bool GDataArray::operator<=(const GDataArray& dataArray) const
{
	return !(operator>(dataArray));
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

void GDataArray::insert(const void* pData, size_t bytes, size_t offset)
{
	if (bytes == 0)
		return;
	if (offset >= m_size)
		offset = m_size - 1;

	size_t newSize = this->size() + bytes;
	reserve(newSize);
	for (size_t i = this->size(); i > offset; --i)
		m_pData[i + bytes - 1] = m_pData[i - 1];
	memcpy_s(m_pData + offset, bytes, pData, bytes);
	m_size = newSize;
}

void GDataArray::erase(size_t offset, size_t count /*= 1*/)
{
	if (count == 0 || this->empty())
		return;
	if (offset >= m_size)
		offset = m_size - 1;
	if (count + offset > m_size)
		count = m_size - offset;
	for (size_t i = offset; i < m_size; ++i)
		m_pData[i] = m_pData[i + count];
	m_size -= count;
}

void GDataArray::replace(const void* replacement, size_t bytes, size_t offset, size_t count /*= 1*/)
{
	if (empty() || bytes == 0)
		return;
	if (offset >= m_size)
		offset = m_size - 1;
	if (count + offset > m_size)
		count = m_size - offset;

	size_t opsize = count > bytes ? count - bytes : bytes - count;
	if (bytes > count)
		insert(replacement, opsize, offset);
	else
		erase(offset, opsize);
	memcpy_s(m_pData + offset, bytes, replacement, bytes);
}

GDataArray GDataArray::sub(size_t offset, size_t count) const
{
	if (empty())
		return GDataArray();
	if (offset >= m_size)
		offset = m_size - 1;
	if (count + offset > m_size)
		count = m_size - offset;
	GDataArray dataArray(count);
	dataArray.m_size = read(dataArray.data(), count, offset);
	return std::move(dataArray);
}

GDataArray GDataArray::slice(size_t _start, size_t _end) const
{
	return sub(_start, _end - _start);
}

bool GDataArray::empty() const
{
	return m_size == 0;
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

size_t GDataArray::hashCode() const
{
	return std::_Hash_bytes(m_pData, m_size);
}

GDataArray GDataArray::encode(GEncoding from, GEncoding to) const
{
	GDataArray dataArray;
	dataArray.reserve(m_size);
	char buf[4096] = {};

	const void* pSrc = m_pData;
	size_t srcSize = m_size;
	size_t bufsize = 4096;
	void* out = buf;
	while (srcSize == 0 || !EncodeIsCritical(EncodeString(from, to, &pSrc, &srcSize, &out, &bufsize)))
	{
		dataArray.append(buf, 4096 - bufsize);
		bufsize = 4096;
		out = buf;
	}

	return std::move(dataArray);
}

GDataArray GDataArray::encode(GEncoding to) const
{
	return encode(EncodingUtf16LE, to);
}

GString GDataArray::toString(GEncoding encoding /*= EncodingUtf8*/) const
{
	GString str;
	str.reserve(m_size);

	Encoder* pEncoder = FindEncoder(encoding, EncodingUtf16LE);
	const void* ptr = m_pData;
	size_t insize = m_size;
	wchar_t buf[2048];
	size_t bufsize = sizeof(buf);
	void* pOut = buf;
	while (insize != 0 && !EncodeIsCritical(EncodeString(pEncoder, &ptr, &insize, &pOut, &bufsize)))
	{
		size_t strsize = sizeof(buf) - bufsize;
		str.append(buf, strsize);
		bufsize = sizeof(buf);
		pOut = buf;
	}
	return std::move(str);
}

std::string GDataArray::toAnsi(GEncoding encoding /*= EncodingUtf8*/) const
{
	return ConvertToMulti(m_pData, m_size, encoding, EncodingAnsi);
}

std::string GDataArray::toUtf8(GEncoding encoding) const
{
	return ConvertToMulti(m_pData, m_size, encoding, EncodingUtf8);
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
