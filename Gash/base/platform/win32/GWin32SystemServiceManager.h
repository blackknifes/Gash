#ifndef __GWIN32SYSTEMSERVICEMANAGER_H__
#define __GWIN32SYSTEMSERVICEMANAGER_H__
#include "../GSystemServiceManager.h"

GREFPTR_DEF(GWin32SystemServiceManager);

class GWin32SystemServiceManager: public GSystemServiceManager
{
public:
	static GWin32SystemServiceManagerPtr Create();
	GWin32SystemServiceManager();


	virtual GServicePtr openService(const GString& name) override;
	virtual GServicePtr createService(const GString& name, const GString& display, const GString& exePath, ServiceType serviceType, StartType startType, ErrorControl errorControl) override;
	virtual void uninstallService(GServicePtr pService) override;
private:
	GObjectClassPooled(GWin32SystemServiceManager);
	~GWin32SystemServiceManager();

	SC_HANDLE m_hSCManager;
};
#endif