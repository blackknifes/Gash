#ifndef __GOBJECTPOOLED_H__
#define __GOBJECTPOOLED_H__
#include "GObject.h"

template<typename _Ty>
class GObjectPooled: public virtual GObject
{
public:
	GObjectPooled()
		:m_bPooled(false)
	{

	}
	template<typename... _argsTy>
	static GScopePointer<_Ty> CreatePooled(_argsTy&&... args)
	{
		_Ty* pData = PopFromGlobalPool<_Ty>(std::move(args)...);
		((GObjectPooled*)pData)->m_bPooled = true;
		return pData;
	}

	virtual void destroy() override
	{
		if (m_bPooled)
			PushToGlobalPool(this);
		else
			GObject::destroy();
	}
private:
	bool m_bPooled;
};
#endif