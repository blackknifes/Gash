#ifndef __GASYNCFILE_H__
#define __GASYNCFILE_H__
#include "GAsyncDevice.h"

GREFPTR_DEF(GAsyncFile);

class GAsyncFile: public virtual GAsyncDevice
{
public:
	enum Operation
	{
		FileNew = CREATE_ALWAYS,
		FileOpen = OPEN_EXISTING,
		FileTruncate = TRUNCATE_EXISTING
	};

	enum Mode
	{
		ModeRead = GENERIC_READ,
		ModeWrite = GENERIC_WRITE,
		ModeReadWrite = ModeRead | ModeWrite
	};

	enum ShareMode
	{
		ShareNone = 0,
		ShareRead = FILE_SHARE_READ,
		ShareWrite = FILE_SHARE_WRITE,
		ShareDelete = FILE_SHARE_DELETE,
	};
	GEnumsDef(ShareMode);

	static GAsyncFilePtr Create(const GString& path);

	virtual bool open(const OpenCallback& callback = nullptr) override;
	bool open(Operation op, Mode mode, ShareMode shareMode = ShareNone, const CloseCallback& callback = nullptr);
	virtual void close(const CloseCallback& callback = nullptr) override;
	virtual bool isClosed() override;
	virtual bool read(size_t size, const ReadCallback& callback = nullptr) override;
	virtual bool write(const void* pData, size_t size, const WriteCallback& callback = nullptr) override;

	virtual void* getNativeHandle() const override;
protected:
	virtual size_t onRead(const void* pData, size_t size) override;
	virtual size_t onWrite(size_t size) override;
	virtual void onEnd() override;
private:
	GAsyncFile(const GString& file);
	GObjectClassPooled(GAsyncFile);
	HANDLE m_hFile;
	GString m_strPath;
};
#endif