#ifndef __GFIELDSETTER_H__
#define __GFIELDSETTER_H__

#include "GReflectionBase.h"

GREFPTR_DEF(GFieldSetter);

class GFieldSetter: public virtual GObject
{
public:
	template<typename _Class, typename _Ty>
	static GFieldSetterPtr Create(_Ty _Class::* pField);
	template<typename _Class, typename _Ty>
	static GFieldSetterPtr Create(void (_Class::* pField)(_Ty val));

	virtual void set(GReflectionObjectPtr obj, const GValue& val) const = 0;
};

template<typename _Class, typename _Ty>
class GFieldSetterImpl : public GFieldSetter
{
public:
	typedef _Ty _Class::* FieldPointer;
	GFieldSetterImpl(FieldPointer pField)
		:m_pFieldPointer(pField)
	{}

	virtual void set(GReflectionObjectPtr obj, const GValue& val) const override
	{
		_Class* pObj = static_cast<_Class*>(obj.get());
		pObj->*m_pFieldPointer = GValueConverter<_Ty>()(val);
	}
private:
	GObjectImplements(GFieldSetterImpl);
	FieldPointer m_pFieldPointer;
};



template<typename _Class, typename _Ty>
class GFieldSetterFunctionImpl : public GFieldSetter
{
public:
	typedef typename std::remove_all_extents<_Ty>::type type;
	typedef void (_Class::* FieldFunctionPointer)(_Ty val);
	GFieldSetterFunctionImpl(FieldFunctionPointer pField)
		:m_pFieldPointer(pField)
	{}

	virtual void set(GReflectionObjectPtr obj, const GValue& val) const override
	{
		_Class* pObj = static_cast<_Class*>(obj.get());
		(pObj->*m_pFieldPointer)(GValueConverter<type>()(val));
	}
private:
	GObjectImplements(GFieldSetterFunctionImpl);
	FieldFunctionPointer m_pFieldPointer;
};

template<typename _Class, typename _Ty>
GFieldSetterPtr GFieldSetter::Create(_Ty _Class::* pField)
{
	return new GFieldSetterImpl<_Class, _Ty>(pField);
}

template<typename _Class, typename _Ty>
GFieldSetterPtr GFieldSetter::Create(void (_Class::* pField)(_Ty val))
{
	return new GFieldSetterFunctionImpl<_Class, _Ty>(pField);
}
#endif