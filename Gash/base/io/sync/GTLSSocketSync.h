#ifndef __GTLSSOCKETSYNC_H__
#define __GTLSSOCKETSYNC_H__
#include "GIODeviceSync.h"

class GTLSSocketSync: public GIODeviceSync
{
public:
	GTLSSocketSync(GProtocol protocol = Protocol_TCP);
	~GTLSSocketSync();

	virtual int read(void* pData, size_t size) override;
	virtual int write(const void* pData, size_t size) override;
	virtual void flush() override;
	virtual bool isClosed() const override;	
	virtual void close() override;
	virtual void* getNativeHandle() const override;

	bool connect(const GString& host, int port = 443);
	void disconnect();
private:
	GObjectClassPooled(GTLSSocketSync);
	void* m_pTLSSocket;
};
#endif