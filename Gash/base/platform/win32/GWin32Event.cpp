#include "GWin32Event.h"



GWin32Event::GWin32Event()
	:m_hEvent(nullptr)
{

}

GWin32Event::~GWin32Event()
{
	signal();
	close();
}

GString GWin32Event::getName() const
{
	return m_strName;
}

void GWin32Event::setName(const GString& name)
{
	close();
	m_strName = name;
}

bool GWin32Event::open()
{
	if (!isClosed())
		return false;
	if (m_strName.isEmptyOrBlank())
		m_hEvent = CreateEventW(nullptr, FALSE, TRUE, nullptr);
	else
		m_hEvent = CreateEventW(nullptr, FALSE, TRUE, m_strName.data());

	GetLastError();
	return m_hEvent != nullptr;
}

void GWin32Event::close()
{
	if (m_hEvent)
	{
		CloseHandle(m_hEvent);
		m_hEvent = nullptr;
	}
}

bool GWin32Event::wait(DWORD timeout /*= INFINITE*/)
{
	DWORD result = WaitForSingleObject(m_hEvent, timeout);
	switch (result)
	{
	case WAIT_OBJECT_0:
	case WAIT_FAILED:
		return true;
	default:
		break;
	}
	return false;
}

bool GWin32Event::isClosed() const
{
	return m_hEvent == nullptr;
}

void* GWin32Event::getNativeHandle() const
{
	return m_hEvent;
}

void GWin32Event::signal()
{
	if (!isClosed())
		SetEvent(m_hEvent);
}
