#ifndef __GSOCKETPOOL_H__
#define __GSOCKETPOOL_H__
#include "../GSingleton.h"
#include <WinSock2.h>
#include "../block_container/GBlockingStack.h"
#include <MSWSock.h>
#include "GIOEnums.h"

class GSocketPool: public GSingleton<GSocketPool>
{
public:

	static SOCKET PopSocket(GProtocol protocol = GProtocol::Protocol_TCP);
	static void PushSocket(SOCKET s);

	static LPFN_ACCEPTEX getAcceptEx();
	static LPFN_CONNECTEX getConnectEx();
	static LPFN_DISCONNECTEX getDisConnectEx();
	static LPFN_GETACCEPTEXSOCKADDRS getGetAcceptExSockAddrsEx();

	GSocketPool();
	~GSocketPool();
	SOCKET pop(GProtocol protocol = GProtocol::Protocol_TCP);
	void push(SOCKET s);

private:
	GBlockingStack<SOCKET> m_tcpSocketPool;
	GBlockingStack<SOCKET> m_udpSocketPool;
	LPFN_ACCEPTEX m_pAcceptEx;
	LPFN_CONNECTEX m_pConnectEx;
	LPFN_DISCONNECTEX m_pDisConnectEx;
	LPFN_GETACCEPTEXSOCKADDRS m_pGetAcceptExSockAddrs;
	
};
#endif