#include "GEncoding.h"
#include "iconv.h"
#include "localcharset.h"
#include <map>
#include <Windows.h>

static const char* GetCharsetName(GEncoding charset)
{
	switch (charset)
	{
	case EncodingUtf8:
		return iconv_canonicalize("UTF-8");
	case EncodingAnsi:
		return locale_charset();
	case EncodingGBK:
		return iconv_canonicalize("GBK");
	case EncodingGB2312:
		return iconv_canonicalize("GB2312");
	case EncodingUtf16LE:
		return iconv_canonicalize("UTF-16LE");
	case EncodingUtf16BE:
		return iconv_canonicalize("UTF-16BE");
	default:
		break;
	}
	return nullptr;
}

struct IConvContext
{
	typedef std::pair<GEncoding, GEncoding> GCharsetKey;
	typedef std::map<GCharsetKey, iconv_t> GIconvMap;
	GIconvMap m_iconvMap;

	IConvContext()
	{

	}

	~IConvContext()
	{
		for (auto& item: m_iconvMap)
			iconv_close(item.second);
		m_iconvMap.clear();
	}

	iconv_t findIconv(GEncoding from, GEncoding to)
	{
		GCharsetKey key(from, to);
		auto itor = m_iconvMap.find(key);
		if (itor == m_iconvMap.end())
		{
			const char* fromStr = GetCharsetName(from);
			const char* toStr = GetCharsetName(to);
			if (fromStr == nullptr || toStr == nullptr)
				return nullptr;
			iconv_t pIconv = iconv_open(toStr, fromStr);
			int param = 1;
			iconvctl(pIconv, ICONV_SET_DISCARD_ILSEQ, &param);
			iconvctl(pIconv, ICONV_SET_TRANSLITERATE, &param);
// 			ICONV_SET_HOOKS;
// 			ICONV_SET_FALLBACKS;
			if (pIconv == (iconv_t)-1)
				return nullptr;
			itor = m_iconvMap.insert(GIconvMap::value_type(key, pIconv)).first;
		}
		return itor->second;
	}
};

static __declspec(thread) IConvContext s_IConvContext;

errno_t EncodeString(GEncoding from, GEncoding to, const void** src, size_t* srcsize, void** dest, size_t* destsize)
{
	return EncodeString(FindEncoder(from, to), src, srcsize, dest, destsize);
}

errno_t EncodeString(Encoder* pEncoder, const void** src, size_t* srcsize, void** dest, size_t* destsize)
{
	if (pEncoder == nullptr)
		return EFAULT;
	iconv((iconv_t)pEncoder, (const char**)src, srcsize, (char**)dest, destsize);
// 	int result = errno;
// 	switch (result)
// 	{
// 	case EINVAL:
// 	case E2BIG:
// 	case EILSEQ:
// 		return result;
// 	default:
// 		break;
// 	}
	return errno;
}

bool EncodeIsCritical(errno_t err)
{
	switch (err)
	{
	case E2BIG:
	case EILSEQ:
	case NOERROR:
		return false;
	default:
		break;
	}
	return true;
}

Encoder* FindEncoder(GEncoding from, GEncoding to)
{
	iconv_t pIconv = s_IConvContext.findIconv(from, to);
	return (Encoder*)pIconv;
}
