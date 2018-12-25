#ifndef __GCALLBACK_H__
#define __GCALLBACK_H__
#include "GObject.h"

class GCallback: public GObject
{
public:
	template<typename... _argsTy>
	GCallback(void(*func)(_argsTy...))
		:m_pCallback((void*)func)
	{

	}

	template<typename... _argsTy>
	GCallback(void(*func)(_argsTy...))
		:m_pCallback((void*)func)
	{

	}

	template<typename _Class, typename... _argsTy>
	GCallback(void(_Class:: *func)(_argsTy...))
		: m_pCallback(*(void**)&func)
	{

	}
	
	template<typename _Class, typename... _argsTy>
	void call(_Class* pObject, _argsTy&&... args)
	{
		typedef void(*Callback)(_argsTy...);
		typedef void(_Class::* ObjectCallback)(_argsTy...);
		if (pObject == nullptr)
			((Callback)m_pCallback)(std::move(args)...);
		else
		{
			ObjectCallback call = *(ObjectCallback**)&m_pCallback;
			(pObject->*call)(std::move(args)...);
		}
	}
private:
	GObjectClassPooled(GCallback);
	void* m_pCallback;
};
#endif