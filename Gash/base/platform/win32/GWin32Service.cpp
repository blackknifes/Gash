#include "GWin32Service.h"
#include "../../GDataArray.h"

GService::State GetServiceState(DWORD state)
{
	switch (state)
	{
	case SERVICE_STOPPED:
		return GService::Stopped;
	case SERVICE_START_PENDING:
		return GService::Starting;
	case SERVICE_STOP_PENDING:
		return GService::Stopping;
	case SERVICE_RUNNING:
		return GService::Running;
	case SERVICE_CONTINUE_PENDING:
		return GService::Resuming;
	case SERVICE_PAUSE_PENDING:
		return GService::Pausing;
	case SERVICE_PAUSED:
		return GService::Paused;
	default:
		break;
	}
	return GService::Unknown;
}

GWin32Service::GWin32Service(SC_HANDLE hService)
	:m_hService(hService)
{

}

GWin32Service::~GWin32Service()
{
	CloseServiceHandle(m_hService);
}

bool GWin32Service::start(const std::vector<GString>& args)
{
	const wchar_t* tmp[64];
	for (size_t i = 0; i < args.size(); ++i)
		tmp[i] = args[i].data();

	return StartServiceW(m_hService, args.size(), tmp);
}

bool GWin32Service::stop()
{
	return ControlService(m_hService, SERVICE_CONTROL_STOP, nullptr);
}

GService::State GWin32Service::getState() const
{
	SERVICE_STATUS status;
	if (QueryServiceStatus(m_hService, &status) == FALSE)
		return GService::Unknown;
	return GetServiceState(status.dwCurrentState);
}

GString GWin32Service::getName() const
{
	QUERY_SERVICE_CONFIGW info[32];
	QueryServiceConfigW(m_hService, info, sizeof(info), nullptr);
	return info[0].lpDisplayName;
}

GString GWin32Service::getDisplayName() const
{
	QUERY_SERVICE_CONFIGW info[32];
	QueryServiceConfigW(m_hService, info, sizeof(info), nullptr);
	return info[0].lpServiceStartName;

}

void* GWin32Service::getNativeHandle() const
{
	return m_hService;
}
