#include "GIODevice.h"

GIODevice::GIODevice()
	:m_iErrorCode(0)
{
	
}

int GIODevice::getErrorCode() const
{
	return m_iErrorCode;
}

std::string GIODevice::getErrorMessage() const
{
	char buf[1024] = {};
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, m_iErrorCode, LANG_USER_DEFAULT, buf, sizeof(buf), nullptr);
	return buf;
}

void GIODevice::setErrorCode(int errCode)
{
	m_iErrorCode = errCode;
}