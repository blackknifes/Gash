#ifndef __GIOENUMS_H__
#define __GIOENUMS_H__
#include <Windows.h>
#include "../GEnums.h"

enum GOperation
{
	FileCreateOnNotExist = CREATE_NEW,
	FileCreate = CREATE_ALWAYS,
	FileOpen = OPEN_EXISTING,
	FileTruncate = TRUNCATE_EXISTING
};
GEnumsDef(GOperation);

enum GFileMode
{
	ModeRead = GENERIC_READ,
	ModeWrite = GENERIC_WRITE,
	ModeReadWrite = ModeRead | ModeWrite
};

enum GFileShareMode
{
	ShareNone = 0,
	ShareRead = FILE_SHARE_READ,
	ShareWrite = FILE_SHARE_WRITE,
	ShareDelete = FILE_SHARE_DELETE
};
GEnumsDef(GFileShareMode);

enum GFileAttribute
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
GEnumsDef(GFileAttribute);

struct GFileStats
{
	GFileAttributes attributes;
	unsigned __int64 createTime;
	unsigned __int64 accessTime;
	unsigned __int64 lastModifiedTime;
	unsigned int volumeSerialNumber;
	unsigned __int64 size;
	unsigned int numOfLinks;
	unsigned __int64 fileIndex;
};


enum GProtocol
{
	Protocol_Unkown = 0,
	Protocol_TCP,
	Protocol_UDP
};

#endif