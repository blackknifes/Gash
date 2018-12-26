#include "GSocket.h"
#include "ares.h"
#include "../GNSLookup.h"

GSocket::GSocket(GProtocol protocol /*= Protocol_TCP*/)
	:m_protocol(protocol)
{
	m_socket = GSocketPool::PopSocket(protocol);
	u_long val = 0;
	ioctlsocket(m_socket, FIONBIO, &val);
	DWORD timeout = 5000;
	setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
	setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
}

GSocket::~GSocket()
{
	GSocketPool::PushSocket(m_socket);
}

bool GSocket::connect(const GString& host, int port)
{
	GAddrInfo addrInfo = {};
	sockaddr* addr = nullptr;
	int addrlen = 0;
	if (host.isIpV4())
	{
		addr = (sockaddr*)&addrInfo.addr4;
		addrlen = sizeof(addrInfo.addr4);
		addrInfo.addr4.sin_family = AF_INET;
		addrInfo.addr4.sin_port = htons(port);
		ares_inet_pton(AF_INET, host.toAnsi().data(), &addrInfo.addr4.sin_addr);
	}
	else if (host.isIpV6())
	{
		addr = (sockaddr*)&addrInfo.addr6;
		addrInfo.addr6.sin6_port = htons(port);
		addrInfo.addr6.sin6_family = AF_INET;
		addrlen = sizeof(addrInfo.addr6);
		ares_inet_pton(AF_INET6, host.toAnsi().data(), &addrInfo.addr6.sin6_addr);
	}
	if (addr == nullptr)
	{
		auto addrs = GNSLookupV4(host, port);
		if (addrs.empty())
			return false;
		addrInfo = addrs.front();
		addrlen = sizeof(addrInfo.addr4);
		addrInfo.addr4.sin_family = AF_INET;
		addrInfo.addr4.sin_port = htons(port);
		addr = (sockaddr*)&addrInfo.addr4;
	}

 	return ::connect(m_socket, addr, addrlen) == 0;
}

void GSocket::disconnect()
{
	GSocketPool::getDisConnectEx()(m_socket, nullptr, TF_REUSE_SOCKET, 0);
}

void GSocket::close()
{
	disconnect();
}

size_t GSocket::read(void* pData, size_t size)
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

size_t GSocket::write(const void* pData, size_t size)
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

void GSocket::flush()
{
	
}

bool GSocket::isClosed() const
{
	DWORD val;
	int len = sizeof(val);
	getsockopt(m_socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&val, &len);
	return val == SOCKET_ERROR;
}

void* GSocket::getNativeHandle() const
{
	return (void*)m_socket;
}

GProtocol GSocket::getProtocol() const
{
	return m_protocol;
}
