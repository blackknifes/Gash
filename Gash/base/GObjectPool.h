#ifndef __GOBJECTPOOL_H__
#define __GOBJECTPOOL_H__
#include <Windows.h>
#include <vector>
#include <assert.h>

template<typename _Ty>
class GObjectPool
{
public:
	GObjectPool()
	{
		::InitializeCriticalSectionAndSpinCount(&m_section, 200);
	}
	~GObjectPool()
	{
		::DeleteCriticalSection(&m_section);
		for (_Ty* pData: m_pool)
			delete pData;
	}

	_Ty* pop()
	{
		::EnterCriticalSection(&m_section);
		if (m_freePool.empty())
			reserve_more();
		_Ty* pData = m_freePool.back();
		m_freePool.pop_back();
		::LeaveCriticalSection(&m_section);
		return pData;
	}

	void push(_Ty* pData)
	{
		assert(pData != nullptr);
		::EnterCriticalSection(&m_section);
		m_freePool.push_back(pData);
		::LeaveCriticalSection(&m_section);
	}
private:
	void reserve_more()
	{
		for (size_t i = 0; i < 16; ++i)
		{
			_Ty* pData = new _Ty();
			m_pool.push_back(pData);
			m_freePool.push_back(pData);
		}
	}

	std::vector<_Ty*> m_pool;
	std::vector<_Ty*> m_freePool;
	CRITICAL_SECTION m_section;
};
#endif
