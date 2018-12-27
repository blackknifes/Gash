#ifndef __GSOCKET_H__
#define __GSOCKET_H__
#include "GIODeviceSync.h"
#include "../GSocketPool.h"

class GSocketSync: public GIODeviceSync
{
public:
	GSocketSync(GProtocol protocol = Protocol_TCP);
	~GSocketSync();

	bool connect(const GString& host, int port);
	void disconnect();
	void close();

	virtual int read(void* pData, size_t size) override;
	virtual int write(const void* pData, size_t size) override;
	virtual void flush() override;
	virtual bool isClosed() const override;
	virtual void* getNativeHandle() const override;
	GProtocol getProtocol() const;

private:
	GObjectClassPooled(GSocketSync);
	SOCKET m_socket;
	GProtocol m_protocol;
};
#endif