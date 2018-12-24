#ifndef __GFILEDEVICE_H__
#define __GFILEDEVICE_H__
#include "GDevice.h"
#include "../io/GFile.h"

class GFileDevice: public GDevice
{
public:
	enum Operation
	{
		CreateOnNotExist = CREATE_NEW,
		Create = CREATE_ALWAYS,
		Open = OPEN_EXISTING,
		Truncate = TRUNCATE_EXISTING
	};
	GEnumsDef(Operation);

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
		ShareDelete = FILE_SHARE_DELETE
	};
	GEnumsDef(ShareMode);

	enum Attribute
	{
		Archive = FILE_ATTRIBUTE_ARCHIVE,
		Encrypted = FILE_ATTRIBUTE_ENCRYPTED,
		Hidden = FILE_ATTRIBUTE_HIDDEN,
		Normal = FILE_ATTRIBUTE_NORMAL,
		Readonly = FILE_ATTRIBUTE_READONLY,
		System = FILE_ATTRIBUTE_SYSTEM,
		Temporary = FILE_ATTRIBUTE_TEMPORARY,

		DeleteOnClose = FILE_FLAG_DELETE_ON_CLOSE,
		NoBuffer = FILE_FLAG_NO_BUFFERING,
		NoRecall = FILE_FLAG_OPEN_NO_RECALL,
		Random = FILE_FLAG_RANDOM_ACCESS,
		Sequential = FILE_FLAG_SEQUENTIAL_SCAN,
		DirectWrite = FILE_FLAG_WRITE_THROUGH
	};
	GEnumsDef(Attribute);

	GFileDevice(const GString& path, Operations op = CreateOnNotExist, Mode mode = ModeReadWrite, ShareMode share = ShareNone, Attributes attributes = Normal);
	~GFileDevice();
	bool isExists() const;
	virtual bool open(bool async = false) override;
	virtual void close() override;
	virtual bool isClosed() const override;
	virtual IOResult write(const void* pData, size_t size) override;
	virtual IOResult read(void* pData, size_t size, size_t* readsize = nullptr) override;
	virtual void flush() override;
	virtual bool isAsync() const override;
	virtual void* getNativeHandle() const override;

	GFile getFile() const;
	const GString& getPath() const;
	bool canRead() const;
	bool canWrite() const;

private:
	GObjectClass(GFileDevice);
	HANDLE m_hFile;
	GString m_path;
	Operations m_operations;
	Mode m_mode;
	ShareMode m_shareMode;
	Attributes m_attributes;
};
#endif
