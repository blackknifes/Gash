#ifndef __GDNSQUERY_H__
#define __GDNSQUERY_H__
#include "../GObject.h"
#include "../block_container/GBlockingQueue.h"
#include "../GSingleton.h"
#include <WinSock2.h>
#include <ws2ipdef.h>

struct GAddrInfo
{
	union
	{
		sockaddr addr;
		sockaddr_in addr4;
		sockaddr_in6 addr6;
	};
};

class GNSLookup: public GObject, public GSingleton<GNSLookup>
{
public:
	GNSLookup();
	~GNSLookup();

	std::vector<GAddrInfo> lookupV4(const GString& ns, int port = 0);
	std::vector<GAddrInfo> lookupV6(const GString& ns, int port = 0);
	bool getHostByNameV4(GAddrInfo* addr, int* addrlen, const GString& host, int port);

	GString toString(GAddrInfo* addr) const;

	const char* getErrorMessage(int err);
private:
	GObjectClassPooled(GNSLookup);
	void* m_pContext;
};

std::vector<GAddrInfo> GNSLookupV4(const GString& ns, int port = 0);
bool GGetHostByNameV4(GAddrInfo* addr, int* addrlen, const GString& host, int port);
#endif