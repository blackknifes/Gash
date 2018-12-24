#ifndef __GMEMORYPOOL_H__
#define __GMEMORYPOOL_H__
#include <Windows.h>
#include <vector>
#include "GAllocator.h"

template<typename _Ty>
class GMemoryPool
{
public:
	static GMemoryPool* getInstance()
	{
		if (s_pInstance == nullptr)
			s_pInstance = new GMemoryPool();
		return s_pInstance;
	}
	static void destroyInstance()
	{
		if (s_pInstance)
		{
			delete s_pInstance;
			s_pInstance = nullptr;
		}
	}

	template<typename... _argsTy>
	_Ty* construct(_argsTy&&... args)
	{
		_Ty* pData = (_Ty*)PopBlockFromPool(sizeof(_Ty)).data;
		new (pData) _Ty(std::move(args)...);
		return pData;
	}

	_Ty* pop()
	{
		_Ty* pData = (_Ty*)PopBlockFromPool(sizeof(_Ty)).data;
		new (pData) _Ty();
		return pData;
	}

	void push(_Ty* pData)
	{
		pData->~_Ty();
		PushBlockToPool(pData, sizeof(_Ty));
	}

private:
	static GMemoryPool* s_pInstance;
};

template<typename _Ty>
__declspec(selectany) GMemoryPool<_Ty>* GMemoryPool<_Ty>::s_pInstance = nullptr;
#endif