#ifndef __GOBJECT_H__
#define __GOBJECT_H__
#include <atomic>
#include "GScopePointer.h"
#include "GEnums.h"
#include "GGlobalFunction.h"
#include "GMemoryPool.h"
#include "GAllocator.h"

GREFPTR_DEF(GObject);

#define GObjectImplementsBase(_Class) \
public: \
	virtual void AddRef() { ++m_referenceCount; } \
	virtual bool Release() { if(--m_referenceCount == 0) { this->destroy(); return true; }; return false; } \
	virtual ref_type GetReferenceCount() const { return m_referenceCount; }; \
private: \
	std::atomic<unsigned int> m_referenceCount;

#define GObjectImplements(_Class) \
GObjectImplementsBase(_Class) \
public: \
	virtual void destroy() { delete this; };


#define GObjectImplementsPooled(_Class) \
GObjectImplementsBase(_Class) \
public: \
	template<typename... _argsTy> \
	static GScopePointer<_Class> New(_argsTy... args) \
	{ \
		return GNewObject<_Class>(args...); \
	} \
	virtual void destroy() override \
	{ \
		GDestroyObject(this); \
	} \
	friend void GDestroyObject<_Class>(_Class*); \
private:

#define GDisallowAssignAndCopy(_Class) private: _Class(const _Class&); _Class(_Class&&);

#define GObjectClass(_Class) GObjectImplements(_Class); GDisallowAssignAndCopy(_Class)
#define GObjectClassPooled(_Class) GObjectImplementsPooled(_Class); GDisallowAssignAndCopy(_Class)

class GObject
{
public:
	typedef unsigned int ref_type;
	virtual ~GObject() {};

	virtual void AddRef() = 0;
	virtual bool Release() = 0;
	virtual ref_type GetReferenceCount() const = 0;
	virtual void destroy() = 0;
private:
	std::atomic<unsigned int> m_referenceCount;
};
#endif