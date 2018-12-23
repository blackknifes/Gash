#include "GConstructor.h"


GConstructor::GConstructor(const char* signature)
	:m_strSignature(signature)
{
	m_sSignatureHash = std::hash<std::string>()(m_strSignature);
}

GObjectPtr GConstructor::newInstance() const
{
	return nullptr;
}

GObjectPtr GConstructor::newInstance(GValue& arg1) const
{
	return nullptr;
}

GObjectPtr GConstructor::newInstance(const GValue& arg1) const
{
	return nullptr;
}

GObjectPtr GConstructor::newInstance(const GValue& arg1, const GValue& arg2) const
{
	return nullptr;
}

GObjectPtr GConstructor::newInstance(const GValue& arg1, const GValue& arg2, const GValue& arg3) const
{
	return nullptr;
}

GObjectPtr GConstructor::newInstance(const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4) const
{
	return nullptr;
}

GObjectPtr GConstructor::newInstance(const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4, const GValue& arg5) const
{
	return nullptr;
}

GObjectPtr GConstructor::newInstance(const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4, const GValue& arg5, const GValue& arg6) const
{
	return nullptr;
}

GObjectPtr GConstructor::newInstance(const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4, const GValue& arg5, const GValue& arg6, const GValue& arg7) const
{
	return nullptr;
}

GObjectPtr GConstructor::newInstance(const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4, const GValue& arg5, const GValue& arg6, const GValue& arg7, const GValue& arg8) const
{
	return nullptr;
}

GObjectPtr GConstructor::newInstance(const GValue& arg1, const GValue& arg2, const GValue& arg3, const GValue& arg4, const GValue& arg5, const GValue& arg6, const GValue& arg7, const GValue& arg8, const GValue& arg9) const
{
	return nullptr;
}

const char* GConstructor::getSignature() const
{
	return m_strSignature.data();
}

GConstructorPtr GConstructor::findBySignature(const char* signature) const
{
	size_t hashCode = std::_Hash_array_representation(signature, strlen(signature));
	GConstructorPtr pFunc = (GConstructor*)this;
	while (pFunc != nullptr && pFunc->m_sSignatureHash != hashCode && pFunc->m_strSignature != signature)
		pFunc = pFunc->m_pNextConstructor;
	return pFunc;
}

void GConstructor::addOverride(GConstructorPtr pConstructor)
{
	GConstructorPtr pFunc = this;
	while (pFunc->m_pNextConstructor != nullptr)
		pFunc = pFunc->m_pNextConstructor;
	pFunc->m_pNextConstructor = pConstructor;
}

