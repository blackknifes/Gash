#ifndef __GCLASSREGISTRY_H__
#define __GCLASSREGISTRY_H__

#include "GClass.h"
#include "GReflectionManager.h"

template<typename _Class>
class GClassRegistry
{
public:
	typedef GClass::FunctionMap FunctionMap;
	typedef GClass::FieldMap FieldMap;

	GClassRegistry(const char* name)
	{
		m_pClass = new GClass(name);
	}

	GClassRegistry& addDefaultConstructor()
	{
		if (m_pClass->m_pConstructor == nullptr)
		{
			m_pClass->m_pConstructor = GConstructor::Create<_Class>();
			return *this;
		}
		m_pClass->m_pConstructor->addOverride(GConstructor::Create<_Class>());
		return *this;
	}

	template<typename... _argsTy>
	GClassRegistry& addConstructor()
	{
		if (m_pClass->m_pConstructor == nullptr)
		{
			m_pClass->m_pConstructor = GConstructor::Create<_Class, _argsTy...>();
			return *this;
		}
		m_pClass->m_pConstructor->addOverride(GConstructor::Create<_Class, _argsTy...>());
		return *this;
	}

	template<typename _fnTy>
	GClassRegistry& addFunction(const char* name, _fnTy func)
	{
		if (std::is_member_function_pointer<_fnTy>::value)
		{
			GFunctionPtr newFunc = GFunction::Create(name, func);
			auto itor = m_pClass->m_functionMap.find(name);
			if (itor == m_pClass->m_functionMap.end())
				m_pClass->m_functionMap.insert(FunctionMap::value_type(name, newFunc));
			else
				itor->second->addOverride(newFunc);
		}
		else
		{
			GFunctionPtr newFunc = GFunction::Create(name, func);
			auto itor = m_pClass->m_staticFunctionMap.find(name);
			if (itor == m_pClass->m_staticFunctionMap.end())
				m_pClass->m_staticFunctionMap.insert(FunctionMap::value_type(name, newFunc));
			else
				itor->second->addOverride(newFunc);
		}
		return *this;
	}

	template<typename _Ty>
	GClassRegistry& addField(const char* name, _Ty _Class::* pField)
	{
		m_pClass->m_fieldMap.insert(FieldMap::value_type(name, GField::Create(name, pField)));
		return *this;
	}
	template<typename _Ty, typename _setTy>
	GClassRegistry& addField(
		const char* name,
		_Ty(_Class::*pGetter)() const,
		void(_Class::* pSetter)(_setTy val))
	{
		m_pClass->m_fieldMap.insert(FieldMap::value_type(name, 
			GField::Create(name, pGetter, pSetter)));
		return *this;
	}

	void end()
	{
		_reflectionManager->addClass(m_pClass);
	}

private:
	GClassPtr m_pClass;
};
#endif