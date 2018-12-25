#ifndef __GALLOCATOR_H__
#define __GALLOCATOR_H__
#include <Windows.h>
#include <memory>
#include <vector>


void* GAllocateFromPool(size_t size);
void GDeallocateToPool(void* pData, size_t size);

template<typename _Ty>
_Ty* GAllocateObject()
{
	return GAllocateFromPool(sizeof(_Ty));
}

template<typename _Ty, typename... _argsTy>
_Ty* GNewObject(_argsTy&&... args)
{
	_Ty* pData = (_Ty*)GAllocateFromPool(sizeof(_Ty));
	new (pData) _Ty(std::move(args)...);
	return pData;
}

template<typename _Ty>
void GDestroyObject(_Ty* pData)
{
	pData->~_Ty();
	GDeallocateToPool(pData, sizeof(_Ty));
}

template<typename _Ty>
void GDeallocateObject(_Ty* pData)
{
	GDeallocateToPool(pData, sizeof(_Ty));
}

//¾²Ì¬ ÄÚ´æ·ÖÅäÆ÷
class GStaticAllocator
{
public:
	GStaticAllocator();
	~GStaticAllocator();

	void* allocate();
	void deallocate(void* pData);
	void reserve_more(size_t count);

	friend class GStaticAllocatorPool;
private:
	void setSize(size_t size);

	size_t m_size;
	void* m_pBufferArray;
	std::vector<void*> m_pBlockArray;
	CRITICAL_SECTION m_section;
};

//¾²Ì¬ ÄÚ´æ·ÖÅä³Ø
class GStaticAllocatorPool
{
public:
	static const size_t max_size = 512 >> 2;
	GStaticAllocatorPool();

	void* allocate(size_t size);
	void deallocate(void* pData, size_t size);

private:
	GStaticAllocator m_allocatorList[max_size];
};

template<typename _Ty>
class GAllocator
{
public:
	_Ty* allocate()
	{
		return (_Ty*)GAllocateFromPool(sizeof(_Ty));
	}
	void deallocate(_Ty* pData)
	{
		GDeallocateToPool(pData, sizeof(_Ty));
	}

	template<typename... _argsTy>
	void construct(_Ty* pData, _argsTy&&... args)
	{
		new (pData) _Ty(std::move(args)...);
	}

	void destroy(_Ty* pData)
	{
		pData->~_Ty();
	}
};
#endif