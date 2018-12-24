#include "GEncodingTransform.h"
#include "../GStringPiece.h"

static void StreamMulti2WideChars(GOutputStreamPtr pOut, const void* pData, size_t size, DWORD codePage)
{
	int _size = MultiByteToWideChar(codePage, 0, (const char*)pData, size, 0, 0);
	if (_size)
	{
		wchar_t buf[4096];
		buf[_size] = 0;
		MultiByteToWideChar(codePage, 0, (const char*)pData, size, buf, _size);
		pOut->write(buf, _size << 1);
	}
	else
	{
		wchar_t* buf = (wchar_t*)malloc(_size << 1);
		buf[_size] = 0;
		MultiByteToWideChar(codePage, 0, (const char*)pData, size, buf, _size);
		pOut->write(buf, _size << 1);
		free(buf);
	}
}

static void StreamWideChars2Multi(GOutputStreamPtr pOut, const void* pData, size_t size, DWORD codePage)
{
	size = size >> 1;
	int _size = WideCharToMultiByte(codePage, 0, (const wchar_t*)pData, size, 0, 0, 0, 0);
	if (_size)
	{
		char buf[4096];
		buf[_size] = 0;
		WideCharToMultiByte(codePage, 0, (const wchar_t*)pData, size, buf, _size, 0, 0);
		pOut->write(buf, _size);
	}
	else
	{
		char* buf = (char*)malloc(_size);
		buf[_size] = 0;
		WideCharToMultiByte(codePage, 0, (const wchar_t*)pData, size, buf, _size, 0, 0);
		pOut->write(buf, _size);
		free(buf);
	}
}

static void StreamMulti2Multi(GOutputStreamPtr pOut, const void* pData, size_t size, DWORD from, DWORD to)
{
	int bufsize = MultiByteToWideChar(from, 0, (const char*)pData, size, 0, 0);
	if (bufsize < 4096)
	{
		wchar_t buf[4096];
		MultiByteToWideChar(from, 0, (const char*)pData, size, buf, 4096);
		StreamWideChars2Multi(pOut, buf, bufsize, to);
	}
	else
	{
		wchar_t* pBuf = (wchar_t*)malloc(bufsize << 1);
		MultiByteToWideChar(from, 0, (const char*)pData, size, pBuf, 4096);
		StreamWideChars2Multi(pOut, pBuf, bufsize, to);
		free(pBuf);
	}
}

GEncodingTransform::GEncodingTransform(GOutputStreamPtr pOut, Encoding from, Encoding to)
	:m_pOutputStream(pOut), m_from(from), m_to(to)
{

}

void GEncodingTransform::write(const void* pData, size_t _size)
{

}

void GEncodingTransform::flush()
{
	
}
