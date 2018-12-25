#ifndef __GFUNCTION_H__
#define __GFUNCTION_H__
#include "GReflectionBase.h"
#include "GFunctionTraits.h"

GREFPTR_DEF(GFunction);

class GFunction : public virtual GObject
{
public:
	template<typename _Class, typename _retTy, typename... _argsTy>
	static GFunctionPtr Create(const char* name, _retTy(_Class::* pFunc)(_argsTy...));
	template<typename _Class, typename... _argsTy>
	static GFunctionPtr Create(const char* name, void(_Class::* pFunc)(_argsTy...));
	template<typename _retTy, typename... _argsTy>
	static GFunctionPtr Create(const char* name, _retTy(* pFunc)(_argsTy...));
	template<typename... _argsTy>
	static GFunctionPtr Create(const char* name, void(* pFunc)(_argsTy...));
	template<typename _retTy, typename... _argsTy>
	static GFunctionPtr CreateCallback(const char* name, _retTy(__stdcall* pFunc)(_argsTy...));
	template<typename... _argsTy>
	static GFunctionPtr CreateCallback(const char* name, void(__stdcall* pFunc)(_argsTy...));

	virtual GValue invoke(GReflectionObjectPtr pObject) const;
	virtual GValue invoke(GReflectionObjectPtr pObject, GValue& arg1) const;
	virtual GValue invoke(GReflectionObjectPtr pObject, const GValue& arg1) const;
	virtual GValue invoke(GReflectionObjectPtr pObject, const GValue& arg1, const GValue& arg2) const;
	virtual GValue invoke(GReflectionObjectPtr pObject, const GValue& arg1, const GValue& arg2, const GValue& arg3) const;
	virtual GValue invoke(GReflectionObjectPtr pObject,
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4) const;
	virtual GValue invoke(GReflectionObjectPtr pObject,
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4,
		const GValue& arg5) const;
	virtual GValue invoke(GReflectionObjectPtr pObject,
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4,
		const GValue& arg5,
		const GValue& arg6) const;
	virtual GValue invoke(GReflectionObjectPtr pObject,
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4,
		const GValue& arg5,
		const GValue& arg6,
		const GValue& arg7) const;
	virtual GValue invoke(GReflectionObjectPtr pObject,
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4,
		const GValue& arg5,
		const GValue& arg6,
		const GValue& arg7,
		const GValue& arg8) const;
	virtual GValue invoke(GReflectionObjectPtr pObject,
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4,
		const GValue& arg5,
		const GValue& arg6,
		const GValue& arg7,
		const GValue& arg8,
		const GValue& arg9) const;


	template<typename... _argsTy>
	GValue call(GReflectionObjectPtr pObject, _argsTy... args) const
	{
		return this->invoke(pObject, GValueConverter<_argsTy>()(args)...);
	}

	template<typename... _argsTy>
	void callVoid(GReflectionObjectPtr pObject, _argsTy... args) const
	{
		this->invoke(pObject, GValueConverter<_argsTy>(args)...);
	}

	GFunction(const char* name, const char* signature);

	GFunctionPtr findBySignature(const char* signature) const;
	const char* getName() const;
	const char* getSignature() const;

	void addOverride(GFunctionPtr pFunction);
private:
	GFunctionPtr m_pNextFunction;
	std::string m_strName;
	std::string m_strSignature;
	size_t m_sSignatureHash;
};

template<typename _Class, typename _retTy, typename... _argsTy>
class GFunctionImpl : public GFunction
{
public:
	typedef typename _retTy(_Class::* function_type)(_argsTy...);
	GFunctionImpl(const char* name, function_type func) 
		:GFunction(name, GGetFunctionSignature<_argsTy...>(name).data()), 
		m_pFunction(func) {};

	virtual GValue invoke(GReflectionObjectPtr pObject,
		const typename GTypeExpand<GValue, _argsTy>::type&... args) const
	{
		_Class* pObj = static_cast<_Class*>(pObject.get());
		return GValueConverter<_retTy>()(
			(pObj->*m_pFunction)(GValueConverter<_argsTy>()(args)...)
			);
	}
private:
	GObjectImplements(GFunctionImpl);
	function_type m_pFunction;
};

template<typename _Class, typename... _argsTy>
class GFunctionVoidImpl : public GFunction
{
public:
	typedef typename void(_Class::* function_type)(_argsTy...);
	GFunctionVoidImpl(const char* name, function_type func) 
		:GFunction(name, GGetFunctionSignature<_argsTy...>(name).data()),
		m_pFunction(func) {};

	virtual GValue invoke(GReflectionObjectPtr pObject,
		const typename GTypeExpand<GValue, _argsTy>::type&... args) const
	{
		_Class* pObj = static_cast<_Class*>(pObject.get());
		(pObj->*m_pFunction)(GValueConverter<_argsTy>()(args)...);
		return GValue();
	}
private:
	GObjectImplements(GFunctionVoidImpl);
	function_type m_pFunction;
};

template<typename _retTy, typename... _argsTy>
class GStaticFunctionImpl : public GFunction
{
public:
	typedef typename _retTy(*function_type)(_argsTy...);
	GStaticFunctionImpl(const char* name, function_type func) 
		:GFunction(name, GGetFunctionSignature<_argsTy...>(name).data()),
		m_pFunction(func) {};

	virtual GValue invoke(GReflectionObjectPtr pObject,
		const typename GTypeExpand<GValue, _argsTy>::type&... args) const
	{
		return GValueConverter<_retTy>()(
			m_pFunction(GValueConverter<_argsTy>()(args)...)
			);
	}
private:
	GObjectImplements(GStaticFunctionImpl);
	function_type m_pFunction;
};

template<typename _Class, typename... _argsTy>
class GStaticFunctionVoidImpl : public GFunction
{
public:
	typedef typename void(*function_type)(_argsTy...);
	GStaticFunctionVoidImpl(const char* name, function_type func) 
		:GFunction(name, GGetFunctionSignature<_argsTy...>(name).data()),
		m_pFunction(func) {};

	virtual GValue invoke(GReflectionObjectPtr pObject,
		const typename GTypeExpand<GValue, _argsTy>::type&... args) const
	{
		m_pFunction(GValueConverter<_argsTy>()(args)...);
		return GValue();
	}
private:
	GObjectImplements(GStaticFunctionVoidImpl);
	function_type m_pFunction;
};

template<typename _retTy, typename... _argsTy>
class GStaticCallbackImpl : public GFunction
{
public:
	typedef typename _retTy(__stdcall* function_type)(_argsTy...);
	GStaticCallbackImpl(const char* name, function_type func) 
		:GFunction(name, GGetFunctionSignature<_argsTy...>(name).data()),
		m_pFunction(func) {};

	virtual GValue invoke(GReflectionObjectPtr pObject,
		const typename GTypeExpand<GValue, _argsTy>::type&... args) const
	{
		return GValueConverter<_retTy>()(
			m_pFunction(GValueConverter<_argsTy>()(args)...)
			);
	}
private:
	GObjectImplements(GStaticCallbackImpl);
	function_type m_pFunction;
};

template<typename _Class, typename... _argsTy>
class GStaticCallbackVoidImpl : public GFunction
{
public:
	typedef typename void(__stdcall* function_type)(_argsTy...);
	GStaticCallbackVoidImpl(const char* name, function_type func) 
		:GFunction(name, GGetFunctionSignature<_argsTy...>(name).data()),
		m_pFunction(func) {};

	virtual GValue invoke(GReflectionObjectPtr pObject,
		const typename GTypeExpand<GValue, _argsTy>::type&... args) const
	{
		m_pFunction(GValueConverter<_argsTy>()(args)...);
		return GValue();
	}
private:
	GObjectImplements(GStaticCallbackVoidImpl);
	function_type m_pFunction;
};

template<typename _Class, typename _retTy, typename... _argsTy>
GFunctionPtr GFunction::Create(const char* name, _retTy(_Class::* pFunc)(_argsTy...))
{
	return new GFunctionImpl<_Class, _retTy, _argsTy...>(name, pFunc);
}

template<typename _Class, typename... _argsTy>
GFunctionPtr GFunction::Create(const char* name, void(_Class::* pFunc)(_argsTy...))
{
	return new GFunctionVoidImpl<_Class, _argsTy...>(name, pFunc);
}

template<typename _retTy, typename... _argsTy>
static GFunctionPtr GFunction::Create(const char* name, _retTy(*pFunc)(_argsTy...))
{
	return new GStaticFunctionImpl(name, pFunc);
}
template<typename... _argsTy>
static GFunctionPtr GFunction::Create(const char* name, void(*pFunc)(_argsTy...))
{
	return new GStaticFunctionVoidImpl(name, pFunc);
}
template<typename _retTy, typename... _argsTy>
static GFunctionPtr GFunction::CreateCallback(const char* name, _retTy(__stdcall* pFunc)(_argsTy...))
{
	return new GStaticCallbackImpl(name, pFunc);
}
template<typename... _argsTy>
static GFunctionPtr GFunction::CreateCallback(const char* name, void(__stdcall* pFunc)(_argsTy...))
{
	return new GStaticCallbackVoidImpl(name, pFunc);
}
#endif