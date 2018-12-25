#ifndef __GMEMORYGC_H__
#define __GMEMORYGC_H__
#include "exception_handler/GNewHandler.h"

template<typename _Ty>
class GMemoryGC
{
public:
	GMemoryGC()
	{
		GAddNewHandler(GarbageCollection, static_cast<_Ty*>(this));
	}
	~GMemoryGC()
	{
		GRemoveNewHandler(GarbageCollection, static_cast<_Ty*>(this));
	}

	static void GarbageCollection(void* pData)
	{
		_Ty* pObj = (_Ty*)pData;
		pObj->gc();
	}
};
#endif