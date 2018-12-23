#ifndef __GFIELD_H__
#define __GFIELD_H__
#include "GReflectionBase.h"
#include "GFieldGetter.h"
#include "GFieldSetter.h"

GREFPTR_DEF(GField);

class GField: public GObject
{
public:
	template<typename _Class, typename _Ty>
	static GFieldPtr Create(
		const char* name, 
		_Ty _Class::* pField)
	{
		return new GField(
			name, 
			GFieldGetter::Create(pField), 
			GFieldSetter::Create(pField));
	}
	template<typename _Class, typename _Ty, typename _setTy>
	static GFieldPtr Create(
		const char* name,
		_Ty(_Class::* pGetter)() const, 
		void(_Class::* pSetter)(_setTy val))
	{
		return new GField(name, 
			GFieldGetter::Create(pGetter), 
			GFieldSetter::Create(pSetter));
	}

	GField(const char* name, GFieldGetterPtr getter, GFieldSetterPtr setter);

	const char* getName() const;
	GValue getValue(GReflectionObjectPtr pObject) const;
	void setValue(GReflectionObjectPtr pObject, const GValue& val) const;

	template<typename _Ty>
	void set(GReflectionObjectPtr pObject, const _Ty& val) const
	{
		setValue(pObject, GValueConverter<_Ty>()(val));
	}

	template<typename _Ty>
	_Ty get(GReflectionObjectPtr pObject) const
	{
		return GValueConverter<_Ty>()(getValue(pObject));
	}
private:
	GObjectImplements(GConstructorImpl);
	std::string m_strName;
	GFieldGetterPtr m_pGetter;
	GFieldSetterPtr m_pSetter;
};
#endif