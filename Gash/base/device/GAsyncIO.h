#ifndef __GASYNCIO_H__
#define __GASYNCIO_H__
#include "../GObject.h"
#include <WinSock2.h>
#include "../stream/GStream.h"
#include "../io/GIODevice.h"
#include "../io/GBufferIO.h"
#include <functional>

GREFPTR_DEF(GAsyncIO);

#define GAsyncCallback_1(func, obj) std::bind(&func, obj, std::placeholders::_1)
#define GAsyncCallback_2(func, obj) std::bind(&func, obj, std::placeholders::_1, std::placeholders::_2)
#define GAsyncCallback_3(func, obj) std::bind(&func, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

class GAsyncIO: public virtual GObject
{
public:
	typedef std::function<void(GAsyncIOPtr pIO)> Callback;

	enum IOType
	{
		IO_Read,
		IO_Write,
		IO_DnsQuery,
		IO_Connect,
		IO_DisConnect
	};

	static GAsyncIOPtr GetAsyncIO(void* overlapped);
	static GAsyncIOPtr CreateIO(GIODevicePtr pDevice, IOType type, const Callback& compCall = nullptr);

	GAsyncIO(GIODevicePtr pDevice, IOType type, const Callback& compCall = nullptr);
	~GAsyncIO();

	LPOVERLAPPED getOverlapped();
	DWORD getSize() const;

	void allocBuffer(size_t size);
	void freeBuffer();
	GBufferIOPtr getBuffer() const;
	const void* getBufferData() const;
	void* getBufferData();
	GIODevicePtr getDevice() const;

	static void OnComplete(LPOVERLAPPED pOverlapped, DWORD dwTransferSize, ULONG_PTR compKey);
	static bool HasPending();
private:
	GObjectClassPooled(GAsyncIO);
	OVERLAPPED m_overlapped;
	IOType m_type;
	DWORD m_dwTransferSize;
	GBufferIOPtr m_bufferIO;
	void* m_pBufferData;
	size_t m_bufferSize;
	GIODevicePtr m_pIODevice;
	Callback m_callback;
};
#endif