#ifndef __GREFLECTIONOBJECT_H__
#define __GREFLECTIONOBJECT_H__
#include "../GObject.h"

#ifndef GReflectionType
#define GReflectionType GObject
#endif
typedef GReflectionType GReflectionObject;

typedef GScopePointer<GReflectionObject> GReflectionObjectPtr;
#endif