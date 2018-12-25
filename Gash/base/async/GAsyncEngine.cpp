#include "GAsyncEngine.h"
#include <MSWSock.h>

#define ASYNC_BUFFER_SIZE 0x4000
#define ALIGN_BUFFER(val) (((val) + (ASYNC_BUFFER_SIZE) - 1) & ~((ASYNC_BUFFER_SIZE) - 1))

typedef BOOL(WINAPI* PFN_GetQueuedCompletionStatusEx)(
	_In_ HANDLE CompletionPort,
	_Out_writes_to_(ulCount, *ulNumEntriesRemoved) LPOVERLAPPED_ENTRY lpCompletionPortEntries,
	_In_ ULONG ulCount,
	_Out_ PULONG ulNumEntriesRemoved,
	_In_ DWORD dwMilliseconds,
	_In_ BOOL fAlertable);

static PFN_GetQueuedCompletionStatusEx g_GetQueuedCompletionStatusEx = nullptr;


GAsyncEngine::GAsyncEngine(DWORD numThread /*= 0*/)
	:m_dwNumOfThreads(numThread)
{
	if (g_GetQueuedCompletionStatusEx == nullptr)
	{
		g_GetQueuedCompletionStatusEx = 
			(PFN_GetQueuedCompletionStatusEx)GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetQueuedCompletionStatusEx");
	}
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, numThread);
}

GAsyncEngine::~GAsyncEngine()
{
	if (m_hIOCP)
		::CloseHandle(m_hIOCP);
}

bool GAsyncEngine::isValid() const
{
	return m_hIOCP != nullptr;
}

bool GAsyncEngine::addDevice(GIODevicePtr pDevice)
{
	return CreateIoCompletionPort((HANDLE)pDevice->getNativeHandle(), m_hIOCP, (ULONG_PTR)pDevice.get(), 0) != NULL;
}

bool GAsyncEngine::getIOTask(GIODevice** pDevice, GAsyncIO** pIO, DWORD overtime /*= INFINITE*/)
{
	DWORD dwTransferSize;
	ULONG_PTR key;
	LPOVERLAPPED pOverlapped;
	if (GetQueuedCompletionStatus(m_hIOCP, &dwTransferSize, &key, &pOverlapped, overtime) == FALSE)
		return false;
	GAsyncIO::OnComplete(pOverlapped, dwTransferSize, key);

	return true;
}

size_t GAsyncEngine::getIOTaskList(GIODevice** pDeviceList, GAsyncIO** pTaskList, size_t size, DWORD overtime /*= INFINITE*/)
{
	if (g_GetQueuedCompletionStatusEx == nullptr)
	{
		GIODevice* pDevice = nullptr;
		GAsyncIO* pIO;
		if (!getIOTask(&pDevice, &pIO, overtime))
			return 0;
		
		pTaskList[0] = pIO;
		return 1;
	}
	if (size > 32)
		size = 32;
	ULONG sizeOfEntry = size;
	OVERLAPPED_ENTRY entries[32];
	if(g_GetQueuedCompletionStatusEx(m_hIOCP, entries, sizeOfEntry, &sizeOfEntry, overtime, FALSE) == FALSE)
		return 0;
	size = (size_t)sizeOfEntry;
	for (size_t i = 0; i < size; ++i)
		GAsyncIO::OnComplete(entries[i].lpOverlapped, entries[i].dwNumberOfBytesTransferred, entries[i].lpCompletionKey);
	return size;
}

void GAsyncEngine::doAsyncIO(GIODevicePtr pDevice, GAsyncIO* pIO)
{
	
}

DWORD GAsyncEngine::getMaxThreadCount() const
{
	return m_dwNumOfThreads;
}