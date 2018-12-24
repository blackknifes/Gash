#ifndef __GASYNCIO_H__
#define __GASYNCIO_H__
#include <Windows.h>
#include <functional>
#include "../../GObjectPool.h"
#include "../../GObject.h"
#include <ws2def.h>

GREFPTR_DEF(GAsyncDevice);

struct GAddressInfoList
{
	GAddressInfoList();
	ADDRINFOEXW addresses[40];			//dns��ѯ����б�
	ADDRINFOEXW* addressArray[40];
};

struct GAsyncIO
{
	typedef std::function<void(GAsyncDevicePtr pDevice)> WriteCallback;
	typedef std::function<void(GAsyncDevicePtr pDevice, const void* pData, size_t size)> ReadCallback;
	typedef std::function<void(GAsyncDevicePtr pDevice)> OpenCallback;
	typedef std::function<void(GAsyncDevicePtr pDevice)> CloseCallback;
	typedef std::function<void(GObjectPtr pObject, const GAddressInfoList& addressList)> QueryDnsCallback;

	static GAsyncIO* GetAsyncIO(OVERLAPPED* pOverlapped, DWORD numTransferSize)
	{
		GAsyncIO* pIO = CONTAINING_RECORD(pOverlapped, GAsyncIO, overlapped);
		pIO->dwTransferSize = numTransferSize;
		return pIO;
	}

	enum IOType
	{
		IORead,
		IOWrite,
		QueryDns,
		NetConnect,
		NetDisconnect
	};

	GAsyncIO(): writeCallback(nullptr) {};
	~GAsyncIO() {};

	OVERLAPPED overlapped;				//�ص�io�ṹ
	IOType ioType;						//io����
	DWORD dwTransferSize;				//����io��С
	DWORD dwExpectSize;					//������С
	unsigned __int64 startTime;			//��ʼʱ��
	GObjectPtr pObject;					//���ö���

	union
	{
		WriteCallback writeCallback;	//д���ص�
		ReadCallback readCallback;		//����ص�
		OpenCallback openCallback;
		CloseCallback closeCallback;
		QueryDnsCallback queryDnsCallback;
	};

	union
	{
		unsigned char buffer[0x4000];		//����
		GAddressInfoList addressList;		//dns��ѯ����б�
	};
};

GAsyncIO* GNewIO();
void GFreeIO(GAsyncIO* pIO);
bool GHasPendingIO();
#endif
