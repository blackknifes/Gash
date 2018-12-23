#include "GField.h"

GField::GField(const char* name, GFieldGetterPtr getter, GFieldSetterPtr setter)
	:m_strName(name), m_pGetter(getter), m_pSetter(setter)
{

}

const char* GField::getName() const
{
	return m_strName.data();
}

GValue GField::getValue(GReflectionObjectPtr pObject) const
{
	return m_pGetter->get(pObject);
}

void GField::setValue(GReflectionObjectPtr pObject, const GValue& val) const
{
	return m_pSetter->set(pObject, val);
}
