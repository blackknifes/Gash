#include "GStreamDevice.h"

GStreamDevice::~GStreamDevice()
{
	if (m_pDevice != nullptr)
		m_pDevice->close();
}

void GStreamDevice::close()
{
	m_pDevice->close();
}

bool GStreamDevice::isClosed() const
{
	return m_pDevice->isClosed();
}

GIODevicePtr GStreamDevice::getDevice() const
{
	return m_pDevice;
}

void GStreamDevice::setDevice(GIODevicePtr pDevice)
{
	m_pDevice = pDevice;
}