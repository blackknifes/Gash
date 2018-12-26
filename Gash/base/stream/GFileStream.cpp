#include "GFileStream.h"

GFileInputStream::GFileInputStream(const GString& path)
{
	setDevice(GFileDevice::Create(
		path, 
		GOperation::FileCreateOnNotExist,
		GFileMode::ModeRead,
		GFileShareMode::ShareRead | GFileShareMode::ShareWrite | GFileShareMode::ShareDelete));
}

size_t GFileInputStream::read(void* pData, size_t _size)
{
	return getDevice()->readSync(pData, _size);
}

GFileOutputStream::GFileOutputStream(const GString& path)
{
	setDevice(GFileDevice::Create(
		path,
		GOperation::FileCreate, 
		GFileMode::ModeWrite, 
		GFileShareMode::ShareRead | GFileShareMode::ShareDelete));
}

void GFileOutputStream::write(const void* pData, size_t _size)
{
	getDevice()->writeSync(pData, _size);
}

void GFileOutputStream::flush()
{
	getDevice()->flush();
}

void GFileOutputStream::end()
{
	getDevice()->close();
}

GFileStream::GFileStream(
	const GString& path, 
	GOperations op /*= Operation::Create*/, 
	GFileMode mode /*= Mode::ModeReadWrite*/,
	GFileShareModes shareMode /*= ShareMode::ShareRead | ShareMode::ShareWrite | ShareMode::ShareDelete */)
{
	setDevice(new GFileDevice(path, op, mode, shareMode));
}
