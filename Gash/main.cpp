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
#include "base/async/fileio/GAsyncFile.h"
#include "base/async/fileio/GAsyncTCPSocket.h"
#include "base/stream/GBufferStream.h"

#include "ares.h"

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

int main()
{
	// 	Win32BaseTypeRegister();
	// 
	// 	GAsyncTCPSocketPtr pSocket = GAsyncTCPSocket::CreatePooled(L"http://www.baidu.com");
	// 
	// 	if (!GAsyncLoop::GetDefaultEngine()->addDevice(pSocket))
	// 		printf("关联失败\n");
	// 
	// 	if (!pSocket->open([](GAsyncDevicePtr pDevice){
	// 		GAsyncTCPSocketPtr pSocket = pDevice;
	// 		if (!pSocket->write(buf, sizeof(buf) - 1, [](GAsyncDevicePtr pDevice) {
	// 			GAsyncTCPSocketPtr pSocket = pDevice;
	// 			auto cb = [pSocket](GAsyncDevicePtr pDevice, const void* pData, size_t readSize) {
	// 				GString str = GString::FromEncoding((char*)pData, readSize, GEncoding::EncodingUtf8);
	// 				printf("%s\n", str.toAnsi().data());
	// 			};
	// 			pSocket->read(0x4000, cb);
	// 			pSocket->read(0x4000, cb);
	// 			pSocket->read(0x4000, cb);
	// 			pSocket->read(0x4000, cb);
	// 			pSocket->read(0x4000, cb);
	// 			pSocket->read(0x4000, cb);
	// 			pSocket->read(0x4000, cb);
	// 			pSocket->read(0x4000, cb);
	// 			pSocket->read(0x4000, cb);
	// 			pSocket->read(0x4000, cb);
	// 			pSocket->read(0x4000, cb);
	// 			pSocket->read(0x4000, cb);
	// 		}))
	// 		{
	// 			printf("写出数据失败");
	// 			return;
	// 		}
	// 	}))
	// 	{
	// 		_getch();
	// 		return -1;
	// 	}
	// 
	// 	GAsyncLoop::GetDefault()->loop();

	WSADATA wsadata = {};
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);

	ares_library_init(ARES_LIB_INIT_ALL);

	ares_channel channel;

	in_addr addr[2];;
	ares_inet_pton(AF_INET, "223.5.5.5", &addr);
	ares_inet_pton(AF_INET, "223.6.6.6", &addr);
	ares_options options = {};
	options.servers = addr;
	options.nservers = 2;

	ares_init_options(&channel, &options, ARES_OPT_SERVERS);
	InitSocketFunctions(channel, hIOCP);

	sockaddr_in dnsServer = {};
	ares_gethostbyname(channel, "www.baidu.com", AF_INET, [](void *arg,
		int status,
		int timeouts,
		struct hostent *hostent) {
		if (status != ARES_SUCCESS)
		{
			printf("%s\n", ares_strerror(status));
			return;
		}

		char* addr;
		size_t i = 0;
		while ((addr = hostent->h_addr_list[i++]) != nullptr)
		{
			char buf[64] = {};
			printf("%s\n", ares_inet_ntop(AF_INET, (void*)addr, buf, 64));
		}
	}, nullptr);

	while (1) {
		DWORD numTransfer = 0;
		ULONG_PTR key = 0;
		LPOVERLAPPED pOverlapped;
		GetQueuedCompletionStatus(hIOCP, &numTransfer, &key, &pOverlapped, INFINITE);
		OVERLAPPED_EXT* pExt = CONTAINING_RECORD(pOverlapped, OVERLAPPED_EXT, overlapped);
		DNSQuery* pQuery = (DNSQuery*)key;
		pQuery->result = numTransfer;
		if (pExt->callback)
			pExt->callback();
		pQuery->result = 0;
		Sleep(500);
	}
	auto t = std::allocator<int>();

	ares_library_cleanup();
	WSACleanup();
	_getch();
	return 0;
}