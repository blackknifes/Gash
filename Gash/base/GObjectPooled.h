#ifndef __GOBJECTPOOLED_H__
#define __GOBJECTPOOLED_H__
#include "GObject.h"
#include "GAllocator.h"

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
		_Ty* pData = m_allocator.allocate();
		m_allocator.construct(pData, args...);
		((GObjectPooled*)pData)->m_bPooled = true;
		return pData;
	}

	virtual void destroy() override
	{
		if (m_bPooled)
			m_allocator.deallocate(this);
		else
			GObject::destroy();
	}
private:
	bool m_bPooled;
	GAllocator<_Ty> m_allocator;
};
#endif