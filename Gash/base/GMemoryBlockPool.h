#ifndef __GMEMORYBLOCKPOOL_H__
#define __GMEMORYBLOCKPOOL_H__
#include <Windows.h>
#include <vector>

struct GMemoryBlock
{
	GMemoryBlock();
	void* data;
	size_t size;
};

class GMemoryBlockAllocator
{
public:
	GMemoryBlockAllocator(size_t size);
	~GMemoryBlockAllocator();

	size_t getSize() const;
	GMemoryBlock pop();
	void push(GMemoryBlock block);
	void dealloc();
private:
	void reserve_more();
	const size_t m_sSizeOfBytes;
	std::vector<void*> m_allPool;
	std::vector<void*> m_pool;
};

class GMemoryBlockPool
{
public:
	static GMemoryBlockPool* getInstance()
	{
		if (s_pInstance == nullptr)
			s_pInstance = new GMemoryBlockPool();
		return s_pInstance;
	}

	static GMemoryBlockPool* destroyInstance()
	{
		if (s_pInstance)
		{
			delete s_pInstance;
			s_pInstance = nullptr;
		}
	}

	GMemoryBlockPool();
	~GMemoryBlockPool();

	GMemoryBlock pop(size_t size);
	void push(const GMemoryBlock& block);

	void dealloc();
private:
	static GMemoryBlockPool* s_pInstance;
	std::vector<GMemoryBlockAllocator> m_pBlockAllocatorList;
	CRITICAL_SECTION m_section;
};

__declspec(selectany) GMemoryBlockPool* GMemoryBlockPool::s_pInstance = nullptr;

GMemoryBlock PopBlockFromPool(size_t size);
void PushBlockFromPool(const GMemoryBlock& block);
#endif