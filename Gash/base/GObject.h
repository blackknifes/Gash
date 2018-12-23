#ifndef __GOBJECT_H__
#define __GOBJECT_H__
#include <atomic>
#include "GScopePointer.h"
#include "GEnums.h"
#include "GGlobalFunction.h"

GREFPTR_DEF(GObject);

#define GObjectImplements(_Class) \
public: \
	virtual void AddRef() { ++m_referenceCount; } \
	virtual bool Release() { if(--m_referenceCount == 0) { this->destroy(); return true; }; return false; } \
	virtual ref_type GetReferenceCount() const { return m_referenceCount; }; \
	virtual void destroy() { delete this; }; \
private: \
	std::atomic<unsigned int> m_referenceCount;

#define GObjectImplements2(_Class) \
public: \
	virtual void AddRef() { ++m_referenceCount; } \
	virtual bool Release() { if(--m_referenceCount == 0) { this->destroy(); return true; }; return false; } \
	virtual ref_type GetReferenceCount() const { return m_referenceCount; }; \
private: \
	std::atomic<unsigned int> m_referenceCount;

#define GObjectClass(_Class) private: _Class(const _Class&); _Class(_Class&&);

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