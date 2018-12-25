// #include "GFileIO.h"
// 
// GFileIO::GFileIO()
// 	:m_hFile(nullptr)
// {
// 
// }
// 
// GFileIO::GFileIO(
// 	const GString& file,
// 	Operations op /*= Open*/,
// 	Mode mode /*= ModeReadWrite*/,
// 	ShareModes shareModes /*= ShareNone*/)
// 	: m_hFile(nullptr), m_asyncFlag(false)
// {
// 	openShare(file, op, mode, shareModes);
// }
// 
// GFileIO::~GFileIO()
// {
// 	close();
// }
// 
// bool GFileIO::open(
// 	const GString& filename,
// 	Operations op, 
// 	Mode mode,
// 	Attributes attributes /*= Normal*/)
// {;
// 	return openShare(filename, op, mode, ShareNone, attributes);
// }
// 
// bool GFileIO::open(bool async /*= false*/)
// {
// 	throw std::logic_error("The method or operation is not implemented.");
// }
// 
// bool GFileIO::openShare(
// 	const GString& filename, 
// 	Operations op, 
// 	Mode mode,
// 	ShareModes shareMode,
// 	Attributes attributes /*= Normal*/)
// {
// 	close();
// 	HANDLE hFile = CreateFileW(
// 		filename.data(),
// 		mode,
// 		shareMode,
// 		nullptr,
// 		op,
// 		attributes,
// 		nullptr);
// 	if (hFile == INVALID_HANDLE_VALUE)
// 	{
// 		hFile = nullptr;
// 		return false;
// 	}
// 
// 	return true;
// }
// 
// GIODevice::IOResult GFileIO::read(void* pData, size_t size, size_t* readsize /*= nullptr*/)
// {
// 	if (ReadFile(m_hFile, pData, (DWORD)size, (DWORD*)readsize, nullptr) == FALSE)
// 		return translateError(GIODevice::IO_Read);
// 	return GIODevice::IO_Success;
// }
// 
// bool GFileIO::isAsync() const
// {
// 	return m_asyncFlag;
// }
// 
// void* GFileIO::getNativeHandle() const
// {
// 	return m_hFile;
// }
// 
// GIODevice::IOResult GFileIO::write(const void* pData, size_t size)
// {
// 	DWORD dwValue = 0;
// 	if (WriteFile(m_hFile, pData, (DWORD)size, &dwValue, nullptr) == FALSE)
// 		return translateError(GIODevice::IO_Write);
// 	return GIODevice::IO_Success;
// }
// 
// void GFileIO::close()
// {
// 	if (m_hFile)
// 	{
// 		::CloseHandle(m_hFile);
// 		m_hFile = nullptr;
// 	}
// }
// 
// bool GFileIO::isClosed() const
// {
// 	return m_hFile == nullptr;
// }
// 
// void GFileIO::flush()
// {
// 	if (m_hFile == nullptr)
// 		return;
// 	FlushFileBuffers(m_hFile);
// }
// 
// GDataArray GFileIO::readAll()
// {
// 	GDataArray dataArray;
// 	dataArray.resize(getSize());
// 	read(dataArray.data(), dataArray.size());
// 	return std::move(dataArray);
// }
// 
// unsigned __int64 GFileIO::getSize() const
// {
// 	if (isClosed())
// 		return 0;
// 	LARGE_INTEGER val;
// 	val.QuadPart = 0;
// 	if (m_hFile)
// 		GetFileSizeEx(m_hFile, &val);
// 	return val.QuadPart;
// }
// 
// GFileIO::Stats GFileIO::getStats() const
// {
// 	Stats stats = {};
// 	BY_HANDLE_FILE_INFORMATION info;
// 	if (GetFileInformationByHandle(m_hFile, &info) != FALSE)
// 	{
// 		stats.attributes = info.dwFileAttributes;
// 		stats.accessTime = *(__int64*)&info.ftLastAccessTime;
// 		stats.createTime = *(__int64*)&info.ftCreationTime;
// 		stats.lastModifiedTime = *(__int64*)&info.ftLastWriteTime;
// 		stats.volumeSerialNumber = info.dwVolumeSerialNumber;
// 		stats.size = ((unsigned __int64)info.nFileSizeLow) | ((unsigned __int64)info.nFileIndexHigh << 32);
// 		stats.numOfLinks = info.nNumberOfLinks;
// 		stats.fileIndex = ((unsigned __int64)info.nFileIndexLow) | ((unsigned __int64)info.nFileIndexHigh << 32);
// 	}
// 	return stats;
// }