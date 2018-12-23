#ifndef __GFUNCTIONTRAITS_H__
#define __GFUNCTIONTRAITS_H__
#include <string>
#include "GUserType.h"

template<typename... _argsTy>
std::string GGetTypesString()
{
	char buf[128] = "";
	size_t offset = 0;
	for (size_t i = 0; i < sizeof...(_argsTy); i++)
	{
		strcat_s(buf, "%s,");
		offset += 3;
	}
	if (sizeof...(_argsTy) != 0)
		buf[offset - 1] = 0;

	char buf2[2048];
	sprintf_s(buf2, buf, GReflectionGetTypeName<_argsTy>()()...);
	return buf2;
}

template<typename... _argsTy>
std::string GGetFunctionSignature(const char* funcName)
{
	char buf[128] = "%s(";
	size_t offset = 3;
	for (size_t i = 0; i < sizeof...(_argsTy); i++)
	{
		strcat_s(buf, "%s,");
		offset += 3;
	}
	if (sizeof...(_argsTy) != 0)
		buf[offset - 1] = 0;
	strcat_s(buf, ")");
	char buf2[2048];
	sprintf_s(buf2, buf, funcName,
		GReflectionGetTypeName<_argsTy>()()...);
	return buf2;
}

#endif