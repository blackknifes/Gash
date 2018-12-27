#include "GSocketSync.h"
#include "ares.h"
#include "../GNSLookup.h"

GSocketSync::GSocketSync(GProtocol protocol /*= Protocol_TCP*/)
	:m_protocol(protocol)
{
	m_socket = GSocketPool::PopSocket(protocol);
	u_long val = 0;
	ioctlsocket(m_socket, FIONBIO, &val);
	DWORD timeout = 5000;
	setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
}

GSocketSync::~GSocketSync()
{
	GSocketPool::PushSocket(m_socket);
}

bool GSocketSync::connect(const GString& host, int port)
{
	if (!isClosed())
		return false;
	GAddrInfo addrInfo = {};
	int addrlen;
	if (!GGetHostByNameV4(&addrInfo, &addrlen, host, port))
		return false;

 	return ::connect(m_socket, &addrInfo.addr, addrlen) == 0;
}

void GSocketSync::disconnect()
{
	GSocketPool::getDisConnectEx()(m_socket, nullptr, TF_REUSE_SOCKET, 0);
}

void GSocketSync::close()
{
	disconnect();
}

int GSocketSync::read(void* pData, size_t size)
{
	switch (m_protocol)
	{
	case Protocol_TCP:
		return ::recv(m_socket, (char*)pData, size, 0);
	default:
		break;
	}
	return 0;
}

int GSocketSync::write(const void* pData, size_t size)
{
	switch (m_protocol)
	{
	case Protocol_TCP:
		return ::send(m_socket, (const char*)pData, size, 0);
	default:
		break;
	}
	return 0;
}

void GSocketSync::flush()
{
	
}

bool GSocketSync::isClosed() const
{
	DWORD val;
	int len = sizeof(val);
	getsockopt(m_socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&val, &len);
	return val == SOCKET_ERROR;
}

void* GSocketSync::getNativeHandle() const
{
	return (void*)m_socket;
}

GProtocol GSocketSync::getProtocol() const
{
	return m_protocol;
}
