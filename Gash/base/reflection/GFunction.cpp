#include "GFunction.h"

GValue GFunction::invoke(GReflectionObjectPtr pObject, const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4, const GValue& arg5, const GValue& arg6, const GValue& arg7, const GValue& arg8, const GValue& arg9) const 
{
	return GValue();
}

GValue GFunction::invoke(GReflectionObjectPtr pObject, const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4, const GValue& arg5, const GValue& arg6, const GValue& arg7, const GValue& arg8) const 
{
	return GValue();
}

GValue GFunction::invoke(GReflectionObjectPtr pObject, const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4, const GValue& arg5, const GValue& arg6, const GValue& arg7) const 
{
	return GValue();
}

GValue GFunction::invoke(GReflectionObjectPtr pObject, const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4, const GValue& arg5, const GValue& arg6) const 
{
	return GValue();
}

GValue GFunction::invoke(GReflectionObjectPtr pObject, const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4, const GValue& arg5) const 
{
	return GValue();
}

GValue GFunction::invoke(GReflectionObjectPtr pObject, const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4) const 
{
	return GValue();
}

GValue GFunction::invoke(GReflectionObjectPtr pObject, const GValue& arg1, const GValue& arg2, const GValue& arg3) const 
{
	return GValue();
}

GValue GFunction::invoke(GReflectionObjectPtr pObject, const GValue& arg1, const GValue& arg2) const 
{
	return GValue();
}

GValue GFunction::invoke(GReflectionObjectPtr pObject, const GValue& arg1) const 
{
	return GValue();
}

GValue GFunction::invoke(GReflectionObjectPtr pObject, GValue& arg1) const 
{
	return GValue();
}

GValue GFunction::invoke(GReflectionObjectPtr pObject) const 
{
	return GValue();
}

GFunction::GFunction(const char* name, const char* signature)
	:m_strName(name), m_strSignature(signature)
{
	m_sSignatureHash = std::hash<std::string>()(m_strSignature);
}

GFunctionPtr GFunction::findBySignature(const char* signature) const
{
	size_t hashCode = std::_Hash_array_representation(signature, strlen(signature));
	GFunctionPtr pFunc = (GFunction*)this;
	while (pFunc != nullptr && pFunc->m_sSignatureHash != hashCode && pFunc->m_strName != signature)
		pFunc = pFunc->m_pNextFunction;
	return pFunc;
}

const char* GFunction::getName() const
{
	return m_strName.data();
}

const char* GFunction::getSignature() const
{
	return m_strSignature.data();
}

void GFunction::addOverride(GFunctionPtr pFunction)
{
	GFunctionPtr pFunc = this;
	while (pFunc->m_pNextFunction != nullptr)
		pFunc = pFunc->m_pNextFunction;
	pFunc->m_pNextFunction = pFunction;
}
