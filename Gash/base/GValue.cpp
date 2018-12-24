#include "GValue.h"
#include "GMemoryPool.h"
#include <iostream>
#include <assert.h>
#include "GMemoryBlockPool.h"

#define TODO(msg) 

template<typename _Ty>
static void AssignGValueField(GValue* pVal, _Ty** field, GType* type, const _Ty& val, GType targetType)
{
	if (targetType == *type)
		**field = val;
	else
	{
		pVal->reset();
		*field = PopFromGlobalPool<_Ty>(val);
		*type = targetType;
	}
}

template<typename _Ty>
static void AssignGValueField(GValue* pVal, _Ty** field, GType* type, _Ty&& val, GType targetType)
{
	if (targetType == *type)
		**field = std::move(val);
	else
	{
		pVal->reset();
		*field = PopFromGlobalPool<_Ty>(std::move(val));
		*type = targetType;
	}
}

GUserData::GUserData()
{

}

GUserData::GUserData(const void* pObject, const GUID& userType)
	: m_userType(GUserType::FindUserType(userType))
{
	assert(m_userType);
	init(pObject);
}

GUserData::GUserData(const void* pObject, const char* typeName)
	: m_userType(GUserType::FindUserType(typeName))
{
	assert(m_userType);
	init(pObject);
}

GUserData::GUserData(const GUserData& obj)
{
	*this = obj.clone();
}

GUserData::GUserData(GUserData&& obj)
	:m_pObject(obj.m_pObject), m_userType(obj.m_userType)
{
	obj.m_pObject.data = nullptr;
	obj.m_pObject.size = 0;
}

GUserData::GUserData(const void* pObject, GUserTypePtr userType)
	:m_userType(userType)
{
	assert(m_userType);
	init(pObject);
}

const GUserData& GUserData::operator=(GUserData&& obj)
{
	destruct();
	m_pObject = obj.m_pObject;
	m_userType = obj.m_userType;
	
	obj.m_pObject.data = nullptr;
	obj.m_pObject.size = 0;
	return *this;
}

const GUserData& GUserData::operator=(const GUserData& obj)
{
	return *this = obj.clone();
}

GUserData::~GUserData()
{
	destruct();
}

bool GUserData::operator==(const GUserData& obj) const
{
	return compare(obj);
}

bool GUserData::isNull() const
{
	return m_pObject.data == nullptr;
}

bool GUserData::isValid() const
{
	return m_userType != nullptr;
}

void* GUserData::get() const
{
	return m_pObject.data;
}

void GUserData::construct()
{
	if (!isValid())
		return;
	if(!isNull())
		m_userType->destruct(m_pObject.data);
	m_userType->construct(m_pObject.data);
}

void GUserData::destruct()
{
	if (!isValid() || isNull())
		return;
	m_userType->destruct(m_pObject.data);
}

GUserData GUserData::clone() const
{
	if (!isValid() || isNull() || !m_userType->canClone())
		return GUserData();
	GUserData obj;
	obj.m_pObject = PopBlockFromPool(size());
	m_userType->clone(obj.m_pObject.data, m_pObject.data);
	return std::move(obj);
}

GUserData GUserData::move() const
{
	if (!isValid() || isNull() || !m_userType->canMove())
		return GUserData();

	GUserData obj;
	obj.m_pObject = PopBlockFromPool(size());
	m_userType->move(obj.m_pObject.data, m_pObject.data);
	return std::move(obj);
}

bool GUserData::compare(const GUserData& obj) const
{
	if (m_userType->getTypeId() != obj.m_userType->getTypeId())
		return false;
	if (!isValid())
		return true;
	return m_userType->compare(m_pObject.data, obj.m_pObject.data);
}

size_t GUserData::size() const
{
	return m_userType->getSize();
}


GUserTypePtr GUserData::getUserType() const
{
	return m_userType;
}

void GUserData::init(const void* pObject)
{
	if (isValid() && m_userType->canClone())
	{
		m_pObject = PopBlockFromPool(size());
		m_userType->clone(m_pObject.data, pObject);
	}
}

struct CharStream
{
	char ch;
	size_t num;
};

CharStream setChar(size_t num, char ch = ' ')
{
	CharStream stream;
	stream.ch = ch;
	stream.num = num;
	return stream;
}

std::ostream& operator<<(std::ostream& os, const CharStream& stream)
{
	for (size_t i = 0; i < stream.num; ++i)
		os << stream.ch;
	return os;
}

void Inspect(std::ostream& os, const GValue& val, size_t offset, size_t space)
{
	switch (val.getType())
	{
	case GType::TypeInvalid:
		os << ::setChar(offset) << "[none]";
		break;
	case GType::TypeBool:
		os << ::setChar(offset) << (val.getBool() ? "true" : "false");
		break;
	case GType::TypeChar:
		os << ::setChar(offset) << val.getChar();
		break;
	case GType::TypeUChar:
		os << ::setChar(offset) << '\'' << val.getUChar() << '\'';
		break;
	case GType::TypeShort:
		os << ::setChar(offset) << val.getShort();
		break;
	case GType::TypeUShort:
		os << ::setChar(offset) << val.getUShort();
		break;
	case GType::TypeInt:
		os << ::setChar(offset) << val.getInt();
		break;
	case GType::TypeUInt:
		os << ::setChar(offset) << val.getUInt();
		break;
	case GType::TypeFloat:
		os << ::setChar(offset) << val.getFloat();
		break;
	case GType::TypeDouble:
		os << ::setChar(offset) << val.getDouble();
		break;
	case GType::TypeInt64:
		os << ::setChar(offset) << val.getInt64();
		break;
	case GType::TypeUInt64:
		os << ::setChar(offset) << val.getUInt64();
		break;
	case GType::TypeString:
		os << ::setChar(offset) << '"' << val.getString().toUtf8().data() << '"';
		break;
	case GType::TypeObject:
		os << ::setChar(offset) << "[object]";//val.getObject();
		break;
	case GType::TypeVector:
		os << "[\n";
		for (const GValue& val : val.getVector())
		{
			Inspect(os, val, offset + space, space);
			os << ",\n";
		}
		os << ::setChar(offset) << "]";
		break;
	case GType::TypeMap:
		os << "{\n";
		for (const auto& pair : val.getMap())
		{
			os << ::setChar(offset + space) << pair.first.toUtf8().data() << " : ";
			switch (pair.second.getType())
			{
			case GType::TypeVector:
			case GType::TypeSet:
			case GType::TypeMap:
				Inspect(os, pair.second, offset + space, space);
				break;
			default:
				Inspect(os, pair.second, 0, space);
				break;
			}
			os << ",\n";
		}
		os << ::setChar(offset) << "}";
		break;
	case GType::TypeSet:
		os << "[\n";
		for (const GValue& val : val.getVector())
		{
			Inspect(os, val, offset + space, space);
			os << ",\n";
		}
		os << ::setChar(offset) << "]";
		break;
	default:
		break;
	}
}

bool GValue::IsValueType(GType type)
{
	switch (type)
	{
	case GType::TypeInvalid:
	case GType::TypeBool:
	case GType::TypeChar:
	case GType::TypeUChar:
	case GType::TypeShort:
	case GType::TypeUShort:
	case GType::TypeInt:
	case GType::TypeUInt:
	case GType::TypeFloat:
	case GType::TypeDouble:
	case GType::TypeInt64:
	case GType::TypeUInt64:
	case GType::TypeUserData:
		return true;
	default:
		break;
	}
	return false;
}

bool GValue::IsReferenceType(GType type)
{
	return !IsValueType(type);
}

GValue GValue::fromBool(bool bValue)
{
	return GValue(bValue);
}

GValue GValue::fromChar(char val)
{
	return GValue(val);
}

GValue GValue::fromUChar(unsigned char val)
{
	return GValue(val);
}

GValue GValue::fromShort(short val)
{
	return GValue(val);
}

GValue GValue::fromUShort(unsigned short val)
{
	return GValue(val);
}

GValue GValue::fromInt(int val)
{
	return GValue(val);
}

GValue GValue::fromUInt(unsigned int val)
{
	return GValue(val);
}

GValue GValue::fromFloat(float val)
{
	return GValue(val);
}

GValue GValue::fromDouble(double val)
{
	return GValue(val);
}

GValue GValue::fromInt64(__int64 val)
{
	return GValue(val);
}

GValue GValue::fromUInt64(unsigned __int64 val)
{
	return GValue(val);
}

GValue GValue::fromString(const GString& str)
{
	return GValue(str);
}

GValue GValue::fromString(GString&& str)
{
	return GValue(std::move(str));
}

GValue GValue::fromUserData(GUserData&& userdata)
{
	return GValue(std::move(userdata));
}


GValue GValue::fromUserData(const GUserData& userdata)
{
	return GValue(userdata);
}


GValue GValue::fromUserData(const void* userdata, const GUID& typeId)
{
	return GValue(userdata, typeId);
}


GValue GValue::fromUserData(const void* userdata, const char* typeName)
{
	return GValue(userdata, typeName);
}

GValue GValue::fromUserData(const void* userdata, GUserTypePtr userType)
{
	return GValue(userdata, userType);
}

GValue GValue::fromVector(const GValueVector& vec)
{
	return GValue(vec);
}

GValue GValue::fromVector(GValueVector&& vec)
{
	return GValue(std::move(vec));
}

GValue GValue::fromMap(const GValueMap& map)
{
	return GValue(map);
}

GValue GValue::fromMap(GValueMap&& map)
{
	return GValue(std::move(map));
}

GValue GValue::fromSet(const GValueSet& set)
{
	return GValue(set);
}

GValue GValue::fromSet(GValueSet&& set)
{
	return GValue(std::move(set));
}

GValue::GValue()
	:m_ui64Value(0), m_type(GType::TypeInvalid)
{

}

GValue::GValue(bool val)
	: m_bValue(val), m_type(GType::TypeBool)
{

}

GValue::GValue(char val)
	: m_cValue(val), m_type(GType::TypeChar)
{

}

GValue::GValue(unsigned char val)
	: m_ucValue(val), m_type(GType::TypeUChar)
{

}

GValue::GValue(short val)
	: m_sValue(val), m_type(GType::TypeShort)
{

}

GValue::GValue(unsigned short val)
	: m_usValue(val), m_type(GType::TypeUShort)
{

}

GValue::GValue(int val)
	: m_iValue(val), m_type(GType::TypeInt)
{

}

GValue::GValue(unsigned int val)
	: m_uiValue(val), m_type(GType::TypeUInt)
{

}

GValue::GValue(float val)
	: m_fValue(val), m_type(GType::TypeFloat)
{

}

GValue::GValue(double val)
	: m_dValue(val), m_type(GType::TypeDouble)
{

}

GValue::GValue(__int64 val)
	: m_i64Value(val), m_type(GType::TypeInt64)
{

}

GValue::GValue(unsigned __int64 val)
	: m_ui64Value(val), m_type(GType::TypeUInt64)
{

}

GValue::GValue(const GString& str)
	: m_strValue(PopFromGlobalPool<GString>(str)), m_type(GType::TypeString)
{

}

GValue::GValue(GString&& str)
	: m_strValue(PopFromGlobalPool<GString>(std::move(str))), m_type(GType::TypeString)
{

}


GValue::GValue(const GUserData& userdata)
	: m_type(GType::TypeInvalid)
{
	if (userdata.isValid())
	{
		m_type = GType::TypeUserData;
		m_pUserData = PopFromGlobalPool<GUserData>(userdata);
	}
}

GValue::GValue(GUserData&& userdata)
	: m_type(GType::TypeInvalid)
{
	if (userdata.isValid())
	{
		m_type = GType::TypeUserData;
		m_pUserData = PopFromGlobalPool<GUserData>(std::move(userdata));
	}
}

GValue::GValue(const void* pData, const GUID& typeId)
	: m_type(GType::TypeInvalid)
{
	GUserData userdata(pData, typeId);
	if (userdata.isValid())
	{
		m_type = GType::TypeUserData;
		m_pUserData = PopFromGlobalPool<GUserData>(std::move(userdata));
	}
}


GValue::GValue(const void* pData, const char* typeName)
	: m_type(GType::TypeInvalid)
{
	GUserData userdata(pData, typeName);
	if (userdata.isValid())
	{
		m_type = GType::TypeUserData;
		m_pUserData = PopFromGlobalPool<GUserData>(std::move(userdata));
	}
}

GValue::GValue(const void* pData, GUserTypePtr userType)
	: m_type(GType::TypeInvalid)
{
	GUserData userdata(pData, userType);
	if (userdata.isValid())
	{
		m_type = GType::TypeUserData;
		m_pUserData = PopFromGlobalPool<GUserData>(std::move(userdata));
	}
}

GValue::GValue(const GValueVector& vecVal)
	: m_vecValue(new GValueVector(vecVal)), m_type(GType::TypeVector)
{

}

GValue::GValue(GValueVector&& vecVal)
	: m_vecValue(new GValueVector(std::move(vecVal))), m_type(GType::TypeVector)
{

}

GValue::GValue(const GValueMap& mapVal)
	: m_mapValue(new GValueMap(mapVal)), m_type(GType::TypeMap)
{

}

GValue::GValue(GValueMap&& mapVal)
	: m_mapValue(new GValueMap(std::move(mapVal))), m_type(GType::TypeMap)
{

}

GValue::GValue(const GValueSet& setVal)
	: m_setValue(new GValueSet(setVal)), m_type(GType::TypeSet)
{

}

GValue::GValue(GValueSet&& setVal)
	: m_setValue(new GValueSet(std::move(setVal))), m_type(GType::TypeSet)
{

}

GValue::GValue(const GValue& val)
	: m_ui64Value(0), m_type(GType::TypeInvalid)
{
	*this = val;
}

GValue::GValue(GValue&& val)
	: m_ui64Value(0), m_type(GType::TypeInvalid)
{
	*this = std::move(val);
}

GValue::GValue(GReflectionObjectPtr pObject)
	: m_pObject(pObject.get()), m_type(GType::TypeObject)
{
	if(m_pObject)
		m_pObject->AddRef();
}

void GValue::reset()
{
	switch (m_type)
	{
	case GType::TypeString:
		PushToGlobalPool(m_strValue);
		break;
	case GType::TypeObject:
		if(m_pObject)
			m_pObject->Release();
		break;
	case GType::TypeVector:
		delete m_vecValue;
		break;
	case GType::TypeMap:
		delete m_mapValue;
		break;
	case GType::TypeSet:
		delete m_setValue;
		break;
	case GType::TypeUserData:
	{
		m_pUserData->destruct();
		PushToGlobalPool(m_pUserData);
	}
	break;
	default:
		break;
	}
	m_type = GType::TypeInvalid;
	m_ui64Value = 0;
}

void GValue::setBool(bool val)
{
	reset();
	m_bValue = val;
	m_type = GType::TypeBool;
}

void GValue::setChar(char val)
{
	reset();
	m_cValue = val;
	m_type = GType::TypeChar;
}

void GValue::setUChar(unsigned char val)
{
	reset();
	m_ucValue = val;
	m_type = GType::TypeUChar;
}

void GValue::setShort(short val)
{
	reset();
	m_sValue = val;
	m_type = GType::TypeShort;
}

void GValue::setUShort(unsigned short val)
{
	reset();
	m_usValue = val;
	m_type = GType::TypeUShort;
}

void GValue::setInt(int val)
{
	reset();
	m_iValue = val;
	m_type = GType::TypeInt;
}

void GValue::setUInt(unsigned int val)
{
	reset();
	m_uiValue = val;
	m_type = GType::TypeUInt;
}

void GValue::setFloat(float val)
{
	reset();
	m_fValue = val;
	m_type = GType::TypeFloat;
}

void GValue::setDouble(double val)
{
	reset();
	m_dValue = val;
	m_type = GType::TypeDouble;
}

void GValue::setInt64(__int64 val)
{
	reset();
	m_i64Value = val;
	m_type = GType::TypeInt64;
}

void GValue::setUInt64(unsigned __int64 val)
{
	reset();
	m_ui64Value = val;
	m_type = GType::TypeUInt64;
}

void GValue::setString(const GString& val)
{
	assign(val);
}

void GValue::setString(GString&& val)
{
	assign(std::move(val));
}

void GValue::setObject(GReflectionObjectPtr pObject)
{
	reset();
	m_pObject = pObject.get();
	if(m_pObject)
		m_pObject->AddRef();
	m_type = GType::TypeObject;
}

void GValue::setUserData(const GUserData& object)
{
	if (!object.isValid())
		return;
	reset();
	m_pUserData = PopFromGlobalPool<GUserData>(object);
	m_type = GType::TypeUserData;
}

void GValue::setUserData(GUserData&& object)
{
	if (!object.isValid())
		return;
	reset();
	m_pUserData = PopFromGlobalPool<GUserData>(std::move(object));
	m_type = GType::TypeUserData;
}

void GValue::setUserData(const void* pData, const GUID& typeId)
{
	setUserData(GUserData(pData, typeId));
}
void GValue::setUserData(const void* pData, const char* typeName)
{
	setUserData(GUserData(pData, typeName));
}

void GValue::setVector(const GValueVector& vec)
{
	assign(vec);
}

void GValue::setVector(GValueVector&& vec)
{
	assign(std::move(vec));
}

void GValue::setMap(const GValueMap& map)
{
	assign(map);
}

void GValue::setMap(GValueMap&& map)
{
	assign(std::move(map));
}

void GValue::setSet(const GValueSet& set)
{
	assign(set);
}

void GValue::setSet(GValueSet&& set)
{
	assign(std::move(set));
}

bool GValue::getBool() const
{
	assert(isValueType());
	if (isBool())
		return m_bValue;
	return m_ui64Value != 0;
}

char GValue::getChar() const
{
	assert(isValueType());
	switch (m_type)
	{
	case TypeBool:
		return (char)getBool();
	case TypeChar:
		return m_cValue;
	case TypeUChar:
		return (char)getUChar();
	case TypeShort:
		return (char)getShort();
	case TypeUShort:
		return (char)getUShort();
	case TypeInt:
		return (char)getInt();
	case TypeUInt:
		return (char)getUInt();
	case TypeFloat:
		return (char)getFloat();
	case TypeDouble:
		return (char)getDouble();
	case TypeInt64:
		return (char)getInt64();
	case TypeUInt64:
		return (char)getUInt64();
	default:
		break;
	}
	return 0;
}

unsigned char GValue::getUChar() const
{
	assert(isValueType());
	switch (m_type)
	{
	case TypeBool:
		return (unsigned char)getBool();
	case TypeChar:
		return (unsigned char)getChar();
	case TypeUChar:
		return m_ucValue;
	case TypeShort:
		return (unsigned char)getShort();
	case TypeUShort:
		return (unsigned char)getUShort();
	case TypeInt:
		return (unsigned char)getInt();
	case TypeUInt:
		return (unsigned char)getUInt();
	case TypeFloat:
		return (unsigned char)getFloat();
	case TypeDouble:
		return (unsigned char)getDouble();
	case TypeInt64:
		return (unsigned char)getInt64();
	case TypeUInt64:
		return (unsigned char)getUInt64();
	default:
		break;
	}
	return 0;
}

short GValue::getShort() const
{
	assert(isValueType());
	switch (m_type)
	{
	case TypeBool:
		return (short)getBool();
	case TypeChar:
		return (short)getChar();
	case TypeUChar:
		return (short)getUChar();
	case TypeShort:
		return m_sValue;
	case TypeUShort:
		return (short)getUShort();
	case TypeInt:
		return (short)getInt();
	case TypeUInt:
		return (short)getUInt();
	case TypeFloat:
		return (short)getFloat();
	case TypeDouble:
		return (short)getDouble();
	case TypeInt64:
		return (short)getInt64();
	case TypeUInt64:
		return (short)getUInt64();
	default:
		break;
	}
	return 0;
}

unsigned short GValue::getUShort() const
{
	assert(isValueType());
	switch (m_type)
	{
	case TypeBool:
		return (unsigned short)getBool();
	case TypeChar:
		return (unsigned short)getChar();
	case TypeUChar:
		return (unsigned short)getUChar();
	case TypeShort:
		return (unsigned short)getShort();
	case TypeUShort:
		return m_usValue;
	case TypeInt:
		return (unsigned short)getInt();
	case TypeUInt:
		return (unsigned short)getUInt();
	case TypeFloat:
		return (unsigned short)getFloat();
	case TypeDouble:
		return (unsigned short)getDouble();
	case TypeInt64:
		return (unsigned short)getInt64();
	case TypeUInt64:
		return (unsigned short)getUInt64();
	default:
		break;
	}
	return 0;
}

int GValue::getInt() const
{
	assert(isValueType());
	switch (m_type)
	{
	case TypeBool:
		return (int)getBool();
	case TypeChar:
		return (int)getChar();
	case TypeUChar:
		return (int)getUChar();
	case TypeShort:
		return (int)getShort();
	case TypeUShort:
		return (int)getUShort();
	case TypeInt:
		return m_iValue;
	case TypeUInt:
		return (int)getUInt();
	case TypeFloat:
		return (int)getFloat();
	case TypeDouble:
		return (int)getDouble();
	case TypeInt64:
		return (int)getInt64();
	case TypeUInt64:
		return (int)getUInt64();
	default:
		break;
	}
	return 0;
}

unsigned int GValue::getUInt() const
{
	assert(isValueType());
	switch (m_type)
	{
	case TypeBool:
		return (unsigned int)getBool();
	case TypeChar:
		return (unsigned int)getChar();
	case TypeUChar:
		return (unsigned int)getUChar();
	case TypeShort:
		return (unsigned int)getShort();
	case TypeUShort:
		return (unsigned int)getUShort();
	case TypeInt:
		return (unsigned int)getInt();
	case TypeUInt:
		return m_uiValue;
	case TypeFloat:
		return (unsigned int)getFloat();
	case TypeDouble:
		return (unsigned int)getDouble();
	case TypeInt64:
		return (unsigned int)getInt64();
	case TypeUInt64:
		return (unsigned int)getUInt64();
	default:
		break;
	}
	return 0;
}

float GValue::getFloat() const
{
	assert(isValueType());
	switch (m_type)
	{
	case TypeBool:
		return (float)getBool();
	case TypeChar:
		return (float)getChar();
	case TypeUChar:
		return (float)getUChar();
	case TypeShort:
		return (float)getShort();
	case TypeUShort:
		return (float)getUShort();
	case TypeInt:
		return (float)getInt();
	case TypeUInt:
		return (float)getUInt();
	case TypeFloat:
		return m_fValue;
	case TypeDouble:
		return (float)getDouble();
	case TypeInt64:
		return (float)getInt64();
	case TypeUInt64:
		return (float)getUInt64();
	default:
		break;
	}
	return 0;
}

double GValue::getDouble() const
{
	assert(isValueType());
	switch (m_type)
	{
	case TypeBool:
		return (double)getBool();
	case TypeChar:
		return (double)getChar();
	case TypeUChar:
		return (double)getUChar();
	case TypeShort:
		return (double)getShort();
	case TypeUShort:
		return (double)getUShort();
	case TypeInt:
		return (double)getInt();
	case TypeUInt:
		return (double)getUInt();
	case TypeFloat:
		return m_fValue;
	case TypeDouble:
		return m_dValue;
	case TypeInt64:
		return (double)getInt64();
	case TypeUInt64:
		return (double)getUInt64();
	default:
		break;
	}
	return 0;
}

__int64 GValue::getInt64() const
{
	assert(isValueType());
	switch (m_type)
	{
	case TypeBool:
		return (__int64)getBool();
	case TypeChar:
		return (__int64)getChar();
	case TypeUChar:
		return (__int64)getUChar();
	case TypeShort:
		return (__int64)getShort();
	case TypeUShort:
		return (__int64)getUShort();
	case TypeInt:
		return (__int64)getInt();
	case TypeUInt:
		return (__int64)getUInt();
	case TypeFloat:
		return (__int64)getFloat();
	case TypeDouble:
		return (__int64)getDouble();
	case TypeInt64:
		return m_i64Value;
	case TypeUInt64:
		return (__int64)getUInt64();
	default:
		break;
	}
	return 0;
}

unsigned __int64 GValue::getUInt64() const
{
	assert(isValueType());
	switch (m_type)
	{
	case TypeBool:
		return (__int64)getBool();
	case TypeChar:
		return (__int64)getChar();
	case TypeUChar:
		return (__int64)getUChar();
	case TypeShort:
		return (__int64)getShort();
	case TypeUShort:
		return (__int64)getUShort();
	case TypeInt:
		return (__int64)getInt();
	case TypeUInt:
		return (__int64)getUInt();
	case TypeFloat:
		return (__int64)getFloat();
	case TypeDouble:
		return (__int64)getDouble();
	case TypeInt64:
		return (__int64)getInt64();
	case TypeUInt64:
		return m_ui64Value;
	default:
		break;
	}
	return 0;
}

const GString& GValue::getString() const
{
	assert(isString());
	return *m_strValue;
}

GString& GValue::getString()
{
	assert(isString());
	return *m_strValue;
}

void GValue::getUserData(void* pData, size_t numOfBytes) const
{
	assert(isUserData());
	if (numOfBytes != m_pUserData->size())
		return;
	m_pUserData->getUserType()->clone(pData, m_pUserData->get());
}


GReflectionObjectPtr GValue::getObject() const
{
	assert(isObject());
	return m_pObject;
}

const GValueVector& GValue::getVector() const
{
	assert(isVector());
	return *m_vecValue;
}

GValueVector& GValue::getVector()
{
	assert(isVector());
	return *m_vecValue;
}

const GValueMap& GValue::getMap() const
{
	assert(isMap());
	return *m_mapValue;
}

GValueMap& GValue::getMap()
{
	assert(isMap());
	return *m_mapValue;
}

const GValueSet& GValue::getSet() const
{
	assert(isSet());
	return *m_setValue;
}

GValueSet& GValue::getSet()
{
	assert(isSet());
	return *m_setValue;
}

GValue::GType GValue::getType() const
{
	return m_type;
}

GUserTypePtr GValue::getUserType() const
{
	if (m_type != GType::TypeUserData)
		return nullptr;
	return m_pUserData->getUserType();
}

bool GValue::isValueType() const
{
	return IsValueType(m_type);
}

bool GValue::isReferenceType() const
{
	return !isValueType();
}

std::string GValue::getTypeName() const
{
	switch (m_type)
	{
	case GType::TypeInvalid:
		return "invalid";
	case GType::TypeBool:
		return "bool";
	case GType::TypeChar:
		return "char";
	case GType::TypeUChar:
		return "unsigned char";
	case GType::TypeShort:
		return "short";
	case GType::TypeUShort:
		return "unsigned short";
	case GType::TypeInt:
		return "int";
	case GType::TypeUInt:
		return "unsigned int";
	case GType::TypeFloat:
		return "float";
	case GType::TypeDouble:
		return "double";
	case GType::TypeInt64:
		return "int64";
	case GType::TypeUInt64:
		return "unsigned int64";
	case GType::TypeString:
		return "string";
	case GType::TypeObject:
		return "object";
	case GType::TypeVector:
		return "vector";
	case GType::TypeMap:
		return "map";
	case GType::TypeSet:
		return "set";
	case GType::TypeUserData:
		return getUserType()->getName();
	default:
		break;
	}
	return "invalid";
}

size_t GValue::getValueSize() const
{
	switch (m_type)
	{
	case GType::TypeInvalid:
		return 0;
	case GType::TypeBool:
	case GType::TypeChar:
	case GType::TypeUChar:
		return 1;
	case GType::TypeShort:
	case GType::TypeUShort:
		return 2;
	case GType::TypeInt:
	case GType::TypeUInt:
	case GType::TypeFloat:
		return 4;
	case GType::TypeDouble:
		return 8;
	case GType::TypeInt64:
	case GType::TypeUInt64:
		return sizeof(__int64);
	case GType::TypeString:
		return m_strValue->size();
	case GType::TypeObject:
		return sizeof(GScopePointer<GObject>);
	case GType::TypeVector:
		return sizeof(*m_vecValue);
	case GType::TypeMap:
		return sizeof(*m_mapValue);
	case GType::TypeSet:
		return sizeof(*m_setValue);
	case GType::TypeUserData:
		return m_pUserData->size();
		break;
	default:
		break;
	}
	return 0;
}

bool GValue::isValid() const
{
	return getType() == GType::TypeInvalid;
}

bool GValue::isBool() const
{
	return m_type == GType::TypeBool;
}

bool GValue::isChar() const
{
	return m_type == GType::TypeChar;
}

bool GValue::isUChar() const
{
	return m_type == GType::TypeUChar;
}

bool GValue::isShort() const
{
	return m_type == GType::TypeShort;
}

bool GValue::isUShort() const
{
	return m_type == GType::TypeUShort;
}

bool GValue::isInt() const
{
	return m_type == GType::TypeInt;
}

bool GValue::isUInt() const
{
	return m_type == GType::TypeUInt;
}

bool GValue::isFloat() const
{
	return m_type == GType::TypeFloat;
}

bool GValue::isDouble() const
{
	return m_type == GType::TypeDouble;
}

bool GValue::isInt64() const
{
	return m_type == GType::TypeInt64;
}

bool GValue::isUInt64() const
{
	return m_type == GType::TypeUInt64;
}

bool GValue::isString() const
{
	return m_type == GType::TypeString;
}

bool GValue::isObject() const
{
	return m_type == GType::TypeObject;
}

bool GValue::isUserData() const
{
	return m_type == GType::TypeUserData;
}

bool GValue::isVector() const
{
	return m_type == GType::TypeVector;
}

bool GValue::isMap() const
{
	return m_type == GType::TypeMap;
}

bool GValue::isSet() const
{
	return m_type == GType::TypeSet;
}

void GValue::inspect(size_t space /*= 0*/, std::ostream& os /*= std::cout*/) const
{
	::Inspect(os, *this, 0, space);
}

size_t GValue::hashCode() const
{
	assert(getType() != GType::TypeVector && getType() != GType::TypeMap && getType() != GType::TypeSet);
	switch (m_type)
	{
	case GType::TypeInvalid:
		return 0;
	case GType::TypeBool:
		return std::hash<bool>()(m_bValue);
	case GType::TypeChar:
	case GType::TypeUChar:
		return std::hash<unsigned char>()(m_ucValue);
	case GType::TypeShort:
	case GType::TypeUShort:
		return std::hash<unsigned short>()(m_usValue);
	case GType::TypeInt:
	case GType::TypeUInt:
		return std::hash<unsigned int>()(m_uiValue);
	case GType::TypeFloat:
		return std::hash<float>()(getFloat());
	case GType::TypeDouble:
		return std::hash<double>()(getDouble());
	case GType::TypeInt64:
	case GType::TypeUInt64:
		return std::hash<unsigned __int64>()(m_ui64Value);
	case GType::TypeString:
		return std::hash<GString>()(getString());
	case GType::TypeObject:
		return std::hash<void*>()(m_pObject);
	case GType::TypeUserData:
	{
		if (m_pUserData->isNull() || !m_pUserData->isValid())
			return 0;
		return std::_Hash_bytes((const unsigned char*)m_pUserData->get(), m_pUserData->size());
	}
	default:
		break;
	}
	return 0;
}

bool GValue::operator!=(const GValue& val) const
{
	return !(operator==(val));
}

bool GValue::operator==(const GValue& val) const
{
	if (getType() != val.getType())
	{
		if (isReferenceType() || val.isReferenceType())
			return false;
		return getInt64() == val.getInt64();
	}
	switch (m_type)
	{
	case GType::TypeInvalid:
		return val.isValid();
	case GType::TypeBool:
		return getBool() == val.getBool();
	case GType::TypeChar:
		return getChar() == val.getChar();
	case GType::TypeUChar:
		return getUChar() == val.getUChar();
	case GType::TypeShort:
		return getShort() == val.getShort();
	case GType::TypeUShort:
		return getUShort() == val.getUShort();
	case GType::TypeInt:
		return getInt() == val.getInt();
	case GType::TypeUInt:
		return getUInt() == val.getUInt();
	case GType::TypeFloat:
		return getFloat() == val.getFloat();
	case GType::TypeDouble:
		return getDouble() == val.getDouble();
	case GType::TypeInt64:
		return getInt64() == val.getInt64();
	case GType::TypeUInt64:
		return getUInt64() == val.getUInt64();
	case GType::TypeString:
		return getString() == val.getString();
	case GType::TypeObject:
		return getObject() == val.getObject();
	case GType::TypeVector:
	case GType::TypeMap:
	case GType::TypeSet:
		return false;
	case GType::TypeUserData:
		return m_pUserData->compare(*val.m_pUserData);
	default:
		break;
	}

	return false;
}

const GValue& GValue::operator=(GValue&& val)
{
	reset();
	m_ui64Value = val.m_ui64Value;
	m_type = val.getType();

	val.m_ui64Value = 0;
	val.m_type = GType::TypeInvalid;

	return *this;
}

const GValue& GValue::operator=(const GValue& val)
{
	switch (val.getType())
	{
	case GType::TypeObject:
		reset();
		m_pObject = val.getObject().get();
		if(m_pObject)
			m_pObject->AddRef();
		break;
	case GType::TypeString:
		assign(val.getString());
		break;
	case GType::TypeVector:
		assign(val.getVector());
		break;
	case GType::TypeMap:
		assign(val.getMap());
		break;
	case GType::TypeSet:
		assign(val.getSet());
		break;
	case GType::TypeUserData:
		setUserData(*val.m_pUserData);
	break;
	default:
		reset();
		m_ui64Value = val.getUInt64();
		break;
	}
	m_type = val.getType();

	return *this;
}

GValue::~GValue()
{
	reset();
}

void GValue::assign(const GStringPiece& str)
{
	if (m_type == GType::TypeString)
		*m_strValue = str.toString();
	else
	{
		reset();
		m_strValue = PopFromGlobalPool<GString>(str.toString());
		m_type = GType::TypeString;
	}
}

void GValue::assign(const GString& str)
{
	AssignGValueField(this, &m_strValue, &m_type, str, GType::TypeString);
}

void GValue::assign(GString&& str)
{
	AssignGValueField(this, &m_strValue, &m_type, std::move(str), GType::TypeString);
}

void GValue::assign(const GValueVector& val)
{
	AssignGValueField(this, &m_vecValue, &m_type, val, GType::TypeVector);
}

void GValue::assign(GValueVector&& val)
{
	AssignGValueField(this, &m_vecValue, &m_type, std::move(val), GType::TypeVector);
}

void GValue::assign(const GValueMap& val)
{
	AssignGValueField(this, &m_mapValue, &m_type, val, GType::TypeMap);
}

void GValue::assign(GValueMap&& val)
{
	AssignGValueField(this, &m_mapValue, &m_type, std::move(val), GType::TypeMap);
}

void GValue::assign(const GValueSet& val)
{
	AssignGValueField(this, &m_setValue, &m_type, val, GType::TypeSet);
}

void GValue::assign(GValueSet&& val)
{
	AssignGValueField(this, &m_setValue, &m_type, std::move(val), GType::TypeSet);
}