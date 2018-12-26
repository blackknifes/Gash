#ifndef __GFILE_H__
#define __GFILE_H__
#include "../GPath.h"
#include "../GShell.h"
#include "../GDataArray.h"
#include "../device/GFileDevice.h"

class GFile
{
public:
	using DirType = GShell::DirectoryType;

	static GFile GetDir(DirType type);
	static GDataArray ReadFile(const GString& path);
	static bool WriteFile(const GString& path, const GDataArray& buffer);
	static bool WriteFile(const GString& path, const void* pData, size_t bytes);
	static GFileAttributes GetFileAttributes(const GString& path);

	GFile(const GPath& path);

	const GString& getPath() const;
	GString getDriver() const;
	GString getDir() const;
	GString getBasename() const;
	GString getFilename() const;
	GString getExtname() const;
	GString getAbsolutePath() const;
	GString getCanonicalPath() const;
/*	GString getRelativePath(const GPath& path) const;*/

	bool isValid() const;
	GFileDevicePtr open(GFileMode mode) const;
	GFileDevicePtr openShare(GFileMode mode, GFileShareModes shareMode) const;

	bool create(const GString& newPath);
	bool createTemporary(const GString& newPath);
	bool remove(bool force = false);
	bool move(const GString& newPath);
	bool rename(const GString& newPath);
	bool hide();

	bool setFileAttributes(GFileAttributes attributes);

	bool isDirectory() const;
	bool isExists() const;
	bool isHidden() const;
	bool isSystem() const;
	bool isReadonly() const;
	bool isEncrypted() const;

	size_t getFileSize() const;
	GFileAttributes getFileAttributes() const;
	GFileStats getFileStats() const;
	void write(const void* pData, size_t size) const;
	void write(const GDataArray& buffer) const;
	GDataArray read() const;

private:
	GPath m_path;
};
#endif