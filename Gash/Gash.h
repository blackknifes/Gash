#ifndef __GREFLECTION_H__
#define __GREFLECTION_H__

#include "base/GTypes.h"
#include "base/GValueConverter.h"
#include "base/GWin32BaseTypeRegistry.h"

#include "base/GObject.h"
#include "base/GMemoryPool.h"
#include "base/GPath.h"
#include "base/GString.h"
#include "base/GStringPiece.h"
#include "base/GRegex.h"
#include "base/GUrlParts.h"
#include "base/GValue.h"
#include "base/GUrlParts.h"
#include "base/GMemoryBlockPool.h"
#include "base/GMemoryPool.h"

#include "base/reflection/GReflectionObject.h"
#include "base/reflection/GReflectionManager.h"
#include "base/reflection/GClassRegistry.h"

#include "base/FileDialog/GFileDialog.h"

#ifndef GREFLECTION_IMPELEMENTS
#ifdef _DEBUG
#pragma comment(lib, "Gashd.lib")
#else
#pragma comment(lib, "Gash.lib")
#endif
#endif
#endif