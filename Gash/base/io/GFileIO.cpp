#include "GFileIO.h"

static DWORD GetFileMoveMethod(GIODevice::SeekType seekType)
{
	switch (seekType)
	{
	case GIODevice::SeekCur:
		return FILE_CURRENT;
	case GIODevice::SeekBegin:
		return FILE_BEGIN;
	case GIODevice::SeekEnd:
		return FILE_END;
	default:
		break;
	}
	return -1;
}

GFileIO::GFileIO()
	:m_hReadFile(nullptr), m_hWriteFile(nullptr), m_dwErrorCode(0)
{

}

GFileIO::GFileIO(
	const GString& file,
	Operations op /*= Open*/,
	Mode mode /*= ModeReadWrite*/,
	ShareModes shareModes /*= ShareNone*/)
	: m_hReadFile(nullptr), m_hWriteFile(nullptr), m_dwErrorCode(0)
{
	openShare(file, op, mode, shareModes);
}

GFileIO::~GFileIO()
{
	close();
}

bool GFileIO::open(
	const GString& filename,
	Operations op, 
	Mode mode,
	Attributes attributes /*= Normal*/)
{;
	return openShare(filename, op, mode, ShareNone, attributes);
}

bool GFileIO::openShare(
	const GString& filename, 
	Operations op, 
	Mode mode,
	ShareModes shareMode,
	Attributes attributes /*= Normal*/)
{
	close();
	HANDLE hReadFile = nullptr;
	HANDLE hWriteFile = nullptr;
	if ((mode & ModeRead) != 0)
	{
		hReadFile = CreateFileW(
			filename.data(),
			ModeRead,
			shareMode,
			nullptr,
			op,
			attributes,
			nullptr);
		if (hReadFile == INVALID_HANDLE_VALUE)
		{
			m_dwErrorCode = GetLastError();
			return false;
		}
	}

	if ((mode & ModeWrite) != 0)
	{
		hWriteFile = CreateFileW(
			filename.data(),
			ModeWrite,
			shareMode,
			nullptr,
			op,
			attributes,
			nullptr);
		if (hWriteFile == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hReadFile);
			m_dwErrorCode = GetLastError();
			return false;
		}
	}

	m_hWriteFile = hWriteFile;
	m_hReadFile = hReadFile;	
	return true;
}

size_t GFileIO::read(void* pData, size_t bytes)
{
	DWORD readSize = (DWORD)bytes;
	if (ReadFile(m_hReadFile, pData, readSize, &readSize, nullptr) == FALSE)
		return 0;
	return readSize;
}

size_t GFileIO::write(const void* pData, size_t bytes)
{
	DWORD dwValue = 0;
	if (WriteFile(m_hWriteFile, pData, (DWORD)bytes, &dwValue, nullptr) == FALSE)
		return 0;
	return dwValue;
}

GDataArray GFileIO::readAll()
{
	if (isClosed())
		return GDataArray();
	size_t readSize = getSize();
	GDataArray buffer(readSize);
	read(buffer.data(), readSize);
	return std::move(buffer);
}

__int64 GFileIO::seekRead(__int64 offset, SeekType pos /*= SeekCur*/)
{
	if (m_hReadFile == nullptr)
		return 0;
	LARGE_INTEGER largeOffset;
	largeOffset.QuadPart = offset;
	if (SetFilePointerEx(m_hReadFile, largeOffset, &largeOffset, GetFileMoveMethod(pos)) == FALSE)
	{
		m_dwErrorCode = GetLastError();
		return 0;
	}
	return largeOffset.QuadPart;
}

__int64 GFileIO::seekWrite(__int64 offset, SeekType pos /*= SeekCur*/)
{
	if (m_hWriteFile == nullptr)
		return 0;
	LARGE_INTEGER largeOffset;
	largeOffset.QuadPart = offset;
	if (SetFilePointerEx(m_hWriteFile, largeOffset, &largeOffset, GetFileMoveMethod(pos)) == FALSE)
	{
		m_dwErrorCode = GetLastError();
		return 0;
	}
	return largeOffset.QuadPart;
}

void GFileIO::resetRead()
{
	seekRead(0, SeekBegin);
}

void GFileIO::resetWrite()
{
	seekWrite(0, SeekBegin);
}

void GFileIO::reset()
{
	resetRead();
	resetWrite();
}

void GFileIO::close()
{
	if (m_hReadFile)
	{
		::CloseHandle(m_hReadFile);
		m_hReadFile = nullptr;
	}
	if (m_hWriteFile)
	{
		::CloseHandle(m_hWriteFile);
		m_hWriteFile = nullptr;
	}

	m_dwErrorCode = 0;
}

bool GFileIO::isClosed() const
{
	return m_hReadFile == nullptr || m_hWriteFile == nullptr;
}

void GFileIO::flush()
{
	if (m_hWriteFile == nullptr)
		return;
	FlushFileBuffers(m_hWriteFile);
}

int GFileIO::getErrorCode() const
{
	return m_dwErrorCode;
}

GString GFileIO::getErrorMessage() const
{
	return GGetErrorMessage(m_dwErrorCode);
}

unsigned __int64 GFileIO::getSize() const
{
	if (isClosed())
		return 0;
	LARGE_INTEGER val;
	val.QuadPart = 0;
	if (m_hReadFile)
		GetFileSizeEx(m_hReadFile, &val);
	else if (m_hWriteFile)
		GetFileSizeEx(m_hWriteFile, &val);
	return val.QuadPart;
}

GFileIO::Stats GFileIO::getStats() const
{
	Stats stats = {};
	BY_HANDLE_FILE_INFORMATION info;
	HANDLE hFile = m_hReadFile;
	if (hFile == nullptr)
		hFile = m_hWriteFile;
	if (GetFileInformationByHandle(hFile, &info) != FALSE)
	{
		stats.attributes = info.dwFileAttributes;
		stats.accessTime = *(__int64*)&info.ftLastAccessTime;
		stats.createTime = *(__int64*)&info.ftCreationTime;
		stats.lastModifiedTime = *(__int64*)&info.ftLastWriteTime;
		stats.volumeSerialNumber = info.dwVolumeSerialNumber;
		stats.size = ((unsigned __int64)info.nFileSizeLow) | ((unsigned __int64)info.nFileIndexHigh << 32);
		stats.numOfLinks = info.nNumberOfLinks;
		stats.fileIndex = ((unsigned __int64)info.nFileIndexLow) | ((unsigned __int64)info.nFileIndexHigh << 32);
	}
	return stats;
}

__int64 GFileIO::tellRead() const
{
	LARGE_INTEGER val;
	val.QuadPart = 0;
	SetFilePointerEx(m_hReadFile, val, &val, FILE_CURRENT);
	return val.QuadPart;
}

__int64 GFileIO::tellWrite() const
{
	LARGE_INTEGER val;
	val.QuadPart = 0;
	SetFilePointerEx(m_hWriteFile, val, &val, FILE_CURRENT);
	return val.QuadPart;
}
