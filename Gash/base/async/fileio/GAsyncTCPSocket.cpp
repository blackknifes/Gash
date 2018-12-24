#include "GAsyncTCPSocket.h"
#include <WS2tcpip.h>
#include "../../GUrlParts.h"
#include <MSWSock.h>

static LPFN_ACCEPTEX g_AcceptEx = nullptr;
static LPFN_CONNECTEX g_ConnectEx = nullptr;
static LPFN_DISCONNECTEX g_DisConnectEx = nullptr;
static LPFN_GETACCEPTEXSOCKADDRS g_GetAcceptExSockaddrs = nullptr;

static void* GetExtensionFunction(SOCKET s, const GUID& which_fn)
{
	void *ptr = NULL;
	DWORD bytes = 0;
	WSAIoctl(s, SIO_GET_EXTENSION_FUNCTION_POINTER,
		(GUID*)&which_fn, sizeof(which_fn),
		&ptr, sizeof(ptr),
		&bytes, NULL, NULL);

	return ptr;
}

static WORD WsaVersions[] = { MAKEWORD(2, 2), MAKEWORD(2, 1), MAKEWORD(2, 0), MAKEWORD(1,1), MAKEWORD(1,0) };
class WSAStartupStruct
{
public:
	WSAStartupStruct()
	{
		DWORD major = 2, minor = 2;
		for (size_t i = 0; i < sizeof(WsaVersions) / sizeof(WORD); ++i)
		{
			if(WSAStartup(WsaVersions[i], &wsadata) == 0)
				break;
		}
	}

	~WSAStartupStruct()
	{
		WSACleanup();
	}
private:
	WSADATA wsadata;
};

class WSAExtensionFunctionInit
{
public:
	WSAExtensionFunctionInit(SOCKET _socket)
	{
		g_AcceptEx = (LPFN_ACCEPTEX)GetExtensionFunction(_socket, WSAID_ACCEPTEX);
		g_ConnectEx = (LPFN_CONNECTEX)GetExtensionFunction(_socket, WSAID_CONNECTEX);
		g_DisConnectEx = (LPFN_DISCONNECTEX)GetExtensionFunction(_socket, WSAID_DISCONNECTEX);
		g_GetAcceptExSockaddrs = (LPFN_GETACCEPTEXSOCKADDRS)GetExtensionFunction(_socket, WSAID_GETACCEPTEXSOCKADDRS);
	}
};

GAsyncTCPSocket::GAsyncTCPSocket(const GString& host, int port /*= 0*/)
	:m_strHost(host), m_port(port)
{
	static WSAStartupStruct startup;
	m_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	if (m_socket == INVALID_SOCKET)
		m_socket = NULL;
	else
	{
		static WSAExtensionFunctionInit extensionInit(m_socket);
		BOOL flag = TRUE;
		setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&flag, sizeof(flag));
	}

}


GAsyncTCPSocket::~GAsyncTCPSocket()
{
	close();
	closesocket(m_socket);
}

bool GAsyncTCPSocket::open(const OpenCallback& callback /*= nullptr*/)
{
	if (!isClosed())
		return false;
	GUrlParts url(m_strHost);
	if (m_port == 0)
		m_port = url.getPort();
	ADDRINFOW* info;
	ADDRINFOW hint = {};
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	if (GetAddrInfoW(url.getHost().toString().data(), nullptr, &hint, &info) != NO_ERROR)
		return false;
	bool result = false;
	ADDRINFOW* pConnectInfo = info;

	GAsyncIO* pIO = GNewIO();
	pIO->ioType = GAsyncIO::NetConnect;
	pIO->openCallback = callback;

	sockaddr_in bindAddr = {};
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons((short)0);
	bindAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(m_socket, (sockaddr*)&bindAddr, sizeof(bindAddr)) != 0)
		return false;

	while (pConnectInfo)
	{
		sockaddr_in* addr = (sockaddr_in*)pConnectInfo->ai_addr;
		addr->sin_port = htons(m_port);
		if (g_ConnectEx(
			m_socket, 
			pConnectInfo->ai_addr, 
			pConnectInfo->ai_addrlen, 
			0, 
			0, 
			0, 
			&pIO->overlapped) == FALSE && 
			(m_iErrCode = WSAGetLastError()) != ERROR_IO_PENDING)
		{
			pConnectInfo = pConnectInfo->ai_next;
			continue;
		}
		result = true;
		break;
	}

	if (!result)
		GFreeIO(pIO);
	FreeAddrInfoW(info);
	return result;
}

void GAsyncTCPSocket::close(const CloseCallback& callback /*= nullptr*/)
{
	if (m_socket != NULL)
	{
		GAsyncIO* pIO = GNewIO();
		pIO->ioType = GAsyncIO::NetDisconnect;
		pIO->closeCallback = callback;
		if (g_DisConnectEx(m_socket, &pIO->overlapped, TF_REUSE_SOCKET, 0) == FALSE && WSAGetLastError() != ERROR_IO_PENDING)
			GFreeIO(pIO);
	}
}

bool GAsyncTCPSocket::isClosed()
{
	int val = 0;
	int size = sizeof(val);
	getsockopt(m_socket, SOL_SOCKET, SO_CONNECT_TIME, (char*)&val, &size);
	return val == 0xFFFFFFFF;
}
 
bool GAsyncTCPSocket::read(size_t size, const ReadCallback& callback/*= nullptr*/)
{
	GAsyncIO* pIO = GNewIO();
	pIO->dwExpectSize = size;
	pIO->readCallback = callback;
	WSABUF wsaBuf;
	wsaBuf.buf = (char*)pIO->buffer;
	wsaBuf.len = sizeof(pIO->buffer);
	DWORD flag = 0;
	pIO->ioType = GAsyncIO::IORead;
	bool result = WSARecv(m_socket, &wsaBuf, 1, nullptr, &flag, &pIO->overlapped, nullptr) == NO_ERROR || GetLastError() == ERROR_IO_PENDING;
	if (!result)
		GFreeIO(pIO);
	return result;
}
 
bool GAsyncTCPSocket::write(const void* pData, size_t size, const WriteCallback& callback /*= nullptr*/)
{
	GAsyncIO* pIO = GNewIO();
	pIO->dwExpectSize = size;
	pIO->writeCallback = callback;
	WSABUF wsaBuf;
	wsaBuf.buf = (char*)pIO->buffer;
	wsaBuf.len = size;
	memcpy_s(wsaBuf.buf, wsaBuf.len, pData, size);
	DWORD flag = 0;
	pIO->ioType = GAsyncIO::IOWrite;
	bool result = WSASend(m_socket, &wsaBuf, 1, nullptr, 0, &pIO->overlapped, nullptr) == NO_ERROR || GetLastError() == ERROR_IO_PENDING;
	if (!result)
		GFreeIO(pIO);
	return result;
}

void* GAsyncTCPSocket::getNativeHandle() const
{
	return (void*)m_socket;
}

size_t GAsyncTCPSocket::onRead(const void* pData, size_t size)
{
	return size;
}

size_t GAsyncTCPSocket::onWrite(size_t size)
{
	return size;
}

void GAsyncTCPSocket::onEnd()
{
	
}

void GAsyncTCPSocket::onConnect()
{

}

void GAsyncTCPSocket::onDisconnect()
{

}
