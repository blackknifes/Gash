#ifndef __GASYNCENGINE_H__
#define __GASYNCENGINE_H__
#include <Windows.h>
#include "fileio/GAsyncIO.h"
#include "fileio/GAsyncDevice.h"
#include "../GObjectPool.h"

GREFPTR_DEF(GAsyncEngine);

class GAsyncEngine: public GObject
{
public:
	GAsyncEngine(DWORD numThread = 0);
	~GAsyncEngine();

	bool isValid() const;
	bool addDevice(GAsyncDevicePtr pDevice);
	bool getIOTask(GAsyncDevice** pDevice, GAsyncIO** pIO, DWORD overtime = INFINITE);
	size_t getIOTaskList(GAsyncDevice** pDeviceList, GAsyncIO** pTaskList, size_t size, DWORD overtime = INFINITE);

	void doAsyncIO(GAsyncDevicePtr pDevice, GAsyncIO* pIO);

	DWORD getMaxThreadCount() const;

private:
	GObjectClass(GAsyncEngine);
	HANDLE m_hIOCP;
	DWORD m_dwNumOfThreads;
};
#endif