#ifndef __GFILESYNC_H__
#define __GFILESYNC_H__
#include "GIODeviceSync.h"

class GFileSync: public GIODeviceSync
{
public:
	GFileSync();
	~GFileSync();

	bool open(const GString& filePath, GOperation op, GFileMode mode = ModeReadWrite, GFileAttributes attrs = GFileAttribute::Normal, GFileShareMode shareMode = ShareNone);
	virtual void close() override;
	virtual bool isClosed() const override;
	virtual void flush() override;
	virtual void* getNativeHandle() const override;
	size_t getSize() const;
	GFileStats getStats() const;
	virtual int read(void* pData, size_t size) override;
	virtual int write(const void* pData, size_t size) override;
private:
	GObjectClassPooled(GFileSync);
	HANDLE m_hFile;
};
#endif