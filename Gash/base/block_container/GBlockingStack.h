#ifndef __GBLOCKINGSTACK_H__
#define __GBLOCKINGSTACK_H__
#include <vector>
#include <Windows.h>

template<typename _Ty>
class GBlockingStack
{
public:
	typedef typename std::vector<_Ty>::iterator iterator;

	GBlockingStack()
	{
		InitializeCriticalSectionAndSpinCount(&m_section, 400);
	}
	~GBlockingStack()
	{
		DeleteCriticalSection(&m_section);
	}

	bool empty()
	{
		return m_pool.empty();
	}

	_Ty pop()
	{
		EnterCriticalSection(&m_section);
		_Ty data = m_pool.back();
		m_pool.pop_back();
		LeaveCriticalSection(&m_section);
		return std::move(data);
	}

	void push(const _Ty& pData)
	{
		EnterCriticalSection(&m_section);
		m_pool.push_back(pData);
		LeaveCriticalSection(&m_section);
	}

	void push(const std::vector<_Ty>& list)
	{
		EnterCriticalSection(&m_section);
		for (auto itor = list.begin(); itor != list.end(); ++itor)
			m_pool.push_back(*itor);
		LeaveCriticalSection(&m_section);
	}

	void lock()
	{
		EnterCriticalSection(&m_section);
	}

	void unlock()
	{
		LeaveCriticalSection(&m_section);
	}

	iterator begin()
	{
		return m_pool.begin();
	}

	iterator end()
	{
		return m_pool.end();
	}
private:
	std::vector<_Ty> m_pool;
	CRITICAL_SECTION m_section;
};
#endif