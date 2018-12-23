#include "GMemoryBlockPool.h"
#include <assert.h>

GMemoryBlockAllocator::GMemoryBlockAllocator(size_t size)
	:m_sSizeOfBytes(size)
{
}

GMemoryBlockAllocator::~GMemoryBlockAllocator()
{
	for (void* pBlock: m_allPool)
		free(pBlock);
}

size_t GMemoryBlockAllocator::getSize() const
{
	return m_sSizeOfBytes;
}

GMemoryBlock GMemoryBlockAllocator::pop()
{
	if (m_pool.empty())
		reserve_more();
	GMemoryBlock block;
	block.data = m_pool.back();
	m_pool.pop_back();

	block.size = m_sSizeOfBytes;
	return block;
}

void GMemoryBlockAllocator::push(GMemoryBlock block)
{
	assert(block.size == m_sSizeOfBytes);
	m_pool.push_back(block.data);
}


void GMemoryBlockAllocator::dealloc()
{
	if (m_sSizeOfBytes < 32)
		return;

	for (void* pBlock: m_pool)
		free(pBlock);
	auto itor = m_allPool.begin();
	while (itor != m_allPool.end())
	{
		auto findItor = std::find(m_pool.begin(), m_pool.end(), *itor);
		if (findItor != m_pool.end())
		{
			m_pool.erase(findItor);
			itor = m_allPool.erase(itor);
			continue;
		}
		++itor;
	}
}

void GMemoryBlockAllocator::reserve_more()
{
	if (m_sSizeOfBytes < 32)
	{
		unsigned char* pBlock = (unsigned char*)malloc(m_sSizeOfBytes * 128);
		m_allPool.push_back(pBlock);
		for (size_t i = 0; i < 128; ++i)
			m_pool.push_back(pBlock + i * m_sSizeOfBytes);
	}
	else
	{
		for (size_t i = 0; i < 128; ++i)
		{
			unsigned char* pBlock = (unsigned char*)malloc(m_sSizeOfBytes);
			m_pool.push_back(pBlock);
			m_allPool.push_back(pBlock);
		}
	}
}

GMemoryBlockPool::GMemoryBlockPool()
{
	InitializeCriticalSectionAndSpinCount(&m_section, 400);
}

GMemoryBlockPool::~GMemoryBlockPool()
{
	DeleteCriticalSection(&m_section);
}

GMemoryBlock GMemoryBlockPool::pop(size_t size)
{
	EnterCriticalSection(&m_section);
	if (m_pBlockAllocatorList.size() < size)
	{
		size_t i = m_pBlockAllocatorList.size();
		for (; i < size; ++i)
			m_pBlockAllocatorList.push_back(GMemoryBlockAllocator(i + 1));
	}
	GMemoryBlock block = m_pBlockAllocatorList.at(size - 1).pop();
	LeaveCriticalSection(&m_section);
	return block;
}

void GMemoryBlockPool::push(const GMemoryBlock& block)
{
	EnterCriticalSection(&m_section);
	if (block.size <= m_pBlockAllocatorList.size())
		m_pBlockAllocatorList.at(block.size).push(block);
	LeaveCriticalSection(&m_section);
}

void GMemoryBlockPool::dealloc()
{
	EnterCriticalSection(&m_section);
	for (size_t i = 32; i < m_pBlockAllocatorList.size(); ++i)
		m_pBlockAllocatorList[i].dealloc();
	LeaveCriticalSection(&m_section);
}

GMemoryBlock PopBlockFromPool(size_t size)
{
	return GMemoryBlockPool::getInstance()->pop(size);
}

void PushBlockFromPool(const GMemoryBlock& block)
{
	GMemoryBlockPool::getInstance()->push(block);
}

GMemoryBlock::GMemoryBlock()
	:data(nullptr), size(0)
{

}
