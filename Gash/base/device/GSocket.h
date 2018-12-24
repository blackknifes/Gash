#ifndef __GSOCKET_H__
#define __GSOCKET_H__
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 
#endif
#include "GDevice.h"
#include <WinSock2.h>
#include <mswsock.h>
#include <ws2ipdef.h>
#include "../GUrlParts.h"
#include "../async/fileio/GAsyncIO.h"

class GSocket: public GDevice
{
public:
	using QueryDnsCallback = GAsyncIO::QueryDnsCallback;

	enum Protocol
	{
		SocketTcp,
		SocketUdp
	};

	enum IPType
	{
		IPv4,
		IPv6
	};

	union IPAddress
	{
		sockaddr_in address_v4;
		sockaddr_in6 address_v6;
	};

	static bool QueryDns(const QueryDnsCallback& callback, const GUrlParts& url, DWORD timeout = 5000, Protocol protocol = SocketTcp);

	GSocket(const GString& host, int port = -1, Protocol protocol = SocketTcp, IPType ipType = IPv4);

	virtual bool open(bool async = false) override;
	virtual void close() override;
	virtual bool isClosed() const override;
	virtual IOResult write(const void* pData, size_t size) override;
	virtual IOResult read(void* pData, size_t size, size_t* readsize = nullptr) override;
	virtual void flush() override;
	virtual bool isAsync() const override;
	virtual void* getNativeHandle() const override;

	int getOptionInt(int flag) const;
	void setOptionInt(int flag, int value);
private:
	GObjectClass(GSocket);
	SOCKET m_socket;
	IPType m_ipType;
	Protocol m_protocol;
	GString m_host;
	int m_port;
	bool m_async;
};
#endif