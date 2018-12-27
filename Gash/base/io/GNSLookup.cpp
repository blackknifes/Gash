#include "GNSLookup.h"
#include "ares.h"
#include "../GErrno.h"
#include "../net/GNetEngine.h"


static std::vector<GAddrInfo> QueryDns(ares_channel pContext, int family, const GString& ns, int port)
{
	typedef struct
	{
		std::vector<GAddrInfo>* addrs;
		int family;
		int port;
	} QueryContext;


	std::vector<GAddrInfo> addrs;
	fd_set readFdSet;
	fd_set writeFdSet;
	timeval timeout = {};
	timeout.tv_sec = 5;
	ares_timeout(pContext, NULL, &timeout);


	QueryContext queryContext;
	queryContext.addrs = &addrs;
	queryContext.family = family;
	queryContext.port = port;

	ares_gethostbyname(pContext, ns.toAnsi().data(), family, [](
		void *arg,
		int status,
		int timeouts,
		struct hostent *hostent) {
		if (status != ARES_SUCCESS)
		{
			GSetErrno(status);
			return;
		}
		QueryContext& context = *(QueryContext*)arg;
		char* addrRecvied;
		size_t i = 0;
		while ((addrRecvied = hostent->h_addr_list[i++]) != nullptr)
		{
			GAddrInfo addr;
			memset(&addr, 0, sizeof(addr));
			addr.addr.sa_family = context.family;
			switch (context.family)
			{
			case AF_INET:
				addr.addr4.sin_addr = *(in_addr*)addrRecvied;
				addr.addr4.sin_port = htons(context.port);
				break;
			case AF_INET6:
				addr.addr6.sin6_addr = *(in6_addr*)addrRecvied;
				addr.addr6.sin6_port = htons(context.port);
				break;
			default:
				break;
			}
			context.addrs->push_back(addr);
		}
	}, &queryContext);

	while (true)
	{
		int nfds;
		FD_ZERO(&readFdSet);
		FD_ZERO(&writeFdSet);
		if ((nfds = ares_fds(pContext, &readFdSet, &writeFdSet)) == 0)
			break;
		select(nfds, &readFdSet, &writeFdSet, NULL, &timeout);   //将ares的SOCKET FD 加入事件循环  
		ares_process(pContext, &readFdSet, &writeFdSet);
	}

	return std::move(addrs);
}

GNSLookup::GNSLookup()
{
	GNetEngine::init();
	ares_library_init(ARES_LIB_INIT_ALL);
	ares_init((ares_channel*)&m_pContext);
}

GNSLookup::~GNSLookup()
{
	ares_destroy((ares_channel)m_pContext);
}

std::vector<GAddrInfo> GNSLookup::lookupV4(const GString& ns, int port /*= 0*/)
{
	return QueryDns((ares_channel)m_pContext, AF_INET, ns, port);
}

std::vector<GAddrInfo> GNSLookup::lookupV6(const GString& ns, int port /*= 0*/)
{
	return QueryDns((ares_channel)m_pContext, AF_INET6, ns, port);
}

bool GNSLookup::getHostByNameV4(GAddrInfo* addr, int* addrlen, const GString& host, int port)
{
	GAddrInfo& addrInfo = *addr;
	if (host.isIpV4())
	{
		addrInfo.addr4.sin_family = AF_INET;
		addrInfo.addr4.sin_port = htons(port);
		ares_inet_pton(AF_INET, host.toAnsi().data(), &addrInfo.addr4.sin_addr);
		*addrlen = sizeof(addrInfo.addr4);
	}
	else if (host.isIpV6())
		return false;
	else
	{
		auto addrs = GNSLookupV4(host, port);
		if (addrs.empty())
			return false;
		addrInfo = addrs.front();
		*addrlen = sizeof(addrInfo.addr4);
	}
	return true;
}

GString GNSLookup::toString(GAddrInfo* addr) const
{
	char buf[128] = {};
	switch (addr->addr.sa_family)
	{
	case AF_INET:
		ares_inet_ntop(AF_INET, &addr->addr4.sin_addr, buf, sizeof(buf));
		break;
	case AF_INET6:
		ares_inet_ntop(AF_INET6, &addr->addr6.sin6_addr, buf, sizeof(buf));
		break;
	default:
		break;
	};
	return GString::FromEncoding(buf);
}

const char* GNSLookup::getErrorMessage(int err)
{
	return ares_strerror(err);
}

std::vector<GAddrInfo> GNSLookupV4(const GString& ns, int port /*= 0*/)
{
	return GNSLookup::getInstance()->lookupV4(ns, port);
}

bool GGetHostByNameV4(GAddrInfo* addr, int* addrlen, const GString& host, int port)
{
	return GNSLookup::getInstance()->getHostByNameV4(addr, addrlen, host, port);
}
