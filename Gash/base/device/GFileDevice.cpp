#include "GFileDevice.h"
#include "../async/fileio/GAsyncIO.h"



GFileDevice::GFileDevice(const GString& path, Operations op /*= CreateOnNotExist*/, Mode mode /*= ModeReadWrite*/, ShareMode share /*= ShareNone*/, Attributes attributes /*= Normal*/)
	:m_hFile(nullptr), m_path(path), m_operations(op), m_mode(mode), m_shareMode(share), m_attributes(attributes)
{

}

GFileDevice::~GFileDevice()
{
	close();
}

bool GFileDevice::isExists() const
{
	return GetFileAttributesW(m_path.data()) != INVALID_FILE_ATTRIBUTES;
}

bool GFileDevice::open(bool async /*= false*/)
{
	if (async)
		m_attributes |= FILE_FLAG_OVERLAPPED;

	if (m_operations == CreateOnNotExist && isExists())
		m_hFile = CreateFileW(m_path.data(), m_mode, m_shareMode, nullptr, OPEN_EXISTING, m_attributes, nullptr);
	else
		m_hFile = CreateFileW(m_path.data(), m_mode, m_shareMode, nullptr, m_operations, m_attributes, nullptr);
	if (m_hFile == INVALID_HANDLE_VALUE)
		m_hFile = nullptr;
	return m_hFile != nullptr;
}

void GFileDevice::close()
{
	if (m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = nullptr;
	}
}

bool GFileDevice::isClosed() const
{
	return m_hFile == nullptr;
}

GDevice::IOResult GFileDevice::write(const void* pData, size_t size)
{
	if (isClosed())
		return GDevice::IO_NotOpen;
	DWORD unused;
	if (isAsync())
	{
		GAsyncIO* pIO = GNewIO();
		pIO->ioType = GAsyncIO::IOWrite;
		if (WriteFile(m_hFile, pData, size, &unused, &pIO->overlapped) == FALSE)
		{
			DWORD errCode;
			switch (errCode = GetLastError())
			{
			case ERROR_IO_PENDING:
				return GDevice::IO_Write_Pending;
			default:
				break;
			}
			setErrorCode(errCode);
			return GDevice::IO_Internal_Error;
		}
		return GDevice::IO_Success;
	}

	if (WriteFile(m_hFile, pData, size, &unused, nullptr) == FALSE)
	{
		setErrorCode(GetLastError());
		return GDevice::IO_Internal_Error;
	}

	return GDevice::IO_Success;
}

GDevice::IOResult GFileDevice::read(void* pData, size_t size, size_t* readsize /*= nullptr*/)
{
	if (isClosed())
		return GDevice::IO_NotOpen;
	DWORD dwReadSize;
	if (isAsync())
	{
		GAsyncIO* pIO = GNewIO();
		pIO->ioType = GAsyncIO::IOWrite;
		if (ReadFile(m_hFile, pData, size, &dwReadSize, &pIO->overlapped) == FALSE)
		{
			DWORD errCode;
			switch (errCode = GetLastError())
			{
			case ERROR_IO_PENDING:
				return GDevice::IO_Write_Pending;
			default:
				break;
			}
			setErrorCode(errCode);
			return GDevice::IO_Internal_Error;
		}
		if (readsize)
			*readsize = dwReadSize;
		return GDevice::IO_Success;
	}

	if (ReadFile(m_hFile, pData, size, &dwReadSize, nullptr) == FALSE)
	{
		setErrorCode(GetLastError());
		return GDevice::IO_Internal_Error;
	}

	if (readsize)
		*readsize = dwReadSize;

	return GDevice::IO_Success;
}

void GFileDevice::flush()
{
	if (FlushFileBuffers(m_hFile) == FALSE)
		setErrorCode(GetLastError());
}

bool GFileDevice::isAsync() const
{
	if (isClosed())
		return false;
	return (m_attributes & FILE_FLAG_OVERLAPPED) != 0;
}

void* GFileDevice::getNativeHandle() const
{
	return (void*)m_hFile;
}

GFile GFileDevice::getFile() const
{
	return GFile(m_path);
}

const GString& GFileDevice::getPath() const
{
	return m_path;
}

bool GFileDevice::canRead() const
{
	return (m_mode & ModeRead) != 0;
}

bool GFileDevice::canWrite() const
{
	return (m_mode & ModeWrite) != 0;
}
