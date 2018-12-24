#ifndef __GFILESTREAM_H__
#define __GFILESTREAM_H__

#include "GStream.h"
#include "..\io\GFileIO.h"

GREFPTR_DEF(GFileInputStream);
GREFPTR_DEF(GFileOutputStream);
GREFPTR_DEF(GFileStream);

class GFileInputStream: public GInputStream
{
public:
	using Operation = GFileIO::Operation;
	using Operations = GFileIO::Operations;

	GFileInputStream(const GString& path);
	virtual size_t read(void* pData, size_t _size) override;

private:
	GObjectClassPooled(GFileInputStream);
protected:
	GFileInputStream() {};
};

class GFileOutputStream: public GOutputStream
{
public:
	using Operation = GFileIO::Operation;
	using Operations = GFileIO::Operations;

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
	using Operation = GFileIO::Operation;
	using Operations = GFileIO::Operations;
	using Mode = GFileIO::Mode;
	using ShareMode = GFileIO::ShareMode;
	using ShareModes = GFileIO::ShareModes;
	GFileStream(const GString& path, 
		Operations op = Operation::Create,
		Mode mode = Mode::ModeReadWrite,
		ShareModes shareMode = 
			ShareMode::ShareRead | ShareMode::ShareWrite | ShareMode::ShareDelete
		);
	
private:
	GObjectClassPooled(GFileStream);
};
#endif