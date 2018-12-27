#include "GFileSync.h"



GFileSync::GFileSync()
	:m_hFile(nullptr)
{

}

GFileSync::~GFileSync()
{
	close();
}

bool GFileSync::open(const GString& filePath, GOperation op, GFileMode mode /*= ModeReadWrite*/, GFileAttributes attrs /*= GAttribute::Normal*/, GFileShareMode shareMode /*= ShareNone*/)
{
	close();
	m_hFile = CreateFileW(
		filePath.data(),
		mode,
		shareMode,
		nullptr,
		op,
		attrs &~(FILE_FLAG_OVERLAPPED),
		nullptr
	);

	if (m_hFile == INVALID_HANDLE_VALUE)
		m_hFile = nullptr;
	return m_hFile != nullptr;
}

void GFileSync::close()
{
	if (!isClosed())
		::CloseHandle(m_hFile);
}

bool GFileSync::isClosed() const
{
	return m_hFile == nullptr;
}

void GFileSync::flush()
{
	FlushFileBuffers(m_hFile);
}

void* GFileSync::getNativeHandle() const
{
	return m_hFile;
}

size_t GFileSync::getSize() const
{
	LARGE_INTEGER len = {};
	GetFileSizeEx(m_hFile, &len);
	return len.QuadPart;
}

GFileStats GFileSync::getStats() const
{
	return GFileStats();
}

int GFileSync::read(void* pData, size_t size)
{
	DWORD readSize = 0;
	if(ReadFile(m_hFile, pData, (DWORD)size, &readSize, nullptr) == FALSE)
		return -1;
	return readSize;
}

int GFileSync::write(const void* pData, size_t size)
{
	DWORD writeSize = 0;
	if (WriteFile(m_hFile, pData, (DWORD)size, &writeSize, nullptr) == FALSE)
		return -1;
	return writeSize;
}
