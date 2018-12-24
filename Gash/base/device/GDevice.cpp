#include "GDevice.h"

GDevice::GDevice()
	:m_iErrorCode(0)
{
	
}

bool GDevice::isSync() const
{
	return !this->isAsync();
}

int GDevice::getErrorCode() const
{
	return m_iErrorCode;
}

std::string GDevice::getErrorMessage() const
{
	switch (m_iErrorCode)
	{
	case IO_Success:
		break;
	case IO_Internal_Error:
		return "Internal Error";
	case IO_NotOpen:
		return "Device is not open.";
	case IO_Read_Pending:
		return "Read Peding";
	case IO_Write_Pending:
		return "Write Peding";
	default:
		break;
	}
	return "Operation success.";
}

void GDevice::setErrorCode(int errCode)
{
	m_iErrorCode = errCode;
}
