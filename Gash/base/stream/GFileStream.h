#ifndef __GFILESTREAM_H__
#define __GFILESTREAM_H__

#include "GStream.h"
#include "../device/GFileDevice.h"

GREFPTR_DEF(GFileInputStream);
GREFPTR_DEF(GFileOutputStream);
GREFPTR_DEF(GFileStream);

class GFileInputStream: public GInputStream, public virtual GStreamDevice
{
public:
	GFileInputStream(const GString& path);
	virtual size_t read(void* pData, size_t _size) override;

private:
	GObjectClassPooled(GFileInputStream);
protected:
	GFileInputStream() {};
};

class GFileOutputStream: public GOutputStream, public virtual GStreamDevice
{
public:
	GFileOutputStream(const GString& path);
	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;


	virtual void end() override;

private:
	GObjectClassPooled(GFileOutputStream);

protected:
	GFileOutputStream() {};
};

class GFileStream: public GFileInputStream, public GFileOutputStream
{
public:
	GFileStream(const GString& path, 
		GOperations op = GOperation::FileCreate,
		GFileMode mode = GFileMode::ModeReadWrite,
		GFileShareModes shareMode = GFileShareMode::ShareRead | GFileShareMode::ShareWrite | GFileShareMode::ShareDelete
		);
	
private:
	GObjectClassPooled(GFileStream);
};
#endif