#ifndef __GCLASS_H__
#define __GCLASS_H__

#include "GReflectionBase.h"
#include "GConstructor.h"
#include "GFunction.h"
#include "GField.h"
#include "GFunctionTraits.h"

GREFPTR_DEF(GClass);

class GClass: public GObject
{
public:
	typedef std::unordered_map<std::string, GFunctionPtr> FunctionMap;
	typedef std::unordered_map<std::string, GFieldPtr> FieldMap;
	typedef std::vector<GValue> ArgumentList;

	GClass(const char* name);
	const char* getName() const;
	std::vector<GClassPtr> getSuperClasses() const;
	GClassPtr findSuperClass(const char* name) const;

	GConstructorPtr getConstructor() const;
	GConstructorPtr findConstructor(const char* signature) const;

	GFunctionPtr getFunction(const char* name) const;
	GFunctionPtr findFunction(const char* signature) const;

	GFunctionPtr getStaticFunction(const char* name) const;
	GFunctionPtr findStaticFunction(const char* signature) const;

	GFieldPtr getField(const char* name) const;

	GValue getValue(const char* name, GReflectionObjectPtr pObject) const;
	void setValue(const char* name, GReflectionObjectPtr pObject, const GValue& val) const;

	GValue apply(const char* signature, GObjectPtr pObject) const;
	GValue apply(const char* signature, GObjectPtr pObject, const ArgumentList& args) const;

	template<typename... _argsTy>
	GObjectPtr newInstance(_argsTy... args)
	{
		GConstructorPtr pConstructor = findConstructor(GGetTypesString<_argsTy...>().data());
		if (pConstructor == nullptr)
			return nullptr;
		return pConstructor->newInstance(GValueConverter<_argsTy>()(args)...);
	}

	template<typename... _argsTy>
	GValue call(const char* name, GObjectPtr pObject, _argsTy... args)
	{
		GFunctionPtr pFunc;
		if (pObject == nullptr)
			pFunc = findStaticFunction(GGetFunctionSignature<_argsTy...>(name).data());
		else
			pFunc = findFunction(GGetFunctionSignature<_argsTy...>(name).data());
		if (pFunc == nullptr)
			return GValue();
		return pFunc->call(pObject, std::move(args)...);
	}

	template<typename _Ty> friend class GClassRegistry;
private:
	GObjectImplements(GClass);
	std::string m_strName;
	std::vector<std::string> m_strSuperClassNames;
	GConstructorPtr m_pConstructor;
	FunctionMap m_functionMap;
	FunctionMap m_staticFunctionMap;
	FieldMap m_fieldMap;
};
#endif