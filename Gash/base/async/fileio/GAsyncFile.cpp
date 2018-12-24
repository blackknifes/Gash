#include "GAsyncFile.h"

GAsyncFile::GAsyncFile(const GString& file)
	:m_strPath(file)
{

}

GAsyncFilePtr GAsyncFile::Create(const GString& path)
{
	return New(path);
}

bool GAsyncFile::open(const CloseCallback& callback /*= nullptr*/)
{
	return open(Operation::FileNew, Mode::ModeReadWrite, ShareNone, callback);
}

bool GAsyncFile::open(Operation op, Mode mode, ShareMode shareMode /*= ShareNone*/, const CloseCallback& callback /*= nullptr*/)
{
	m_hFile = CreateFileW(m_strPath.data(), mode, shareMode, nullptr, op, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, nullptr);
	if (m_hFile == INVALID_HANDLE_VALUE)
		m_hFile = nullptr;
	else if (callback)
			callback(this);
	return m_hFile != NULL;
}

void GAsyncFile::close(const CloseCallback& callback /*= nullptr*/)
{
	if (m_hFile)
	{
		::CloseHandle(m_hFile);
		m_hFile = nullptr;
	}
	if (callback)
		callback(this);
}

bool GAsyncFile::isClosed()
{
	return m_hFile == nullptr;
}

bool GAsyncFile::read(size_t size, const ReadCallback& callback /*= nullptr*/)
{
	GAsyncIO* pIO = GNewIO();
	pIO->dwExpectSize = size;
	pIO->ioType = GAsyncIO::IORead;
	pIO->readCallback = callback;
	return ReadFile(m_hFile, pIO->buffer, size, 0, &pIO->overlapped) != FALSE || GetLastError() == ERROR_IO_PENDING;
}

bool GAsyncFile::write(const void* pData, size_t size, const WriteCallback& callback /*= nullptr*/)
{
	GAsyncIO* pIO = GNewIO();
	pIO->dwExpectSize = size;
	pIO->ioType = GAsyncIO::IOWrite;
	pIO->writeCallback = callback;
	return WriteFile(m_hFile, pData, size, 0, &pIO->overlapped) != FALSE || GetLastError() == ERROR_IO_PENDING;
}

void* GAsyncFile::getNativeHandle() const
{
	return (void*)m_hFile;
}

size_t GAsyncFile::onRead(const void* pData, size_t size)
{
	return size;
}

size_t GAsyncFile::onWrite(size_t size)
{
	return size;
}

void GAsyncFile::onEnd()
{
	
}
