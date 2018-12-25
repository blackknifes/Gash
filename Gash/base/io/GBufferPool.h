#ifndef __GBUFFERPOOL_H__
#define __GBUFFERPOOL_H__
#include <Windows.h>
#include "../exception_handler/GNewHandler.h"
#include "../GMemoryGC.h"

template<int blockSize, int allocCount = 16>
class GBufferPool: public GMemoryGC<GBufferPool<blockSize, allocCount>>
{
public:
	static const size_t buffer_size = (size_t)blockSize;

	GBufferPool()
	{
		InitializeCriticalSectionAndSpinCount(&m_section, 400);
	}
	~GBufferPool()
	{
		for (void* pData : m_pool)
			free(pData);
		DeleteCriticalSection(&m_section);
	}

	void push(void* pData)
	{
		EnterCriticalSection(&m_section);
		m_pool.push_back(pData);
		LeaveCriticalSection(&m_section);
	}

	void* pop()
	{
		EnterCriticalSection(&m_section);
		if (m_pool.empty())
			reserveMore();
		void* pData = m_pool.back();
		m_pool.pop_back();
		LeaveCriticalSection(&m_section);
		return pData;
	}

	void gc()
	{
		EnterCriticalSection(&m_section);
		for (void* pData: m_pool)
			free(pData);
		m_pool.clear();
		LeaveCriticalSection(&m_section);
	}
private:
	void reserveMore()
	{
		for (int i = 0; i < allocCount; ++i)
			m_pool.push_back(malloc(buffer_size));
	}

	std::vector<void*> m_pool;
	CRITICAL_SECTION m_section;
};
#endif