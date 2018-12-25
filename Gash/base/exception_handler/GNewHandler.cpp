#include "GNewHandler.h"
#include <new.h>

int GNewHandler::Handle(size_t size)
{
	GNewHandler* pHandler = GNewHandler::getInstance();
	EnterCriticalSection(&pHandler->m_section);
	for (Handler handler : pHandler->m_handlerList)
		handler.callback(handler.pUserData);
	LeaveCriticalSection(&pHandler->m_section);
	return 0;
}

GNewHandler::GNewHandler()
{
	_set_new_handler(Handle);
	InitializeCriticalSectionAndSpinCount(&m_section, 400);
}

GNewHandler::~GNewHandler()
{
	_set_new_handler(nullptr);
	DeleteCriticalSection(&m_section);
}

void GNewHandler::addHandler(Callback callback, void* pUserData /*= nullptr*/)
{
	Handler handler;
	handler.callback = callback;
	handler.pUserData = pUserData;
	EnterCriticalSection(&m_section);
	m_handlerList.push_back(handler);
	LeaveCriticalSection(&m_section);
}

void GNewHandler::removeHandler(Callback callback, void* pUserData /*= nullptr*/)
{
	EnterCriticalSection(&m_section);
	for (auto itor = m_handlerList.begin(); itor != m_handlerList.end(); ++itor)
	{
		const Handler& handler = *itor;
		if (handler.callback == callback && handler.pUserData == pUserData)
		{
			m_handlerList.erase(itor);
			break;
		}
	}
	LeaveCriticalSection(&m_section);
}

void GNewHandler::clearHandler()
{
	EnterCriticalSection(&m_section);
	m_handlerList.clear();
	LeaveCriticalSection(&m_section);
}

void GAddNewHandler(GNewHandler::Callback callback, void* pUserData /*= nullptr*/)
{
	GNewHandler::getInstance()->addHandler(callback, pUserData);
}

void GRemoveNewHandler(GNewHandler::Callback callback, void* pUserData /*= nullptr*/)
{
	GNewHandler::getInstance()->removeHandler(callback, pUserData);
}
