#ifndef __GFILEDEVICE_H__
#define __GFILEDEVICE_H__
#include "../io/GIODevice.h"
#include "GAsyncIO.h"


GREFPTR_DEF(GFileDevice);

class GFileDevice: public GIODevice
{
public:
	enum Operation
	{
		FileCreateOnNotExist = CREATE_NEW,
		FileCreate = CREATE_ALWAYS,
		FileOpen = OPEN_EXISTING,
		FileTruncate = TRUNCATE_EXISTING
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

	struct Stats
	{
		Attributes attributes;
		unsigned __int64 createTime;
		unsigned __int64 accessTime;
		unsigned __int64 lastModifiedTime;
		unsigned int volumeSerialNumber;
		unsigned __int64 size;
		unsigned int numOfLinks;
		unsigned __int64 fileIndex;
	};

	static GFileDevicePtr Create(
		const GString& path, 
		Operations op = FileCreateOnNotExist, 
		Mode mode = ModeReadWrite, 
		ShareModes share = ShareNone);
	GFileDevice(const GString& path, Operations op = FileCreateOnNotExist, Mode mode = ModeReadWrite, ShareModes share = ShareNone);
	~GFileDevice();
	bool isExists() const;
	virtual bool open() override;
	bool open(DWORD attributes);
	virtual void close() override;
	virtual bool isClosed() const override;
	virtual void flush() override;
	virtual void* getNativeHandle() const override;
	size_t getSize() const;

	const GString& getPath() const;
	bool canRead() const;
	bool canWrite() const;

	Stats getStats() const;

	virtual size_t writeSync(const void* pData, size_t size) override;
	virtual size_t readSync(void* pData, size_t size) override;
	virtual bool write(const void* pData, size_t size, const WriteCallback& callback) override;
	virtual bool read(size_t size, const ReadCallback& callback) override;
	virtual GDataArray readAllSync();

private:
	GObjectClassPooled(GFileDevice);
	void OnRead(const ReadCallback& callback, GAsyncIOPtr pIO);
	void OnWrite(const WriteCallback& callback, GAsyncIOPtr pIO);
	
	HANDLE m_hFile;
	GString m_path;
	Operations m_operations;
	Mode m_mode;
	ShareMode m_shareMode;
};

typedef GFileDevice::Stats GFileStats;
#endif
