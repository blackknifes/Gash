#include "GWin32SystemServiceManager.h"
#include "GWin32Service.h"

static DWORD GetServiceType(GSystemServiceManager::ServiceType type)
{
	switch (type)
	{
	case GSystemServiceManager::OwnProcess:
		return SERVICE_WIN32_OWN_PROCESS;
	case GSystemServiceManager::ShareProcess:
		return SERVICE_WIN32_SHARE_PROCESS;
	default:
		break;
	}
	return -1;
}

static DWORD GetStartType(GSystemServiceManager::StartType type)
{
	switch (type)
	{
	case GSystemServiceManager::BootStart:
		return SERVICE_BOOT_START;
	case GSystemServiceManager::SystemStart:
		return SERVICE_SYSTEM_START;
	case GSystemServiceManager::AutoStart:
		return SERVICE_AUTO_START;
	case GSystemServiceManager::DemandStart:
		return SERVICE_DEMAND_START;
	case GSystemServiceManager::Disabled:
		return SERVICE_DISABLED;
	default:
		break;
	}
	return -1;
}

static DWORD GetErrorControl(GSystemServiceManager::ErrorControl control)
{
	switch (control)
	{
	case GSystemServiceManager::Ignore:
		return SERVICE_ERROR_IGNORE;
	case GSystemServiceManager::Normal:
		return SERVICE_ERROR_NORMAL;
	case GSystemServiceManager::Restart:
		return SERVICE_ERROR_SEVERE;
	case GSystemServiceManager::Critical:
		return SERVICE_ERROR_CRITICAL;
	default:
		break;
	}
	return -1;
}

GWin32SystemServiceManagerPtr GWin32SystemServiceManager::Create()
{
	SC_HANDLE hSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
	if (hSCManager == nullptr)
		return nullptr;
	GWin32SystemServiceManagerPtr pManager = GWin32SystemServiceManager::CreatePooled();
	pManager->m_hSCManager = hSCManager;
	return pManager;
}

GServicePtr GWin32SystemServiceManager::openService(const GString& name)
{
	SC_HANDLE hService = OpenServiceW(m_hSCManager, name.data(), SERVICE_ALL_ACCESS);
	if (hService == nullptr)
		return nullptr;
	return new GWin32Service(hService);
}

GServicePtr GWin32SystemServiceManager::createService(const GString& name, const GString& display, const GString& exePath, ServiceType serviceType, StartType startType, ErrorControl errorControl)
{
	SC_HANDLE hService = ::CreateServiceW(m_hSCManager, name.data(), display.data(), SERVICE_ALL_ACCESS,
		GetServiceType(serviceType),
		GetStartType(startType),
		GetErrorControl(errorControl),
		exePath.data(),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr);
	if (hService == nullptr)
		return nullptr;
	return new GWin32Service(hService);
}

void GWin32SystemServiceManager::uninstallService(GServicePtr pService)
{
	pService->stop();
	DeleteService((SC_HANDLE)pService->getNativeHandle());
}

GWin32SystemServiceManager::GWin32SystemServiceManager()
{
	
}

GWin32SystemServiceManager::~GWin32SystemServiceManager()
{
	::CloseServiceHandle(m_hSCManager);
}
