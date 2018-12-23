#include "GClass.h"
#include "GReflectionManager.h"



GClass::GClass(const char* name)
	:m_strName(name)
{

}

const char* GClass::getName() const
{
	return m_strName.data();
}

std::vector<GClassPtr> GClass::getSuperClasses() const
{
	std::vector<GClassPtr> ret;
	for (const std::string& name: m_strSuperClassNames)
	{
		GClassPtr pClass = _reflectionManager->findClass(name.data());
		if (pClass != nullptr)
			ret.push_back(pClass);
	}
	return std::move(ret);
}

GClassPtr GClass::findSuperClass(const char* name) const
{
	GClassPtr pClass;
	for (size_t i = 0; i < m_strSuperClassNames.size(); ++i)
	{
		if (m_strSuperClassNames[i] == name)
		{
			pClass = _reflectionManager->findClass(name);
			break;
		}
	}
	return pClass;
}

GConstructorPtr GClass::getConstructor() const
{
	return m_pConstructor;
}

GConstructorPtr GClass::findConstructor(const char* signature) const
{
	if (m_pConstructor == nullptr)
		return nullptr;
	return m_pConstructor->findBySignature(signature);
}

GFunctionPtr GClass::getFunction(const char* name) const
{
	auto itor = m_functionMap.find(name);
	if (itor == m_functionMap.end())
		return nullptr;
	return itor->second;
}

GFunctionPtr GClass::findFunction(const char* signature) const
{
	char buf[512];
	const char* pRight = strchr(signature, '(');
	char* ptr = std::copy(signature, pRight, buf);
	*ptr = 0;
	GFunctionPtr pFunc = getFunction(buf);
	if (pFunc == nullptr)
		return nullptr;
	return pFunc->findBySignature(signature);
}

GFunctionPtr GClass::getStaticFunction(const char* name) const
{
	auto itor = m_staticFunctionMap.find(name);
	if (itor == m_staticFunctionMap.end())
		return nullptr;
	return itor->second;
}

GFunctionPtr GClass::findStaticFunction(const char* signature) const
{
	char buf[512];
	const char* pRight = strchr(signature, '(');
	std::copy(signature, pRight, buf);
	GFunctionPtr pFunc = getStaticFunction(buf);
	if (pFunc == nullptr)
		return nullptr;
	return pFunc->findBySignature(signature);
}

GFieldPtr GClass::getField(const char* name) const
{
	auto itor = m_fieldMap.find(name);
	if (itor == m_fieldMap.end())
		return nullptr;
	return itor->second;
}

GValue GClass::getValue(const char* name, GReflectionObjectPtr pObject) const
{
	GFieldPtr pField = getField(name);
	if (pField == nullptr)
		return GValue();
	return pField->getValue(pObject);
}

void GClass::setValue(const char* name, GReflectionObjectPtr pObject, const GValue& val) const
{
	GFieldPtr pField = getField(name);
	if (pField == nullptr)
		return;
	pField->setValue(pObject, val);
}

GValue GClass::apply(const char* signature, GObjectPtr pObject) const
{
	GFunctionPtr pFunc;
	if (pObject == nullptr)
		pFunc = findStaticFunction(signature);
	else
		pFunc = findFunction(signature);
	if (pFunc == nullptr)
		return GValue();
	return pFunc->invoke(pObject);
}

GValue GClass::apply(const char* signature, GObjectPtr pObject, const ArgumentList& args) const
{
	GFunctionPtr pFunc;
	if (pObject == nullptr)
		pFunc = findStaticFunction(signature);
	else
		pFunc = findFunction(signature);
	if (pFunc == nullptr)
		return GValue();
	switch (args.size())
	{
	case 0:
		return pFunc->invoke(pObject);
	case 1:
		return pFunc->invoke(pObject, args[0]);
	case 2:
		return pFunc->invoke(pObject, args[0], args[1]);
	case 3:
		return pFunc->invoke(pObject, args[0], args[1], args[2]);
	case 4:
		return pFunc->invoke(pObject, args[0], args[1], args[2], args[3]);
	case 5:
		return pFunc->invoke(pObject, args[0], args[1], args[2], args[3], args[4]);
	case 6:
		return pFunc->invoke(pObject, args[0], args[1], args[2], args[3], args[4], args[5]);
	case 7:
		return pFunc->invoke(pObject, args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
	case 8:
		return pFunc->invoke(pObject, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
	case 9:
		return pFunc->invoke(pObject, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]);
	default:
		break;
	}
	return GValue();
}
