#include "GTLSSocketSync.h"
#include "mbedtls/net.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/debug.h"
#include <WinSock2.h>
#include "../GSocketPool.h"
#include "../GNSLookup.h"

typedef struct
{
	mbedtls_net_context connection;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context crtdbg;
	mbedtls_ssl_context ssl_context;
	mbedtls_ssl_config config;
	mbedtls_x509_crt x509;
} GTLSSocketSyncContext;

static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
	((void)level);
	fprintf((FILE *)ctx, "%s:%04d: %s", file, line, str);
	fflush((FILE *)ctx);
}

static bool InitTLSSocket(GTLSSocketSyncContext* pContext, GProtocol protocol = Protocol_TCP)
{
	SOCKET s = GSocketPool::PopSocket(protocol);
	u_long nbio = 0;
	ioctlsocket(s, FIONBIO, &nbio);
	DWORD timeout = 3000;
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
	setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

	pContext->connection.fd = s;
	mbedtls_x509_crt_init(&pContext->x509);
	mbedtls_ctr_drbg_init(&pContext->crtdbg);
	mbedtls_entropy_init(&pContext->entropy);
	mbedtls_ssl_init(&pContext->ssl_context);
	mbedtls_ssl_config_init(&pContext->config);
	int result = mbedtls_ctr_drbg_seed(&pContext->crtdbg, mbedtls_entropy_func, &pContext->entropy, nullptr, 0);
	if (result == 0)
	{
		result = mbedtls_ssl_config_defaults(
			&pContext->config,
			MBEDTLS_SSL_IS_CLIENT,
			(protocol == Protocol_TCP) ? MBEDTLS_SSL_TRANSPORT_STREAM : MBEDTLS_SSL_TRANSPORT_DATAGRAM,
			MBEDTLS_SSL_PRESET_DEFAULT);
	}


	if (result == 0)
	{
		mbedtls_ssl_conf_authmode(&pContext->config, MBEDTLS_SSL_VERIFY_NONE);
		mbedtls_ssl_conf_rng(&pContext->config, mbedtls_ctr_drbg_random, &pContext->crtdbg);
		mbedtls_ssl_conf_dbg(&pContext->config, my_debug, stdout);
		result = mbedtls_ssl_setup(&pContext->ssl_context, &pContext->config);
	}

	if (result != 0)
	{
		mbedtls_net_free(&pContext->connection);
		mbedtls_ssl_config_free(&pContext->config);
		mbedtls_ctr_drbg_free(&pContext->crtdbg);
		mbedtls_entropy_free(&pContext->entropy);
		GSocketPool::PushSocket(s);
		return false;
	}
	mbedtls_ssl_set_bio(&pContext->ssl_context, &pContext->connection, mbedtls_net_send, mbedtls_net_recv, NULL);
	
	return true;
}

GTLSSocketSync::GTLSSocketSync(GProtocol protocol /*= Protocol_TCP*/)
{
	GTLSSocketSyncContext* pContext = GNewObject<GTLSSocketSyncContext>();
	memset(pContext, 0, sizeof(GTLSSocketSyncContext));
	if (InitTLSSocket(pContext))
		m_pTLSSocket = pContext;
	else
	{
		GDestroyObject(pContext);
		pContext = nullptr;
	}
}

GTLSSocketSync::~GTLSSocketSync()
{
	GTLSSocketSyncContext* pContext = (GTLSSocketSyncContext*)m_pTLSSocket;

	if (pContext)
	{
		mbedtls_net_free(&pContext->connection);
		mbedtls_ssl_free(&pContext->ssl_context);
		mbedtls_ssl_config_free(&pContext->config);
		mbedtls_ctr_drbg_free(&pContext->crtdbg);
		mbedtls_entropy_free(&pContext->entropy);

		GSocketPool::PushSocket(pContext->connection.fd);
		GDestroyObject((GTLSSocketSyncContext*)m_pTLSSocket);
	}
}

int GTLSSocketSync::read(void* pData, size_t size)
{
	GTLSSocketSyncContext* pContext = (GTLSSocketSyncContext*)m_pTLSSocket;
	return mbedtls_ssl_read(&pContext->ssl_context, (unsigned char*)pData, size);
}

int GTLSSocketSync::write(const void* pData, size_t size)
{
	GTLSSocketSyncContext* pContext = (GTLSSocketSyncContext*)m_pTLSSocket;
	size_t tmp = mbedtls_ssl_get_max_frag_len(&pContext->ssl_context);
	return mbedtls_ssl_write(&pContext->ssl_context, (const unsigned char*)pData, size);
}

void GTLSSocketSync::flush()
{
	
}

bool GTLSSocketSync::isClosed() const
{
	DWORD val;
	int len = sizeof(val);
	getsockopt((SOCKET)getNativeHandle(), SOL_SOCKET, SO_CONNECT_TIME, (char*)&val, &len);
	return val == SOCKET_ERROR;
}

void GTLSSocketSync::close()
{
	disconnect();
}

void* GTLSSocketSync::getNativeHandle() const
{
	GTLSSocketSyncContext* pContext = (GTLSSocketSyncContext*)m_pTLSSocket;
	return (void*)pContext->connection.fd;
}

bool GTLSSocketSync::connect(const GString& host, int port /*= 443*/)
{
	if (!isClosed())
		return false;
	GAddrInfo addrInfo = {};
	int addrlen;
	if (!GGetHostByNameV4(&addrInfo, &addrlen, host, port))
		return false;

	GTLSSocketSyncContext* pContext = (GTLSSocketSyncContext*)m_pTLSSocket;
	if (mbedtls_ssl_set_hostname(&pContext->ssl_context, host.toAnsi().data()) != 0)
		return false;
	if (::connect((SOCKET)getNativeHandle(), &addrInfo.addr, addrlen) != 0)
		return false;
	return mbedtls_ssl_handshake(&pContext->ssl_context) == 0;
}

void GTLSSocketSync::disconnect()
{
	GTLSSocketSyncContext* pContext = (GTLSSocketSyncContext*)m_pTLSSocket;
	GSocketPool::getDisConnectEx()((SOCKET)getNativeHandle(), nullptr, TF_REUSE_SOCKET, 0);
}
