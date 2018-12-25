#include "GFileDevice.h"
#include "GAsyncIO.h"

GFileDevicePtr GFileDevice::Create(const GString& path, Operations op /*= FileCreateOnNotExist*/, Mode mode /*= ModeReadWrite*/, ShareModes share /*= ShareNone*/)
{
	return GFileDevice::CreatePooled(path, op, mode, share);
}

GFileDevice::GFileDevice(const GString& path, Operations op /*= CreateOnNotExist*/, Mode mode /*= ModeReadWrite*/, ShareModes share /*= ShareNone*/)
	:m_hFile(nullptr), m_path(path), m_operations(op), m_mode(mode), m_shareMode(share)
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

bool GFileDevice::open()
{
	return open(FILE_ATTRIBUTE_NORMAL);
}

bool GFileDevice::open(DWORD attributes)
{
	if (m_operations == FileCreateOnNotExist && isExists())
		m_hFile = CreateFileW(m_path.data(), m_mode, m_shareMode, nullptr, OPEN_EXISTING, attributes | FILE_FLAG_OVERLAPPED, nullptr);
	else
		m_hFile = CreateFileW(m_path.data(), m_mode, m_shareMode, nullptr, m_operations, attributes | FILE_FLAG_OVERLAPPED, nullptr);
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

void GFileDevice::flush()
{
	if (FlushFileBuffers(m_hFile) == FALSE)
		setErrorCode(GetLastError());
}

void* GFileDevice::getNativeHandle() const
{
	return (void*)m_hFile;
}

size_t GFileDevice::getSize() const
{
	if (isClosed())
		return 0;
	LARGE_INTEGER len;
	if (GetFileSizeEx(m_hFile, &len) == FALSE)
		return 0;
	return len.QuadPart;
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

GFileDevice::Stats GFileDevice::getStats() const
{
	return Stats();
}

size_t GFileDevice::writeSync(const void* pData, size_t size)
{
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	if (WriteFile(m_hFile, pData, size, nullptr, &overlapped) == FALSE && WSAGetLastError() != ERROR_IO_PENDING)
		return -1;

	DWORD writeSize;
	if (GetOverlappedResult(m_hFile, &overlapped, &writeSize, TRUE) == FALSE)
		return -1;
	return writeSize;
}

size_t GFileDevice::readSync(void* pData, size_t size)
{
	DWORD writeSize = 0;
	if (ReadFile(m_hFile, pData, size, &writeSize, nullptr) == FALSE)
		return -1;
	return writeSize;
}

bool GFileDevice::write(const void* pData, size_t size, const WriteCallback& callback)
{
	GAsyncIOPtr pIO = GAsyncIO::CreateIO(this, GAsyncIO::IO_Write, 
		std::bind(&GFileDevice::OnWrite, this, callback, std::placeholders::_1));

	pIO->AddRef();
	return WriteFile(m_hFile, pData, size, nullptr, pIO->getOverlapped()) == TRUE || WSAGetLastError() == ERROR_IO_PENDING;
}

bool GFileDevice::read(size_t size, const ReadCallback& callback)
{
	GAsyncIOPtr pIO = GAsyncIO::CreateIO(this, GAsyncIO::IO_Read,
		std::bind(&GFileDevice::OnRead, this, callback, std::placeholders::_1));
	pIO->allocBuffer(size);
	pIO->AddRef();
	return ReadFile(m_hFile, pIO->getBufferData(), size, nullptr, pIO->getOverlapped()) == TRUE || WSAGetLastError() == ERROR_IO_PENDING;
}

GDataArray GFileDevice::readAllSync()
{
	if (m_hFile == nullptr)
		return GDataArray();
	DWORD len;
	GetFileSize(m_hFile, &len);
	GDataArray dataArray;
	dataArray.resize(len);
	readSync(dataArray.data(), dataArray.size());
	return std::move(dataArray);
}

void GFileDevice::OnRead(const ReadCallback& callback, GAsyncIOPtr pIO)
{
	if(callback)
		callback(pIO->getBufferData(), pIO->getSize());
}

void GFileDevice::OnWrite(const WriteCallback& callback, GAsyncIOPtr pIO)
{
	if (callback)
		callback(pIO->getSize());
}
