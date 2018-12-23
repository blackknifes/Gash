#ifndef __GFIELDGETTER_H__
#define __GFIELDGETTER_H__

#include "GReflectionBase.h"

GREFPTR_DEF(GFieldGetter);

class GFieldGetter: public GObject
{
public:
	template<typename _Class, typename _Ty>
	static GFieldGetterPtr Create(_Ty _Class::* pField);
	template<typename _Class, typename _Ty>
	static GFieldGetterPtr Create(_Ty (_Class::* pField)() const);

	virtual GValue get(GReflectionObjectPtr pObject) const = 0;
};

template<typename _Class, typename _Ty>
class GFieldGetterImpl : public GFieldGetter
{
public:
	typedef _Ty _Class::* FieldPointer;
	GFieldGetterImpl(FieldPointer pField)
		:m_pField(pField)
	{}

	virtual GValue get(GReflectionObjectPtr pObject) const override
	{
		_Class* pObj = static_cast<_Class*>(pObject.get());
		return GValueConverter<_Ty>()(pObj->*m_pField);
	}
private:
	GObjectImplements(GFieldGetterImpl);
	FieldPointer m_pField;
};

template<typename _Class, typename _Ty>
class GFieldGetterFunctionImpl : public GFieldGetter
{
public:
	typedef _Ty(_Class::* FieldFunctionPointer)() const;
	GFieldGetterFunctionImpl(FieldFunctionPointer pField)
		:m_pField(pField)
	{}

	virtual GValue get(GReflectionObjectPtr pObject) const override
	{
		_Class* pObj = static_cast<_Class*>(pObject.get());
		return GValueConverter<_Ty>()((pObj->*m_pField)());
	}
private:
	GObjectImplements(GFieldGetterFunctionImpl);
	FieldFunctionPointer m_pField;
};

template<typename _Class, typename _Ty>
GFieldGetterPtr GFieldGetter::Create(_Ty _Class::* pField)
{
	return new GFieldGetterImpl<_Class, _Ty>(pField);
}

template<typename _Class, typename _Ty>
GFieldGetterPtr GFieldGetter::Create(_Ty(_Class::* pField)() const)
{
	return new GFieldGetterFunctionImpl<_Class, _Ty>(pField);
}
#endif