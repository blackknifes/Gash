#include "GStream.h"
#include "GBufferStream.h"

void GInputStream::pipe(GOutputStreamPtr pOutputStream)
{
	read(pOutputStream);
}

GDataArray GInputStream::read(size_t _size)
{
	GDataArray dataArray(_size);
	_size = read(dataArray.data(), _size);
	dataArray.resize(_size);
	return std::move(dataArray);
}

void GInputStream::read(GOutputStreamPtr pOutputStream)
{
	unsigned char buf[4096] = {};
	size_t readSize;
	while ((readSize = read(buf, sizeof(buf))) != 0)
		pOutputStream->write(buf, readSize);
	pOutputStream->end();
}

GDataArray GInputStream::readAll()
{
	GBufferStream bufferStream;
	unsigned char buf[4096] = {};
	size_t readSize;
	while ((readSize = read(buf, sizeof(buf))) != 0)
		bufferStream.write(buf, readSize);
	return bufferStream.readAll();
}

void GOutputStream::write(const GDataArray& dataArray)
{
	write(dataArray.data(), dataArray.size());
}

const GOutputStream& GOutputStream::operator<<(unsigned char val)
{
	write(&val, sizeof(unsigned char));
	return *this;
}

const GOutputStream& GOutputStream::operator<<(char val)
{
	write(&val, sizeof(char));
	return *this;
}

const GOutputStream& GOutputStream::operator<<(unsigned short val)
{
	write(&val, sizeof(unsigned short));
	return *this;
}

const GOutputStream& GOutputStream::operator<<(short val)
{
	write(&val, sizeof(short));
	return *this;
}

const GOutputStream& GOutputStream::operator<<(unsigned int val)
{
	write(&val, sizeof(unsigned int));
	return *this;
}

const GOutputStream& GOutputStream::operator<<(int val)
{
	write(&val, sizeof(int));
	return *this;
}

const GOutputStream& GOutputStream::operator<<(unsigned __int64 val)
{
	write(&val, sizeof(unsigned __int64));
	return *this;
}

const GOutputStream& GOutputStream::operator<<(__int64 val)
{
	write(&val, sizeof(__int64));
	return *this;
}

const GOutputStream& GOutputStream::operator<<(float val)
{
	write(&val, sizeof(float));
	return *this;
}

const GOutputStream& GOutputStream::operator<<(double val)
{
	write(&val, sizeof(double));
	return *this;
}

const GOutputStream& GOutputStream::operator<<(const GString& val)
{
	std::string str = val.toUtf8();
	write(str.data(), str.size());
	return *this;
}

const GOutputStream& GOutputStream::operator<<(const std::string& val)
{
	write(val.data(), val.size());
	return *this;
}

const GOutputStream& GOutputStream::operator<<(const std::wstring& val)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, val.data(), val.size(), 0, 0, 0, 0);
	if (size <= 4096)
	{
		char buf[4096];
		WideCharToMultiByte(CP_UTF8, 0, val.data(), val.size(), buf, sizeof(buf), 0, 0);
		write(buf, size);
	}
	else
	{
		char* pBuf = (char*)malloc(size);
		WideCharToMultiByte(CP_UTF8, 0, val.data(), val.size(), pBuf, size, 0, 0);
		write(pBuf, size);
		free(pBuf);
	}
	return *this;
}

const GOutputStream& GOutputStream::operator<<(const GDataArray& val)
{
	write(val.data(), val.size());
	return *this;
}

const GOutputStream& GOutputStream::operator<<(const char* val)
{
	write(val, strlen(val));
	return *this;
}

const GOutputStream& GOutputStream::operator<<(const wchar_t* val)
{
	write(val, wcslen(val) << 1);
	return *this;
}

const GInputStream& GInputStream::operator>>(unsigned char& val)
{
	read(&val, sizeof(unsigned char));
	return *this;
}

const GInputStream& GInputStream::operator>>(char& val)
{
	read(&val, sizeof(char));
	return *this;
}

const GInputStream& GInputStream::operator>>(unsigned short& val)
{
	read(&val, sizeof(unsigned short));
	return *this;
}

const GInputStream& GInputStream::operator>>(short& val)
{
	read(&val, sizeof(short));
	return *this;
}

const GInputStream& GInputStream::operator>>(unsigned int& val)
{
	read(&val, sizeof(unsigned int));
	return *this;
}

const GInputStream& GInputStream::operator>>(int& val)
{
	read(&val, sizeof(int));
	return *this;
}

const GInputStream& GInputStream::operator>>(unsigned __int64& val)
{
	read(&val, sizeof(unsigned __int64));
	return *this;
}

const GInputStream& GInputStream::operator>>(__int64& val)
{
	read(&val, sizeof(__int64));
	return *this;
}

const GInputStream& GInputStream::operator>>(float& val)
{
	read(&val, sizeof(float));
	return *this;
}

const GInputStream& GInputStream::operator>>(double& val)
{
	read(&val, sizeof(double));
	return *this;
}

const GInputStream& GInputStream::operator>>(GString& val)
{
	val = readAll().toString();
	return *this;
}

const GInputStream& GInputStream::operator>>(std::string& val)
{
	val = readAll().toString().toAnsi();
	return *this;
}

const GInputStream& GInputStream::operator>>(std::wstring& val)
{
	GDataArray dataArray = readAll();
	val.resize(dataArray.size() >> 1);
	memcpy_s((void*)val.data(), dataArray.size(), dataArray.data(), dataArray.size());
	return *this;
}

const GInputStream& GInputStream::operator>>(GDataArray& val)
{
	val = readAll();
	return *this;
}
