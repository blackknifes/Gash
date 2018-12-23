#ifndef __GMEMORYPOOL_H__
#define __GMEMORYPOOL_H__
#include <Windows.h>
#include <vector>

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

	
	GMemoryPool()
	{
		if (sizeof(_Ty) < 32)
			m_sBlockSize = 256;
		else
			m_sBlockSize = 64;
		InitializeCriticalSectionAndSpinCount(&m_section, 100);
	}
	~GMemoryPool()
	{
		for (_Ty* pArray : m_pBlockList)
			free(pArray);
		DeleteCriticalSection(&m_section);
	}

	template<typename... _argsTy>
	_Ty* construct(_argsTy&&... args)
	{
		EnterCriticalSection(&m_section);
		if (empty())
			reserve_more();
		_Ty* pData = m_pPool.back();
		m_pPool.pop_back();
		LeaveCriticalSection(&m_section);
		new (pData) _Ty(std::move(args...));
		return pData;
	}

	_Ty* pop()
	{
		EnterCriticalSection(&m_section);
		if (empty())
			reserve_more();
		_Ty* pData = m_pPool.back();
		m_pPool.pop_back();
		LeaveCriticalSection(&m_section);
		new (pData) _Ty();
		return pData;
	}

	void push(_Ty* pData)
	{
		pData->~_Ty();
		EnterCriticalSection(&m_section);
		m_pPool.push_back(pData);
		LeaveCriticalSection(&m_section);
	}

	bool empty() const
	{
		return m_pPool.empty();
	}

private:
	size_t m_sBlockSize;
	std::vector<_Ty*> m_pBlockList;
	std::vector<_Ty*> m_pPool;
	CRITICAL_SECTION m_section;

	void reserve_more()
	{
		_Ty* pArray = (_Ty*)malloc(m_sBlockSize * sizeof(_Ty));
		m_pBlockList.push_back(pArray);
		for (size_t i = 0; i < m_sBlockSize; ++i)
			m_pPool.push_back(pArray + i);
	}

	static GMemoryPool* s_pInstance;
};

template<typename _Ty>
__declspec(selectany) GMemoryPool<_Ty>* GMemoryPool<_Ty>::s_pInstance = nullptr;

template<typename _Ty, typename... _argsTy>
_Ty* PopFromGlobalPool(_argsTy&&... args)
{
	return GMemoryPool<_Ty>::getInstance()->construct(std::move(args)...);
}

template<typename _Ty>
_Ty* PopFromGlobalPool()
{
	return GMemoryPool<_Ty>::getInstance()->pop();
}

template<typename _Ty>
void PushToGlobalPool(_Ty* pData)
{
	GMemoryPool<_Ty>::getInstance()->push(pData);
}
#endif