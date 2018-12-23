#include "GFileStream.h"
#include "..\io\GFileIO.h"

GFileInputStream::GFileInputStream(const GString& path)
{
	setDevice(new GFileIO(
		path, 
		GFileIO::Open, 
		GFileIO::ModeRead, 
		GFileIO::ShareRead | GFileIO::ShareWrite | GFileIO::ShareDelete));
}

size_t GFileInputStream::read(void* pData, size_t _size)
{
	return getDevice()->read(pData, _size);
}

GFileOutputStream::GFileOutputStream(const GString& path)
{
	setDevice(new GFileIO(
		path,
		GFileIO::Create, 
		GFileIO::ModeWrite, 
		GFileIO::ShareRead | GFileIO::ShareDelete));
}

void GFileOutputStream::write(const void* pData, size_t _size)
{
	getDevice()->write(pData, _size);
}

void GFileOutputStream::flush()
{
	getDevice()->flush();
}

GFileStream::GFileStream(
	const GString& path, 
	Operations op /*= Operation::Create*/, 
	Mode mode /*= Mode::ModeReadWrite*/, 
	ShareModes shareMode /*= ShareMode::ShareRead | ShareMode::ShareWrite | ShareMode::ShareDelete */)
{
	setDevice(new GFileIO(path, op, mode, shareMode));
}
