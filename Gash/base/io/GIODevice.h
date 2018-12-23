#ifndef __GIODEVICE_H__
#define __GIODEVICE_H__
#include "../GObject.h"
#include "../GString.h"
#include "../GDataArray.h"
#include <io.h>

GREFPTR_DEF(GIODevice);

class GIODevice: public GObject
{
public:
	enum SeekType
	{
		SeekCur,
		SeekBegin,
		SeekEnd
	};

	virtual size_t read(void* pData, size_t bytes) = 0;
	virtual size_t write(const void* pData, size_t bytes) = 0;
	GDataArray read(size_t bytes);
	size_t write(const GDataArray& pBuffer);
	virtual GDataArray readAll();

	virtual void flush() = 0;
	virtual __int64 seekRead(__int64 offset, SeekType pos = SeekCur) = 0;
	virtual __int64 seekWrite(__int64 offset, SeekType pos = SeekCur) = 0;
	virtual __int64 tellRead() const = 0;
	virtual __int64 tellWrite() const = 0;

	virtual void resetRead();
	virtual void resetWrite();
	virtual void reset();
	virtual void close() = 0;
	virtual bool isClosed() const = 0;

	virtual int getErrorCode() const = 0;
	virtual GString getErrorMessage() const = 0;
};
#endif