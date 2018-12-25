#ifndef __GCONSTRUCTOR_H__
#define __GCONSTRUCTOR_H__

#include "GReflectionBase.h"
#include "GFunctionTraits.h"

GREFPTR_DEF(GConstructor);

class GConstructor: public virtual GObject
{
public:
	template<typename _Ty, typename... _argsTy>
	static GConstructorPtr Create();

	GConstructor(const char* signature);
	virtual GObjectPtr newInstance() const;
	virtual GObjectPtr newInstance(GValue& arg1) const;
	virtual GObjectPtr newInstance(const GValue& arg1) const;
	virtual GObjectPtr newInstance(const GValue& arg1, const GValue& arg2) const;
	virtual GObjectPtr newInstance(const GValue& arg1, const GValue& arg2, const GValue& arg3) const;
	virtual GObjectPtr newInstance(
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4) const;
	virtual GObjectPtr newInstance(
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4,
		const GValue& arg5) const;
	virtual GObjectPtr newInstance(
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4,
		const GValue& arg5,
		const GValue& arg6) const;
	virtual GObjectPtr newInstance(
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4,
		const GValue& arg5,
		const GValue& arg6,
		const GValue& arg7) const;
	virtual GObjectPtr newInstance(
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4,
		const GValue& arg5,
		const GValue& arg6,
		const GValue& arg7,
		const GValue& arg8) const;
	virtual GObjectPtr newInstance(
		const GValue& arg1,
		const GValue& arg2,
		const GValue& arg3,
		const GValue& arg4,
		const GValue& arg5,
		const GValue& arg6,
		const GValue& arg7,
		const GValue& arg8,
		const GValue& arg9) const;

	const char* getSignature() const;
	GConstructorPtr findBySignature(const char* signature) const;
	void addOverride(GConstructorPtr pConstructor);
private:
	GConstructorPtr m_pNextConstructor;
	std::string m_strSignature;
	size_t m_sSignatureHash;
};

template<typename _Ty, typename... _argsTy>
class GConstructorImpl : public GConstructor
{
public:
	GConstructorImpl()
		:GConstructor(GGetTypesString<_argsTy...>().data())
	{
		
	}
	virtual GObjectPtr newInstance(typename GTypeExpand<const GValue&, _argsTy>::type... args) const override
	{
		return new _Ty(GValueConverter<_argsTy>()(args)...);
	}
private:
	GObjectImplements(GConstructorImpl);
};

template<typename _Ty, typename... _argsTy>
GConstructorPtr GConstructor::Create()
{
	return new GConstructorImpl<_Ty, _argsTy...>();
}

#endif