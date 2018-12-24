#include "GSocket.h"
#include "../GUrlParts.h"
#include <ws2tcpip.h>
#include "../async/fileio/GAsyncIO.h"

static LPFN_ACCEPTEX s_AcceptEx = nullptr;
static LPFN_CONNECTEX s_ConnectEx = nullptr;
static LPFN_DISCONNECTEX s_DisconnectEx = nullptr;
static LPFN_GETACCEPTEXSOCKADDRS s_GetAcceptExSockAddrs = nullptr;
static PWSANAMESPACE_INFOEXW s_dnsCacheInfo = nullptr;

template<typename _pfnTy>
static void GetExtensionFunction(SOCKET sock, const GUID& guid, _pfnTy* pfn)
{
	DWORD unused;
	WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER, (LPVOID)&guid, sizeof(guid), pfn, sizeof(pfn), &unused, nullptr, nullptr);
}

class MSSockInit
{
public:
	MSSockInit(SOCKET socket)
	{
		GetExtensionFunction(socket, WSAID_ACCEPTEX, &s_AcceptEx);
		GetExtensionFunction(socket, WSAID_CONNECTEX, &s_ConnectEx);
		GetExtensionFunction(socket, WSAID_DISCONNECTEX, &s_DisconnectEx);
		GetExtensionFunction(socket, WSAID_GETACCEPTEXSOCKADDRS, &s_GetAcceptExSockAddrs);
	}
};

class DNSCacheInit
{
public:
	DNSCacheInit()
	{
		DWORD bufferLen = 0;
		if (WSAEnumNameSpaceProvidersExW(&bufferLen, nullptr) == SOCKET_ERROR)
		{
			s_dnsCacheInfo = (PWSANAMESPACE_INFOEXW)malloc(bufferLen);
			if (WSAEnumNameSpaceProvidersExW(&bufferLen, s_dnsCacheInfo) == SOCKET_ERROR)
			{
				free(s_dnsCacheInfo);
				s_dnsCacheInfo = nullptr;
			}
		}
	}
};


static void GetSockType(GSocket::Protocol protocol, int* type, int* iProtocol)
{
	switch (protocol)
	{
	case GSocket::SocketTcp:
		*type = SOCK_STREAM;
		*iProtocol = IPPROTO_TCP;
		break;
	case GSocket::SocketUdp:
		*type = SOCK_DGRAM;
		*iProtocol = IPPROTO_UDP;
		break;
	default:
		*type = 0;
		*iProtocol = 0;
		break;
	}
}

static int GetIPFamily(GSocket::IPType type)
{
	switch (type)
	{
	case GSocket::IPv4:
		return AF_INET;
	case GSocket::IPv6:
		return AF_INET6;
	default:
		break;
	}
	return 0;
}

bool GSocket::QueryDns(const QueryDnsCallback& callback, const GUrlParts& url, DWORD timeout /*= 5000*/, Protocol protocol /*= SocketTcp*/)
{
	static DNSCacheInit dnsCacheInit;
	wchar_t host[4096] = {};
	wchar_t protocolStr[256] = {};
	std::copy(url.getHost().begin(), url.getHost().end(), host);
	std::copy(url.getProtocol().begin(), url.getProtocol().end(), protocolStr);
	{
		wchar_t* pTmp = wcsrchr(protocolStr, ':');
		if (pTmp)
			*pTmp = 0;
	}

	ADDRINFOEXW addrinfoex = {};
	addrinfoex.ai_family = AF_UNSPEC;
	switch (protocol)
	{
	case GSocket::SocketTcp:
		addrinfoex.ai_protocol = IPPROTO_TCP;
		break;
	case GSocket::SocketUdp:
		addrinfoex.ai_protocol = IPPROTO_UDP;
		break;
	default:
		break;
	}

	timeval tval = {};
	tval.tv_usec = timeout;

	GAsyncIO* pIO = GNewIO();
	pIO->addressList = GAddressInfoList();
	pIO->ioType = GAsyncIO::QueryDns;
	pIO->queryDnsCallback = callback;
	int result = GetAddrInfoExW(
		host, 
		url.getProtocol().isEmptyOrBlank()? nullptr : protocolStr,
		NS_DNS,
		s_dnsCacheInfo? &s_dnsCacheInfo[0].NSProviderId : nullptr,
		&addrinfoex,
		pIO->addressList.addressArray,
		&tval,
		&pIO->overlapped,
		nullptr,
		nullptr
		);
	if (result != 0 && WSAGetLastError() != ERROR_IO_PENDING)
	{
		GFreeIO(pIO);
		return false;
	}

	return true;
}

GSocket::GSocket(const GString& host, int port /*= -1*/, Protocol protocol /*= SocketTcp*/, IPType ipType /*= IPv4*/)
	:m_ipType(ipType), m_host(host), m_port(port), m_protocol(protocol)
{
	GUrlParts urlParts(host);
	if (m_port == -1)
		m_port = urlParts.getPort();
	m_host = urlParts.getHost().toString();
}

bool GSocket::open(bool async /*= false*/)
{
	int type, iProtocol;
	GetSockType(m_protocol, &type, &iProtocol);

	m_socket = socket(GetIPFamily(m_ipType), type, iProtocol);

	if (m_socket == INVALID_SOCKET)
	{
		m_socket = NULL;
		return false;
	}

	static MSSockInit msSockInit = MSSockInit(m_socket);
	if (async)
	{
		unsigned long val = 1;
		ioctlsocket(m_socket, FIONBIO, &val);
		m_async = true;
	}

	//s_ConnectEx();
}

void GSocket::close()
{
	if (m_socket)
	{
		closesocket(m_socket);
		m_socket = NULL;
	}
}

bool GSocket::isClosed() const
{
	return m_socket != NULL || getOptionInt(SO_CONNECT_TIME) == SOCKET_ERROR;
}

GDevice::IOResult GSocket::write(const void* pData, size_t size)
{
	if (isClosed())
		return GDevice::IO_NotOpen;
	if (m_async)
	{
		
	}
	return GDevice::IO_Success;
}

GDevice::IOResult GSocket::read(void* pData, size_t size, size_t* readsize /*= nullptr*/)
{
	if (isClosed())
		return GDevice::IO_NotOpen;


	return GDevice::IO_Success;
}

void GSocket::flush()
{
	
}

bool GSocket::isAsync() const
{
	return m_async;
}

void* GSocket::getNativeHandle() const
{
	return (void*)m_socket;
}

int GSocket::getOptionInt(int flag) const
{
	if (isClosed())
		return 0;
	int opt = 0;
	int size = sizeof(opt);
	getsockopt(m_socket, SOL_SOCKET, flag, (char*)&opt, &size);
	return opt;
}

void GSocket::setOptionInt(int flag, int value)
{
	if (isClosed())
		return;
	int opt = 0;
	int optlen = sizeof(opt);
	setsockopt(m_socket, SOL_SOCKET, flag, (const char*)&opt, optlen);
}
