// #ifndef __GFILEIO_H__
// #define __GFILEIO_H__
// #include "GIODevice.h"
// #include <fcntl.h>
// #include <sys\types.h>
// #include <sys\stat.h>
// #include "..\GDataArray.h"
// 
// GREFPTR_DEF(GFileIO);
// 
// class GFileIO: public GIODevice
// {
// public:
// 	enum Operation
// 	{
// 		Create = CREATE_ALWAYS,
// 		Open = OPEN_EXISTING,
// 		Truncate = TRUNCATE_EXISTING
// 	};
// 	GEnumsDef(Operation);
// 
// 	enum Mode
// 	{
// 		ModeRead = GENERIC_READ,
// 		ModeWrite = GENERIC_WRITE,
// 		ModeReadWrite = ModeRead | ModeWrite
// 	};
// 
// 	enum ShareMode
// 	{
// 		ShareNone = 0,
// 		ShareRead = FILE_SHARE_READ,
// 		ShareWrite = FILE_SHARE_WRITE,
// 		ShareDelete = FILE_SHARE_DELETE
// 	};
// 	GEnumsDef(ShareMode);
// 
// 	enum Attribute
// 	{
// 		Archive = FILE_ATTRIBUTE_ARCHIVE,
// 		Encrypted = FILE_ATTRIBUTE_ENCRYPTED,
// 		Hidden = FILE_ATTRIBUTE_HIDDEN,
// 		Normal = FILE_ATTRIBUTE_NORMAL,
// 		Readonly = FILE_ATTRIBUTE_READONLY,
// 		System = FILE_ATTRIBUTE_SYSTEM,
// 		Temporary = FILE_ATTRIBUTE_TEMPORARY,
// 
// 		DeleteOnClose = FILE_FLAG_DELETE_ON_CLOSE,
// 		NoBuffer = FILE_FLAG_NO_BUFFERING,
// 		NoRecall = FILE_FLAG_OPEN_NO_RECALL,
// 		Random = FILE_FLAG_RANDOM_ACCESS,
// 		Sequential = FILE_FLAG_SEQUENTIAL_SCAN,
// 		DirectWrite = FILE_FLAG_WRITE_THROUGH
// 	};
// 	GEnumsDef(Attribute);
// 
// 	struct Stats
// 	{
// 		Attributes attributes;
// 		unsigned __int64 createTime;
// 		unsigned __int64 accessTime;
// 		unsigned __int64 lastModifiedTime;
// 		unsigned int volumeSerialNumber;
// 		unsigned __int64 size;
// 		unsigned int numOfLinks;
// 		unsigned __int64 fileIndex;
// 	};
// 
// 	GFileIO();
// 	GFileIO(const GString& file, Operations op = Open, Mode mode = ModeReadWrite, ShareModes shareModes = ShareNone);
// 	~GFileIO();
// 
// 	bool open(const GString& filename, Operations op, Mode mode, Attributes attributes = Normal);
// 	bool openShare(const GString& filename, Operations op, Mode mode, ShareModes shareMode, Attributes attributes = Normal);
// 	
// 	virtual void close() override;
// 	virtual bool isClosed() const override;
// 
// 	virtual void flush() override;
// 	GDataArray readAll();
// 
// 	unsigned __int64 getSize() const;
// 	Stats getStats() const;
// 
// 
// 	virtual bool open(bool async = false) override;
// 	virtual IOResult write(const void* pData, size_t size) override;
// 	virtual IOResult read(void* pData, size_t size, size_t* readsize = nullptr) override;	
// 	virtual bool isAsync() const override;
// 	virtual void* getNativeHandle() const override;
// private:
// 	GObjectClass(GFileIO);
// 
// 	HANDLE m_hFile;
// 	bool m_asyncFlag;
// };
// 
// typedef GFileIO::Stats GFileStats;
// #endif