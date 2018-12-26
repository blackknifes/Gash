#include "GFile.h"

static bool IsSeperator(wchar_t ch)
{
	return ch == '/' || ch == '\\';
}

static bool ComparePathChar(wchar_t ch1, wchar_t ch2)
{
	if (::tolower(ch1) != ::tolower(ch2))
	{
		if (!IsSeperator(ch1) || !IsSeperator(ch2))
			return false;
	}
	return true;
}

GFile GFile::GetDir(DirType type)
{
	return GFile(GShell::GetPath(type));
}

GDataArray GFile::ReadFile(const GString& path)
{
	GFileDevice fileIO(path, GOperation::FileOpen, GFileMode::ModeRead, GFileShareMode::ShareWrite);
	if (!fileIO.open())
		return GDataArray();
	return fileIO.readAllSync();
}

bool GFile::WriteFile(const GString& path, const GDataArray& buffer)
{
	GFileDevice fileIO(path, GOperation::FileCreate, GFileMode::ModeWrite, GFileShareMode::ShareRead);
	if (!fileIO.open())
		return false;
	return fileIO.writeSync(buffer.data(), buffer.size()) == buffer.size();
}

bool GFile::WriteFile(const GString& path, const void* pData, size_t bytes)
{
	GFileDevice fileIO(path, GOperation::FileCreate, GFileMode::ModeWrite, GFileShareMode::ShareRead);
	if (!fileIO.open())
		return false;
	return fileIO.writeSync(pData, bytes) == bytes;
}

GFileAttributes GFile::GetFileAttributes(const GString& path)
{
	return GetFileAttributesW(path.data());
}

GFile::GFile(const GPath& path)
	:m_path(path)
{

}

const GString& GFile::getPath() const
{
	return m_path.getPath();
}

GString GFile::getDriver() const
{
	return m_path.getDriver();
}

GString GFile::getDir() const
{
	return m_path.getDir();
}

GString GFile::getBasename() const
{
	return m_path.getBasename();
}

GString GFile::getFilename() const
{
	return m_path.getFilename();
}

GString GFile::getExtname() const
{
	return m_path.getExtname();
}

GString GFile::getAbsolutePath() const
{
	if (m_path.isAbsolutePath())
		return m_path.getPath();
	return m_path.getAbsolutePath().getPath();
}

GString GFile::getCanonicalPath() const
{
	GString path = L"file:///";
	if (m_path.isAbsolutePath())
		path += m_path.getPath();
	else
		path += m_path.getAbsolutePath().getPath();
	return std::move(path);
}

// GString GFile::getRelativePath(const GPath& path) const
// {
// 	GString abpath1 = getAbsolutePath();
// 	GString abpath2 = path.getAbsolutePath().getPath();
// 	if (abpath1.empty())
// 		return L"";
// 	if (abpath2.empty() || !tolower(abpath1.front()) != ::tolower(abpath2.front()))
// 		return std::move(abpath1);
// 
// 	GString::iterator itor1 = abpath1.begin();
// 	GString::iterator itor2 = abpath2.begin();
// 
// 	GString::iterator record1 = itor1;
// 	GString::iterator record2 = itor2;
// 
// 	while (itor1 != abpath1.end() && itor2 != abpath2.end())
// 	{
// 		if (ComparePathChar(*itor1, *itor2))
// 			break;
// 		if (IsSeperator(*itor1))
// 		{
// 			record1 = itor1;
// 			record2 = itor2;
// 		}
// 	}
// }

bool GFile::isValid() const
{
	return m_path.isValid();
}

GFileDevicePtr GFile::open(GFileMode mode) const
{
	return new GFileDevice(getPath(), GOperation::FileOpen, mode);
}

GFileDevicePtr GFile::openShare(GFileMode mode, GFileShareModes shareMode) const
{
	return new GFileDevice(getPath(), GOperation::FileOpen, mode, shareMode);
}

bool GFile::create(const GString& newPath)
{
	HANDLE hFile = CreateFileW(newPath.data(), FILE_ALL_ACCESS, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	::CloseHandle(hFile);
	return true;
}

bool GFile::createTemporary(const GString& newPath)
{
	HANDLE hFile = CreateFileW(newPath.data(), FILE_ALL_ACCESS, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_DELETE_ON_CLOSE, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	::CloseHandle(hFile);
	return true;
}

bool GFile::remove(bool force /*= false*/)
{
	return ::DeleteFileW(getPath().data()) != FALSE;
}

bool GFile::move(const GString& newPath)
{
	return ::MoveFileW(getPath().data(), newPath.data()) != FALSE;
}

bool GFile::rename(const GString& newPath)
{
	return move(newPath);
}

bool GFile::hide()
{
	GString tmp = getPath();
	DWORD attr = GetFileAttributesW(tmp.data());
	if (attr = INVALID_FILE_ATTRIBUTES)
		return false;
	return SetFileAttributesW(tmp.data(), attr | FILE_ATTRIBUTE_HIDDEN) != FALSE;
}

bool GFile::setFileAttributes(GFileAttributes attributes)
{
	return SetFileAttributesW(getPath().data(), attributes) != FALSE;
}

bool GFile::isDirectory() const
{
	return (GetFileAttributesW(getPath().data()) & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool GFile::isExists() const
{
	return GetFileAttributesW(getPath().data()) != INVALID_FILE_ATTRIBUTES;
}

bool GFile::isHidden() const
{
	return (GetFileAttributesW(getPath().data()) & FILE_ATTRIBUTE_HIDDEN) != 0;
}

bool GFile::isSystem() const
{
	return (GetFileAttributesW(getPath().data()) & FILE_ATTRIBUTE_SYSTEM) != 0;
}

bool GFile::isReadonly() const
{
	return (GetFileAttributesW(getPath().data()) & FILE_ATTRIBUTE_READONLY) != 0;
}

bool GFile::isEncrypted() const
{
	return (GetFileAttributesW(getPath().data()) & FILE_ATTRIBUTE_ENCRYPTED) != 0;
}

size_t GFile::getFileSize() const
{
	GFileDevice file(getPath(), GOperation::FileOpen, GFileMode::ModeRead, GFileShareMode::ShareRead | GFileShareMode::ShareWrite);
	if (file.isClosed())
		return 0;
	return file.getSize();
}

GFileAttributes GFile::getFileAttributes() const
{
	return ::GetFileAttributesW(getPath().data());
}

GFileStats GFile::getFileStats() const
{
	GFileDevice file(getPath(), GOperation::FileOpen, GFileMode::ModeRead, GFileShareMode::ShareRead | GFileShareMode::ShareWrite);
	if (file.isClosed())
		return GFileStats();
	return file.getStats();
}

void GFile::write(const void* pData, size_t size) const
{
	GFile::WriteFile(getPath(), pData, size);
}

void GFile::write(const GDataArray& buffer) const
{
	GFile::WriteFile(getPath(), buffer);
}

GDataArray GFile::read() const
{
	return GFile::ReadFile(getPath());
}
