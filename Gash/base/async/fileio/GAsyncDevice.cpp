#include "GAsyncDevice.h"



GAsyncDevice::GAsyncDevice()
	:m_ended(false)
{

}

bool GAsyncDevice::isEnd()
{
	return m_ended;
}

void GAsyncDevice::end()
{
	this->onEnd();
	m_ended = true;
}
