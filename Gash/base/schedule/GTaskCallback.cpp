#include "GTaskCallback.h"



GTaskCallback::GTaskCallback(Callback callback)
	:GTask(GTaskCallback::ID), m_callback(callback)
{

}

void GTaskCallback::execute()
{
	m_callback();
}
