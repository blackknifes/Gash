#ifndef __GREFLECTIONBASE_H__
#define __GREFLECTIONBASE_H__

#include "../GValue.h"
#include "../GObject.h"
#include "../GValueConverter.h"
#include "GReflectionObject.h"
#include <type_traits>

template<typename _Ty, typename _Ty2>
struct GTypeExpand
{
	typedef _Ty type;
};
#endif