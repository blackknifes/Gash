#include "GFileStream.h"

GFileInputStream::GFileInputStream(const GString& path)
{
	setDevice(GFileDevice::Create(
		path, 
		GFileDevice::FileCreateOnNotExist, 
		GFileDevice::ModeRead,
		GFileDevice::ShareRead | GFileDevice::ShareWrite | GFileDevice::ShareDelete));
}

size_t GFileInputStream::read(void* pData, size_t _size)
{
	return getDevice()->readSync(pData, _size);
}

GFileOutputStream::GFileOutputStream(const GString& path)
{
	setDevice(GFileDevice::Create(
		path,
		GFileDevice::FileCreate, 
		GFileDevice::ModeWrite, 
		GFileDevice::ShareRead | GFileDevice::ShareDelete));
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
	Operations op /*= Operation::Create*/, 
	Mode mode /*= Mode::ModeReadWrite*/, 
	ShareModes shareMode /*= ShareMode::ShareRead | ShareMode::ShareWrite | ShareMode::ShareDelete */)
{
	setDevice(new GFileDevice(path, op, mode, shareMode));
}
