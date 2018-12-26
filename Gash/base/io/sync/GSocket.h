#ifndef __GSOCKET_H__
#define __GSOCKET_H__
#include "GIODeviceSync.h"
#include "../GSocketPool.h"

class GSocket: public GIODeviceSync
{
public:
	GSocket(GProtocol protocol = Protocol_TCP);
	~GSocket();

	bool connect(const GString& host, int port);
	void disconnect();
	void close();

	virtual size_t read(void* pData, size_t size) override;
	virtual size_t write(const void* pData, size_t size) override;
	virtual void flush() override;
	virtual bool isClosed() const override;
	virtual void* getNativeHandle() const override;
	GProtocol getProtocol() const;

private:
	GObjectClassPooled(GSocket);
	SOCKET m_socket;
	GProtocol m_protocol;
};
#endif