#include "GSocketPool.h"
#include "../net/GNetEngine.h"

static void* GetExtensionFunction(SOCKET s, const GUID& guid)
{
	void* ptr;
	DWORD retBytes;
	WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER, (void*)&guid, sizeof(guid), &ptr, sizeof(ptr), &retBytes, nullptr, nullptr);
	return ptr;
}

SOCKET GSocketPool::PopSocket(GProtocol protocol /*= Protocol_TCP*/)
{
	return GSocketPool::getInstance()->pop(protocol);
}

void GSocketPool::PushSocket(SOCKET s)
{
	GSocketPool::getInstance()->push(s);
}

LPFN_ACCEPTEX GSocketPool::getAcceptEx()
{
	return GSocketPool::getInstance()->m_pAcceptEx;
}

LPFN_CONNECTEX GSocketPool::getConnectEx()
{
	return GSocketPool::getInstance()->m_pConnectEx;
}

LPFN_DISCONNECTEX GSocketPool::getDisConnectEx()
{
	return GSocketPool::getInstance()->m_pDisConnectEx;
}

LPFN_GETACCEPTEXSOCKADDRS GSocketPool::getGetAcceptExSockAddrsEx()
{
	return GSocketPool::getInstance()->m_pGetAcceptExSockAddrs;
}

GSocketPool::GSocketPool()
{
	GNetEngine::init();
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s != SOCKET_ERROR)
	{
		m_pAcceptEx = (LPFN_ACCEPTEX)GetExtensionFunction(s, WSAID_ACCEPTEX);
		m_pConnectEx = (LPFN_CONNECTEX)GetExtensionFunction(s, WSAID_ACCEPTEX);
		m_pDisConnectEx = (LPFN_DISCONNECTEX)GetExtensionFunction(s, WSAID_ACCEPTEX);
		m_pGetAcceptExSockAddrs = (LPFN_GETACCEPTEXSOCKADDRS)GetExtensionFunction(s, WSAID_ACCEPTEX);
		m_tcpSocketPool.push(s);
	}
}

GSocketPool::~GSocketPool()
{
	m_tcpSocketPool.lock();
	for (SOCKET s: m_tcpSocketPool)
		closesocket(s);
	m_tcpSocketPool.unlock();
	m_udpSocketPool.lock();
	for (SOCKET s: m_udpSocketPool)
		closesocket(s);
	m_udpSocketPool.unlock();
}

SOCKET GSocketPool::pop(GProtocol protocol /*= Protocol_TCP*/)
{
	switch (protocol)
	{
	case GProtocol::Protocol_TCP:
		if (m_tcpSocketPool.empty())
		{
			SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (s == SOCKET_ERROR)
				return s;
			m_tcpSocketPool.push(s);
		}
		return m_tcpSocketPool.pop();
	case GProtocol::Protocol_UDP:
		if (m_udpSocketPool.empty())
		{
			SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (s == SOCKET_ERROR)
				return s;
			m_udpSocketPool.push(s);
		}
		return m_udpSocketPool.pop();
	default:
		break;
	}
	return 0;
}

void GSocketPool::push(SOCKET s)
{
	int val;
	int len = sizeof(val);
	getsockopt(s, SOL_SOCKET, SO_TYPE, (char*)&val, &len);
	switch (val)
	{
	case SOCK_STREAM:
		m_tcpSocketPool.push(s);
		break;
	case SOCK_DGRAM:
		m_udpSocketPool.push(s);
		break;
	default:
		break;
	}
}
