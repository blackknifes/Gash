#ifndef __GASYNCENGINE_H__
#define __GASYNCENGINE_H__
#include <Windows.h>
#include "../GObjectPool.h"
#include "../io/GIODevice.h"
#include "../device/GAsyncIO.h"

GREFPTR_DEF(GAsyncEngine);

class GAsyncEngine: public virtual GObject
{
public:
	GAsyncEngine(DWORD numThread = 0);
	~GAsyncEngine();

	bool isValid() const;
	bool addDevice(GIODevicePtr pDevice);
	bool getIOTask(GIODevice** pDevice, GAsyncIO** pIO, DWORD overtime = INFINITE);
	size_t getIOTaskList(GIODevice** pDeviceList, GAsyncIO** pTaskList, size_t size, DWORD overtime = INFINITE);

	void doAsyncIO(GIODevicePtr pDevice, GAsyncIO* pIO);

	DWORD getMaxThreadCount() const;

private:
	GObjectClass(GAsyncEngine);
	HANDLE m_hIOCP;
	DWORD m_dwNumOfThreads;
};
#endif