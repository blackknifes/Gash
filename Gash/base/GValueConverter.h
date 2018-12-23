#ifndef __GVALUECONVERTER_H__
#define __GVALUECONVERTER_H__
#include "GValue.h"
#include "reflection\GUserType.h"
#include <assert.h>

template<typename _Ty>
struct GValueConverter
{
	GValue operator()(const _Ty& val) const
	{
		GUserTypePtr userType = GUserType::GetUserType<_Ty>();
		if (userType == nullptr)
			return GValue();
		return GValue::fromUserData(&val, userType);
	}
	_Ty operator()(const GValue& val) const
	{
		_Ty tmp;
		GUserTypePtr userType = GUserType::GetUserType<_Ty>();
		if (val.getUserType()->getTypeId() != userType->getTypeId())
		{
			memset(&tmp, 0, sizeof(_Ty));
			return tmp;
		}
		val.getUserData(&tmp, sizeof(tmp));
		return std::move(tmp);
	}
};

template<typename _Ty>
struct GValueConverter<_Ty&> : public GValueConverter<_Ty>
{};
template<typename _Ty>
struct GValueConverter<const _Ty> : public GValueConverter<_Ty>
{};
template<typename _Ty>
struct GValueConverter<volatile _Ty> : public GValueConverter<_Ty>
{};

template<>
struct GValueConverter<bool>
{
	GValue operator()(bool val) const
	{
		return GValue::fromBool(val);
	}
	bool operator()(const GValue& val) const
	{
		if (!val.isValueType())
			return false;
		return val.getBool();
	}
};

template<>
struct GValueConverter<char>
{
	GValue operator()(char val) const
	{
		return GValue::fromChar(val);
	}
	char operator()(const GValue& val) const
	{
		if (!val.isValueType())
			return 0;
		return val.getChar();
	}
};

template<>
struct GValueConverter<unsigned char>
{
	GValue operator()(unsigned char val) const
	{
		return GValue::fromUChar(val);
	}
	unsigned char operator()(const GValue& val) const
	{
		if (!val.isValueType())
			return 0;
		return val.getUChar();
	}
};

template<>
struct GValueConverter<short>
{
	GValue operator()(short val) const
	{
		return GValue::fromShort(val);
	}
	short operator()(const GValue& val) const
	{
		if (!val.isValueType())
			return 0;
		return val.getShort();
	}
};

template<>
struct GValueConverter<unsigned short>
{
	GValue operator()(unsigned short val) const
	{
		return GValue::fromUShort(val);
	}
	unsigned short operator()(const GValue& val) const
	{
		if (!val.isValueType())
			return 0;
		return val.getUShort();
	}
};

template<>
struct GValueConverter<int>
{
	GValue operator()(int val) const
	{
		return GValue::fromInt(val);
	}
	int operator()(const GValue& val) const
	{
		if (!val.isValueType())
			return 0;
		return val.getInt();
	}
};

template<>
struct GValueConverter<unsigned int>
{
	GValue operator()(unsigned int val) const
	{
		return GValue::fromUInt(val);
	}
	unsigned int operator()(const GValue& val) const
	{
		if (!val.isValueType())
			return 0;
		return val.getUInt();
	}
};

template<>
struct GValueConverter<float>
{
	GValue operator()(float val) const
	{
		return GValue::fromFloat(val);
	}
	float operator()(const GValue& val) const
	{
		if (!val.isValueType())
			return 0;
		return val.getFloat();
	}
};



template<>
struct GValueConverter<double>
{
	GValue operator()(double val) const
	{
		return GValue::fromDouble(val);
	}
	double operator()(const GValue& val) const
	{
		if (!val.isValueType())
			return 0;
		return val.getDouble();
	}
};

template<>
struct GValueConverter<__int64>
{
	GValue operator()(__int64 val) const
	{
		return GValue::fromInt64(val);
	}
	__int64 operator()(const GValue& val) const
	{
		if (!val.isValueType())
			return 0;
		return val.getInt64();
	}
};

template<>
struct GValueConverter<unsigned __int64>
{
	GValue operator()(unsigned __int64 val) const
	{
		return GValue::fromUInt64(val);
	}
	__int64 operator()(const GValue& val) const
	{
		if (!val.isValueType())
			return 0;
		return val.getUInt64();
	}
};

template<>
struct GValueConverter<GString>
{
	GValue operator()(const GString& val) const
	{
		return GValue::fromString(val);
	}
	const GString& operator()(const GValue& val) const
	{
		return val.getString();
	}
};

template<>
struct GValueConverter<GValueVector>
{
	GValue operator()(const GValueVector& val) const
	{
		return GValue::fromVector(val);
	}
	const GValueVector& operator()(const GValue& val) const
	{
		return val.getVector();
	}
};

template<>
struct GValueConverter<GValueMap>
{
	GValue operator()(const GValueMap& val) const
	{
		return GValue::fromMap(val);
	}
	const GValueMap& operator()(const GValue& val) const
	{
		return val.getMap();
	}
};

template<>
struct GValueConverter<GValueSet>
{
	GValue operator()(const GValueSet& val) const
	{
		return GValue::fromSet(val);
	}
	const GValueSet& operator()(const GValue& val) const
	{
		return val.getSet();
	}
};

template<typename _Ty>
GValue GValueConvert(_Ty val)
{
	return GValueConverter<typename std::remove_extent<_Ty>::type>()(val);
}

template<typename _Ty>
_Ty GValueConvert(const GValue& val)
{
	return GValueConverter<typename std::remove_extent<_Ty>::type>()(val);
}

#endif