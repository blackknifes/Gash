#include "GReflectionManager.h"

GReflectionManager* GReflectionManager::s_pInstance = nullptr;

GReflectionManager* GReflectionManager::getInstance()
{
	if (s_pInstance == nullptr)
		s_pInstance = new GReflectionManager();
	return s_pInstance;
}

void GReflectionManager::destroyInstance()
{
	if (s_pInstance)
	{
		delete s_pInstance;
		s_pInstance = nullptr;
	}
}

void GReflectionManager::addClass(GClassPtr pClass)
{
	m_ClassMap.insert(ClassMap::value_type(pClass->getName(), pClass));
}

void GReflectionManager::removeClass(GClassPtr pClass)
{
	auto itor = m_ClassMap.find(pClass->getName());
	if (itor != m_ClassMap.end())
		m_ClassMap.erase(itor);
}

void GReflectionManager::removeClass(const char* name)
{
	auto itor = m_ClassMap.find(name);
	if (itor != m_ClassMap.end())
		m_ClassMap.erase(itor);
}

GClassPtr GReflectionManager::findClass(const char* name) const
{
	auto itor = m_ClassMap.find(name);
	if (itor == m_ClassMap.end())
		return nullptr;
	return itor->second;
}
