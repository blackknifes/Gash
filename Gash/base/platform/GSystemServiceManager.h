#ifndef __GSYSTEMSERVICEMANAGER_H__
#define __GSYSTEMSERVICEMANAGER_H__
#include "../GObject.h"
#include "GService.h"

class GSystemServiceManager: public virtual GObject
{
public:
	using State = GService::State;

	enum ServiceType
	{
		OwnProcess,
		ShareProcess,
		OwnAndShareProcess = OwnProcess | ShareProcess
	};

	enum StartType
	{
		BootStart,
		SystemStart,
		AutoStart,
		DemandStart,
		Disabled
	};

	enum ErrorControl
	{
		Ignore,
		Normal,
		Restart,
		Critical
	};

	virtual GServicePtr openService(const GString& name) = 0;
	virtual GServicePtr createService(
		const GString& name, 
		const GString& display, 
		const GString& exePath,
		ServiceType serviceType,
		StartType startType,
		ErrorControl errorControl
		) = 0;
	virtual void uninstallService(GServicePtr pService) = 0;
};
#endif