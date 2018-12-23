#ifndef __GUSERTYPEREGISTER_H__
#define __GUSERTYPEREGISTER_H__
#include "GUserType.h"

template<typename _Ty>
struct GUserTypeCompare
{
	static bool call(const _Ty& left, const _Ty& right)
	{
		return left == right;
	}
};

template<typename _Ty>
struct GUserTypeGetName
{
	static const char* call()
	{
		return "";
	}
};

template<typename _Ty>
struct GUserTypeConstructor
{
	static void call(void* pData)
	{
		_Ty* pObj = (_Ty*)pData;
		new (pObj) _Ty();
	}
};

template<typename _Ty>
struct GUserTypeDestructor
{
	static void call(void* pData)
	{
		_Ty* pObj = (_Ty*)pData;
		pObj->~_Ty();
	}
};

template<typename _Ty>
struct GUserTypeCloner
{
	static void call(void* pDest, const void* pSrc)
	{
		_Ty* pDestObj = (_Ty*)pDest;
		const _Ty* pSrcObj = (const _Ty*)pSrc;
		*pDestObj = *pSrcObj;
	}
};

template<typename _Ty>
struct GUserTypeMover
{
	static void call(void* pDest, void* pSrc)
	{
		_Ty* pDestObj = (_Ty*)pDest;
		_Ty* pSrcObj = (_Ty*)pSrc;
		*pDestObj = std::move(*pSrcObj);
	}
};

template<typename _Ty>
struct GUserTypeComparetor
{
	static bool call(const void* pLeft, const void* pRight)
	{
		const _Ty* pLeftObj = (const _Ty*)pLeft;
		const _Ty* pRightObj = (const _Ty*)pRight;
		return GUserTypeCompare<_Ty>::call(*pLeftObj, *pRightObj);
	}
};

//用户类型注册器
template<
	typename _Ty,
	typename _NameGetter = GUserTypeGetName<_Ty>,
	typename _Ctor = GUserTypeConstructor<_Ty>,
	typename _Dtor = GUserTypeDestructor<_Ty>,
	typename _Cloner = GUserTypeCloner<_Ty>,
	typename _Mover = GUserTypeMover<_Ty>,
	typename _Cmp = GUserTypeComparetor<_Ty>
>
class GUserTypeRegistry
{
public:
	typename _Ty usertype;
	typename _Ctor constructor;
	typename _Dtor desstructor;
	typename _Cloner cloner;
	typename _Mover mover;
	typename _Cmp comparetor;

	GUserTypeRegistry()
	{
		GUserType::RegisterUserType(
			getName(),
			getTypeId(),
			getSize(),
			getConstructor(),
			getDestructor(),
			getCloner(),
			getMover(),
			getComparetor()
		);
	}

	static const char* getName()
	{
		return _NameGetter::call();
	}

	static GUID getTypeId()
	{
		return GUserTypeGetGuid<_Ty>()();
	}
	static size_t getSize()
	{
		return sizeof(_Ty);
	}
	static GUserType::Constructor getConstructor()
	{
		return _Ctor::call;
	}
	static GUserType::Destructor getDestructor()
	{
		return _Dtor::call;
	}
	static GUserType::Cloner getCloner()
	{
		return _Cloner::call;
	}
	static GUserType::Mover getMover()
	{
		return _Mover::call;
	}
	static GUserType::Comparetor getComparetor()
	{
		return _Cmp::call;
	}
};

#define GUserTypeRegister2(type, name) \
template<> struct GUserTypeGetName<type> \
{ \
	static const char* call()  { return name; } \
};

#define GUserTypeRegister(type) GUserTypeRegister2(type, #type)


#define GAddUserType(type) GUserTypeRegistry<type>();
#endif