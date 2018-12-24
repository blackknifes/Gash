#ifndef __GASYNCTCPSOCKET_H__
#define __GASYNCTCPSOCKET_H__
#include <WinSock2.h>
#include <Windows.h>
#include "GAsyncDevice.h"


GREFPTR_DEF(GAsyncTCPSocket);

class GAsyncTCPSocket: public GAsyncDevice
{
public:
	virtual bool open(const OpenCallback& callback = nullptr) override;
	virtual void close(const CloseCallback& callback = nullptr) override;
	virtual bool isClosed() override;
	virtual bool read(size_t size, const ReadCallback& callback = nullptr) override;
	virtual bool write(const void* pData, size_t size, const WriteCallback& callback = nullptr) override;
	virtual void* getNativeHandle() const override;
protected:
	virtual size_t onRead(const void* pData, size_t size) override;
	virtual size_t onWrite(size_t size) override;
	virtual void onEnd() override;
	virtual void onConnect();
	virtual void onDisconnect();


	friend class GAsyncEngine;
private:
	GObjectClassPooled(GAsyncTCPSocket);
	GAsyncTCPSocket(const GString& host, int port = 0);
	~GAsyncTCPSocket();
	GString m_strHost;
	int m_port;
	SOCKET m_socket;
	int m_iErrCode;
};
#endif