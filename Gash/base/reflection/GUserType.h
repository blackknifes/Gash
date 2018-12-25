#ifndef __GUSERTYPE_H__
#define __GUSERTYPE_H__
#include <unordered_map>
#include <string>
#include <guiddef.h>
#include <Windows.h>
#include "..\GObject.h"

GUID GuidFromString(const char* str);

GREFPTR_DEF(GUserType);

class GValue;

template<typename _Ty>
struct GUserTypeGetGuid
{
	GUID operator()() const
	{
		return __uuidof(_Ty);
	}
};

class GUserType: public virtual GObject
{
public:
	typedef void(*Constructor)(void* pData);
	typedef void(*Cloner)(void* pDest, const void* pSrc);
	typedef void(*Mover)(void* pDest, void* pSrc);
	typedef void(*Destructor)(void* pData);
	typedef bool(*Comparetor)(const void* pLeft, const void* pRight);

	static bool RegisterUserType(
		const char* name,
		const GUID& typeId,
		size_t size, 
		Constructor pConstructor = nullptr, 
		Destructor pDestructor = nullptr,
		Cloner pCloner = nullptr,
		Mover pMover = nullptr,
		Comparetor pComparetor = nullptr);
	static void UnregisterUserType(const GUID& typeId);
	static void UnregisterUserType(const char* name);

	template<typename _Ty>
	static GUserTypePtr GetUserType()
	{
		return FindUserType(GUserTypeGetGuid<_Ty>()());
	}

	static GUserTypePtr FindUserType(const GUID& guid);
	static GUserTypePtr FindUserType(const char* name);

	const char* getName() const;
	const GUID& getTypeId() const;
	size_t getSize() const;

	void construct(void* pData) const;
	void destruct(void* pData) const;
	void clone(void* pDest, const void* pSrc) const;
	void move(void* pDest, void* pSrc) const;
	bool compare(const void* pLeft, const void* pRight) const;
	bool canConstructor() const;
	bool canDestructor() const;
	bool canClone() const;
	bool canMove() const;
	bool canCompare() const;
private:
	GObjectImplements(GUserType);
	std::string m_strName;
	GUID m_userTypeId;
	size_t m_sSizeOfUserData;
	Constructor m_pConstructor;
	Destructor m_pDestructor;
	Cloner m_pCloner;
	Mover m_pMover;
	Comparetor m_pComparetor;
};

#define BaseTypeTypeNameDef(type) \
template<> \
struct GReflectionGetTypeName<type> \
{ \
	const char* operator()() const \
	{ \
		return #type; \
	} \
};

template<typename _Ty>
struct GReflectionGetTypeNameBase
{
	static const char* s_strName;
};

template<typename _Ty>
struct GReflectionGetTypeName
{
	const char* operator()() const
	{
		GUserTypePtr userType = GUserType::GetUserType<_Ty>();
		if (userType == nullptr)
			return "unknown";
		return userType->getName();
	}
};

#define GClassGUID_DEF(type, guid) \
template<> struct GUserTypeGetGuid<type> \
{ \
	GUID operator()() const { return GuidFromString(guid); } \
};

BaseTypeTypeNameDef(void);
BaseTypeTypeNameDef(bool);
BaseTypeTypeNameDef(char);
BaseTypeTypeNameDef(unsigned char);
BaseTypeTypeNameDef(short);
BaseTypeTypeNameDef(unsigned short);
BaseTypeTypeNameDef(int);
BaseTypeTypeNameDef(unsigned int);
BaseTypeTypeNameDef(__int64);
BaseTypeTypeNameDef(unsigned __int64);
BaseTypeTypeNameDef(float);
BaseTypeTypeNameDef(double);

BaseTypeTypeNameDef(RECT);
BaseTypeTypeNameDef(POINT);
BaseTypeTypeNameDef(SIZE);
BaseTypeTypeNameDef(POINTS);

template<typename _Ty>
struct GReflectionGetTypeName<_Ty*>
{
	const char* operator()() const
	{
		if (GReflectionGetTypeNameBase<_Ty*>::s_strName == nullptr)
		{
			char buf[1024] = {};
			sprintf_s(buf, "%s*", GReflectionGetTypeName<_Ty>()());
			size_t len = strlen(buf);
			char* str = new char[len + 1];
			strcpy_s(str, len + 1, buf);
			GReflectionGetTypeNameBase<_Ty*>::s_strName = str;
		}
		return GReflectionGetTypeNameBase<_Ty*>::s_strName;
	}
};

template<typename _Ty>
struct GReflectionGetTypeName<_Ty* const>
{
	const char* operator()() const
	{
		if (GReflectionGetTypeNameBase<_Ty* const>::s_strName == nullptr)
		{
			char buf[1024] = {};
			sprintf_s(buf, "%s* const", GReflectionGetTypeName<_Ty>()());
			size_t len = strlen(buf);
			char* str = new char[len + 1];
			strcpy_s(str, len + 1, buf);
			GReflectionGetTypeNameBase<_Ty* const>::s_strName = str;
		}
		return GReflectionGetTypeNameBase<_Ty* const>::s_strName;
	}
};

template<typename _Ty>
struct GReflectionGetTypeName<_Ty&>
{
	const char* operator()() const
	{
		if (GReflectionGetTypeNameBase<_Ty&>::s_strName == nullptr)
		{
			char buf[1024] = {};
			sprintf_s(buf, "%s&", GReflectionGetTypeName<_Ty>()());
			size_t len = strlen(buf);
			char* str = new char[len + 1];
			strcpy_s(str, len + 1, buf);
			GReflectionGetTypeNameBase<_Ty&>::s_strName = str;
		}
		return GReflectionGetTypeNameBase<_Ty&>::s_strName;
	}
};

template<typename _Ty>
struct GReflectionGetTypeName<_Ty&&>
{
	const char* operator()() const
	{
		if (GReflectionGetTypeNameBase<_Ty&&>::s_strName == nullptr)
		{
			char buf[1024] = {};
			sprintf_s(buf, "%s&&", GReflectionGetTypeName<_Ty>()());
			size_t len = strlen(buf);
			char* str = new char[len + 1];
			strcpy_s(str, len + 1, buf);
			GReflectionGetTypeNameBase<_Ty&&>::s_strName = str;
		}
		return GReflectionGetTypeNameBase<_Ty&&>::s_strName;
	}
};

template<typename _Ty>
struct GReflectionGetTypeName<const _Ty>
{
	const char* operator()() const
	{
		if (GReflectionGetTypeNameBase<const _Ty>::s_strName == nullptr)
		{
			char buf[1024] = {};
			sprintf_s(buf, "const %s", GReflectionGetTypeName<_Ty>()());
			size_t len = strlen(buf);
			char* str = new char[len + 1];
			strcpy_s(str, len + 1, buf);
			GReflectionGetTypeNameBase<const _Ty>::s_strName = str;
		}
		return GReflectionGetTypeNameBase<const _Ty>::s_strName;
	}
};

template<typename _Ty>
__declspec(selectany) const char* GReflectionGetTypeNameBase<_Ty>::s_strName 
	= GReflectionGetTypeName<_Ty>()();

#endif