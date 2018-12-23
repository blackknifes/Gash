#ifndef __GWIN32BASETYPEREGISTRY_H__
#define __GWIN32BASETYPEREGISTRY_H__
#include <Windows.h>
#include "reflection/GUserTypeRegister.h"

struct __declspec(uuid("264A9FB1-53A1-44DB-8BB6-EEC14ED8872A")) tagRECT;
struct __declspec(uuid("8A2192D5-6917-4F6A-88D4-836A1F4E61C6")) tagPOINT;
struct __declspec(uuid("64B07557-311F-4327-998A-E761C9175646")) tagSIZE;
struct __declspec(uuid("E22134F2-42AB-4DFE-B994-C184C13A5CCA")) tagPOINTS;

bool _CompareUCharArray(const void* pLeft, const void* pRight, size_t size);

#define Win32_BaseType_Def(baseType, name) \
template<> struct GUserTypeCompare<baseType> \
{ \
	typedef typename baseType type; \
	static bool call(const type& left, const type& right) \
	{ \
		return _CompareUCharArray(&left, &right, sizeof(baseType)); \
	} \
}; \
GUserTypeRegister(name)

Win32_BaseType_Def(tagRECT, RECT);
Win32_BaseType_Def(tagPOINT, POINT);
Win32_BaseType_Def(tagSIZE, SIZE);
Win32_BaseType_Def(tagPOINTS, POINTS);

void Win32BaseTypeRegister();
#endif