#include "GAsyncIO.h"

static GObjectPool<GAsyncIO> m_objectPool;
static std::atomic<unsigned __int64> s_pendingIOCount = 0;



GAddressInfoList::GAddressInfoList()
{
	for (size_t i = 0; i < sizeof(addresses) / sizeof(ADDRINFOEXW); ++i)
		addressArray[i] = addresses + i;
}

GAsyncIO* GNewIO()
{
	++s_pendingIOCount;
	GAsyncIO* pIO = m_objectPool.pop();
	memset(&pIO->overlapped, 0, sizeof(OVERLAPPED));
	pIO->startTime = GetTickCount64();
	return pIO;
}

void GFreeIO(GAsyncIO* pIO)
{
	--s_pendingIOCount;
	switch (pIO->ioType)
	{
	case GAsyncIO::IORead:
		pIO->readCallback = nullptr;
		break;
	case GAsyncIO::QueryDns:
		pIO->queryDnsCallback = nullptr;
		break;
	case GAsyncIO::IOWrite:
	case GAsyncIO::NetConnect:
	case GAsyncIO::NetDisconnect:
		pIO->writeCallback = nullptr;
		break;
	default:
		break;
	}
	pIO->pObject = nullptr;
	memset(pIO, 0, sizeof(GAsyncIO));
	m_objectPool.push(pIO);
}

bool GHasPendingIO()
{
	return s_pendingIOCount != 0;
}
