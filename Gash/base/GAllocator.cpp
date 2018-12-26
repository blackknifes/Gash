#include "GAllocator.h"
#include "math/GMath.h"

static GStaticAllocatorPool staticAllocatorPool;

template<typename _Ty>
static _Ty* MovePointer(_Ty* pData, int offset)
{
	return (_Ty*)((unsigned char*)pData + offset);
}

struct GStaticMemoryNode
{
	GStaticMemoryNode* next;
};

void* GAllocateFromPool(size_t size)
{
	return staticAllocatorPool.allocate(size);
}

void GDeallocateToPool(void* pData, size_t size)
{
	staticAllocatorPool.deallocate(pData, size);
}

GStaticAllocator::GStaticAllocator()
	:m_size(0), m_pBufferArray(nullptr)
{
	InitializeCriticalSectionAndSpinCount(&m_section, 400);
}

GStaticAllocator::~GStaticAllocator()
{
	DeleteCriticalSection(&m_section);
	for (void* pData : m_pBlockArray)
		free(pData);
}

void* GStaticAllocator::allocate()
{
	EnterCriticalSection(&m_section);
	if (m_pBufferArray == nullptr)
		reserve_more(1);
	void* pData = m_pBufferArray;
	m_pBufferArray = ((GStaticMemoryNode*)pData)->next;
	LeaveCriticalSection(&m_section);
	return pData;
}

void GStaticAllocator::deallocate(void* pData)
{
	EnterCriticalSection(&m_section);
	GStaticMemoryNode* pNode = (GStaticMemoryNode*)pData;
	pNode->next = (GStaticMemoryNode*)m_pBufferArray;
	LeaveCriticalSection(&m_section);
}

void GStaticAllocator::reserve_more(size_t count)
{
	static size_t allocCount = 512 / m_size;
	for (size_t i = 0; i < count; i += allocCount)
	{
		char* pData = (char*)malloc(512);
		m_pBlockArray.push_back(pData);
		GStaticMemoryNode* pNode = (GStaticMemoryNode*)pData;
		GStaticMemoryNode* pCur = (GStaticMemoryNode*)m_pBufferArray;
		m_pBufferArray = pNode;
		for (size_t j = 0; j < allocCount - 1; ++j)
		{
			pNode->next = MovePointer(pNode, m_size);
			pNode = pNode->next;
		}
		pNode->next = pCur;
	}
}

void GStaticAllocator::setSize(size_t size)
{
	m_size = size;
}

GStaticAllocatorPool::GStaticAllocatorPool()
{
	for (size_t i = 0; i < max_size; ++i)
		m_allocatorList[i].setSize((i + 1) << 2);
}

void* GStaticAllocatorPool::allocate(size_t size)
{
	if ((size >> 2) > max_size)
		return malloc(size);
	size = GMath::Align(size, 4) >> 2;
	return m_allocatorList[size - 1].allocate();
}

void GStaticAllocatorPool::deallocate(void* pData, size_t size)
{
	if ((size >> 2) > max_size)
		return free(pData);
	size = GMath::Align(size, 4) >> 2;
	return m_allocatorList[size - 1].deallocate(pData);
}
