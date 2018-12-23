#ifndef __GREFLECTIONMANAGER_H__
#define __GREFLECTIONMANAGER_H__
#include "GClass.h"

#define _reflectionManager GReflectionManager::getInstance()

class GReflectionManager
{
public:
	typedef std::unordered_map<std::string, GClassPtr> ClassMap;
	static GReflectionManager* getInstance();
	static void destroyInstance();

	void addClass(GClassPtr pClass);
	void removeClass(GClassPtr pClass);
	void removeClass(const char* name);

	GClassPtr findClass(const char* name) const;
private:
	ClassMap m_ClassMap;
	static GReflectionManager*  s_pInstance;
};
#endif