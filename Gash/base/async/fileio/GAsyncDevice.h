#ifndef __GASYNCDEVICE_H__
#define __GASYNCDEVICE_H__
#include "../../GObject.h"
#include "GAsyncIO.h"

GREFPTR_DEF(GAsyncDevice);

class GAsyncDevice: public GObject
{
public:
	typedef GAsyncIO::ReadCallback ReadCallback;
	typedef GAsyncIO::WriteCallback WriteCallback;
	typedef GAsyncIO::OpenCallback OpenCallback;
	typedef GAsyncIO::CloseCallback CloseCallback;
	GAsyncDevice();

	virtual bool open(const OpenCallback& callback = nullptr) = 0;
	virtual void close(const CloseCallback& callback = nullptr) = 0;
	virtual bool isClosed() = 0;
	virtual bool isEnd();
	virtual bool read(size_t size, const ReadCallback& = nullptr) = 0;
	virtual bool write(const void* pData, size_t size, const WriteCallback& callback = nullptr) = 0;
	virtual void end();
	virtual void* getNativeHandle() const = 0;

	friend class GAsyncEngine;
protected:
	virtual size_t onRead(const void* pData, size_t size) = 0;
	virtual size_t onWrite(size_t size) = 0;
	virtual void onEnd() = 0;

private:
	bool m_ended;
};
#endif