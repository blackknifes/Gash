#include "GNSLookup.h"
#include "ares.h"
#include "../GErrno.h"
#include "../net/GNetEngine.h"

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
	std::vector<GAddrInfo> addrs;
	fd_set readFdSet;
	fd_set writeFdSet;
	timeval timeout = {};
	timeout.tv_sec = 5;
	ares_timeout((ares_channel)m_pContext, NULL, &timeout);

	ares_gethostbyname((ares_channel)m_pContext, ns.toAnsi().data(), AF_INET, [](
		void *arg,
		int status,
		int timeouts,
		struct hostent *hostent) {
		if (status != ARES_SUCCESS)
		{
			GSetErrno(status);
			return;
		}
		std::vector<GAddrInfo>& addrs = *(std::vector<GAddrInfo>*)arg;
		char* addrRecvied;
		size_t i = 0;
		while ((addrRecvied = hostent->h_addr_list[i++]) != nullptr)
		{
			GAddrInfo addr = {};
			addr.addr.sa_family = AF_INET;
			addr.addr4.sin_addr = *(in_addr*)addrRecvied;
			addrs.push_back(addr);
		}
	}, &addrs);

	while (true)
	{
		int nfds;
		FD_ZERO(&readFdSet);
		FD_ZERO(&writeFdSet);
		if((nfds = ares_fds((ares_channel)m_pContext, &readFdSet, &writeFdSet)) == 0)
			break;
		select(nfds, &readFdSet, &writeFdSet, NULL, &timeout);   //将ares的SOCKET FD 加入事件循环  
		ares_process((ares_channel)m_pContext, &readFdSet, &writeFdSet);
	}

	if (port != 0)
	{
		for (auto itor = addrs.begin(); itor != addrs.end(); ++itor)
			(*itor).addr4.sin_port = htons(port);
	}

	return std::move(addrs);
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