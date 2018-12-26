#ifndef __GFILEDEVICE_H__
#define __GFILEDEVICE_H__
#include "../io/GIODevice.h"
#include "GAsyncIO.h"


GREFPTR_DEF(GFileDevice);

class GFileDevice: public GIODevice
{
public:
	static GFileDevicePtr Create(
		const GString& path, 
		GOperations op = FileCreateOnNotExist, 
		GFileMode mode = ModeReadWrite, 
		GFileShareModes share = ShareNone);
	GFileDevice(const GString& path, GOperations op = FileCreateOnNotExist, GFileMode mode = ModeReadWrite, GFileShareModes share = ShareNone);
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

	GFileStats getStats() const;

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
	GOperations m_operations;
	GFileMode m_mode;
	GFileShareMode m_shareMode;
};
#endif
