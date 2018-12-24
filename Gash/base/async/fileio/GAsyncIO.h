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
	ADDRINFOEXW addresses[40];			//dns查询结果列表
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

	OVERLAPPED overlapped;				//重叠io结构
	IOType ioType;						//io类型
	DWORD dwTransferSize;				//本次io大小
	DWORD dwExpectSize;					//期望大小
	unsigned __int64 startTime;			//开始时间
	GObjectPtr pObject;					//引用对象

	union
	{
		WriteCallback writeCallback;	//写出回调
		ReadCallback readCallback;		//读入回调
		OpenCallback openCallback;
		CloseCallback closeCallback;
		QueryDnsCallback queryDnsCallback;
	};

	union
	{
		unsigned char buffer[0x4000];		//缓冲
		GAddressInfoList addressList;		//dns查询结果列表
	};
};

GAsyncIO* GNewIO();
void GFreeIO(GAsyncIO* pIO);
bool GHasPendingIO();
#endif
