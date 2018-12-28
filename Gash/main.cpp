#include "base/GPath.h"
#include <conio.h>
#include "base/GUrlParts.h"
#include <unordered_map>
#include "base/reflection/GUserTypeRegister.h"
#include "base/GWin32BaseTypeRegistry.h"
#include "base/GValue.h"
#include "base/GValueConverter.h"
#include "base/reflection/GField.h"
#include "base/reflection/GFunction.h"
#include "base/reflection/GFunctionTraits.h"
#include "base/reflection/GClassRegistry.h"
#include "base/GShell.h"
#include "base/GRegex.h"
#include "base/stream/GStream.h"
#include "base/stream/GFileStream.h"
#include "base/io/GBufferIO.h"
#include "base/stream/GBufferStream.h"
#include "base/stream/GIOStream.h"
#include "base/stream/GEncodingTransform.h"

#include "base/GDepsIncludes.h"
#include "base/charset/GEncoding.h"
#include "base/algorithm/GEncoder.h"
#include "base/stream/GEncodeStream.h"
#include "base/GStrings.h"
#include "base/schedule/GSchedulerManager.h"
#include "base/schedule/GSchedulerCondition.h"
#include "base/schedule/condition/GConditionInterval.h"
#include "base/schedule/condition/GConditionTimeout.h"
#include "base/schedule/condition/GConditionOvercount.h"
#include "base/schedule/condition/GConditionNot.h"
#include "base/schedule/condition/GConditionCancelable.h"
#include "base/schedule/condition/GConditionOr.h"
#include "base/schedule/condition/GConditionCallback.h"
#include "base/schedule/GTaskCallback.h"
#include "base/async/GAsyncLoop.h"
#include "base/stream/GBufferStream.h"

#include "ares.h"
#include "base/device/GFileDevice.h"
#include "base/io/GNSLookup.h"
#include "base/GErrno.h"
#include "base/io/sync/GSocketSync.h"
#include "base/io/GFile.h"
#include "base/stream/GGUncompressStream.h"
#include "base/io/sync/GTLSSocketSync.h"
#include "base/stream/GTransformStream.h"
#include "base/stream/GUnpackStream.h"
#include "base/stream/GHttpParserStream.h"

const char buf[] = "GET / HTTP/1.1\r\n\r\n";

ares_socket_functions functions;

typedef std::function<void()> Callback;

struct OVERLAPPED_EXT
{
	OVERLAPPED overlapped;
	Callback callback;
};

struct DNSQuery
{
	HANDLE hIOCP;
	ares_channel channel;
	ares_socket_t readSocket;
	ares_socket_t writeSocket;
	int result = 0;
};

struct iovec
{
	void *iov_base;
	size_t iov_len;
};

void InitSocketFunctions(ares_channel channel, HANDLE hIOCP)
{
	functions.asocket = [](int family, int type, int protocol, void *)->ares_socket_t {
		return socket(AF_INET, type, protocol);
	};
	functions.aclose = [](ares_socket_t s, void*) {
		return closesocket(s);
	};
	functions.aconnect = [](ares_socket_t s, const struct sockaddr * addr, ares_socklen_t len, void * pData) {
		DNSQuery* pQuery = (DNSQuery*)pData;
		int result = connect(s, addr, len);
		u_long bio = 1;
		ioctlsocket(s, FIONBIO, &bio);
		HANDLE hResult = CreateIoCompletionPort((HANDLE)s, pQuery->hIOCP, (ULONG_PTR)pQuery, 0);
		return result;
	};

	functions.asendv = [](ares_socket_t s, const struct iovec* bufs, int size, void* pData)->ares_ssize_t {

		DNSQuery* pQuery = (DNSQuery*)pData;
		pQuery->readSocket = s;
		pQuery->writeSocket = s;

		WSABUF buf;
		buf.buf = (char*)bufs[0].iov_base;
		buf.len = bufs[0].iov_len;
		OVERLAPPED_EXT* pOl = new OVERLAPPED_EXT();
		pOl->callback = [pQuery]() {
			ares_process_fd(pQuery->channel, pQuery->readSocket, pQuery->writeSocket);
		};
		DWORD result = 0;
		if (WSASend(s, &buf, 1, nullptr, 0, &pOl->overlapped, nullptr) != 0 && (result = WSAGetLastError()) != ERROR_IO_PENDING)
			return -1;
		return pQuery->result;
	};

	functions.arecvfrom = [](ares_socket_t s, void * pOut, size_t len, int, struct sockaddr * addr, ares_socklen_t * socklen, void * pData)->ares_ssize_t {
		DNSQuery* pQuery = (DNSQuery*)pData;
		pQuery->writeSocket = s;

		OVERLAPPED_EXT* pOl = new OVERLAPPED_EXT();
		pOl->callback = [pQuery]() {
			ares_process_fd(pQuery->channel, pQuery->readSocket, pQuery->writeSocket);
		};
		WSABUF buf;
		buf.buf = (char*)pOut;
		buf.len = len;
		DWORD flags = 0;
		DWORD errCode = 0;
		if (WSARecvFrom(s, &buf, 1, nullptr, &flags, addr, socklen, &pOl->overlapped, nullptr) != 0 && (errCode = WSAGetLastError()) != ERROR_IO_PENDING)
		{
			char buf[1024];
			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errCode, LANG_USER_DEFAULT, buf, sizeof(buf), nullptr);
			return -1;
		}
		return pQuery->result;
	};
	DNSQuery* pQuery = new DNSQuery();
	memset(pQuery, 0, sizeof(DNSQuery));
	pQuery->channel = channel;
	pQuery->hIOCP = hIOCP;
	ares_set_socket_functions(channel, &functions, pQuery);
}

const char req[] = "GET / HTTP/1.1\r\n"
"Host: www.baidu.com\r\n"
"Connection: keep-alive\r\n"
"Pragma: no-cache\r\n"
"Cache-Control: no-cache\r\n"
"Upgrade-Insecure-Requests: 1\r\n"
"User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.67 Safari/537.36\r\n"
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
"Accept-Encoding: gzip, deflate, br\r\n"
"Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n\r\n";

int main()
{
	// 	Win32BaseTypeRegister();

// 	WSADATA wsadata = {};
// 	WSAStartup(MAKEWORD(2, 2), &wsadata);
// 
// 	HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
// 
// 	ares_library_init(ARES_LIB_INIT_ALL);
// 
// 	ares_channel channel;
// 
// 	in_addr addr[2];;
// 	ares_inet_pton(AF_INET, "223.5.5.5", &addr);
// 	ares_inet_pton(AF_INET, "223.6.6.6", &addr);
// 	ares_options options = {};
// 	options.servers = addr;
// 	options.nservers = 2;
// 
// 	ares_init_options(&channel, &options, ARES_OPT_SERVERS);
// 	InitSocketFunctions(channel, hIOCP);
// 
// 	sockaddr_in dnsServer = {};
// 	ares_gethostbyname(channel, "www.baidu.com", AF_INET, [](void *arg,
// 		int status,
// 		int timeouts,
// 		struct hostent *hostent) {
// 		if (status != ARES_SUCCESS)
// 		{
// 			printf("%s\n", ares_strerror(status));
// 			return;
// 		}
// 
// 		char* addr;
// 		size_t i = 0;
// 		while ((addr = hostent->h_addr_list[i++]) != nullptr)
// 		{
// 			char buf[64] = {};
// 			printf("%s\n", ares_inet_ntop(AF_INET, (void*)addr, buf, 64));
// 		}
// 	}, nullptr);
// 
// 	while (1) {
// 		DWORD numTransfer = 0;
// 		ULONG_PTR key = 0;
// 		LPOVERLAPPED pOverlapped;
// 		GetQueuedCompletionStatus(hIOCP, &numTransfer, &key, &pOverlapped, INFINITE);
// 		OVERLAPPED_EXT* pExt = CONTAINING_RECORD(pOverlapped, OVERLAPPED_EXT, overlapped);
// 		DNSQuery* pQuery = (DNSQuery*)key;
// 		pQuery->result = numTransfer;
// 		if (pExt->callback)
// 			pExt->callback();
// 		pQuery->result = 0;
// 		Sleep(500);
// 	}
// 	auto t = std::allocator<int>();
// 
// 	ares_library_cleanup();
// 	WSACleanup();

	GTLSSocketSync socket;

	char buf[4096] = {};
	int readsize;
	bool result = socket.connect(L"www.baidu.com", 443);
	readsize = socket.write(req, sizeof(req) - 1);

	GHttpParserStreamPtr httpParser = GHttpParserStream::Create();

	httpParser->setStatusLineCallback([](const char* version, int statusCode, const char* statusMessage) {
		printf("%s %d %s\n", version, statusCode, statusMessage);
	});

	httpParser->setHeaderCallback([](const char* key, const char* value) {
		printf("%s: %s\n", key, value);
	});

	httpParser->setCookieCallback([](const char* key, const char* value) {
		printf("%s: %s\n", key, value);
	});

	httpParser->setBodyCallback([](const void* data, size_t size) {
		std::string str = GString::FromEncoding((char*)data, size, EncodingUtf8).toAnsi();
		printf("%s\n", str.data());
	});


	while ((readsize = socket.read(buf, sizeof(buf))) > 0)
		httpParser->write(buf, readsize);
	httpParser->flush();

	_getch();
	return 0;
}