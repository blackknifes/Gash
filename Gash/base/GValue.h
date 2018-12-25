#ifndef __CORE_GVALUE_H__
#define __CORE_GVALUE_H__

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include "GString.h"
#include <iostream>
#include "GTypes.h"
#include "reflection/GUserType.h"
#include "GAllocator.h"
#include "reflection/GReflectionObject.h"
#include "GStringPiece.h"

class GValue;

typedef std::vector<GValue> GValueVector;
typedef std::unordered_map<GString, GValue> GValueMap;
typedef std::unordered_set<GValue> GValueSet;

class GUserData
{
public:
	GUserData();
	GUserData(const void* pObject, const GUID& userType);
	GUserData(const void* pObject, const char* typeName);
	GUserData(const void* pObject, GUserTypePtr userType);
	~GUserData();

	GUserData(const GUserData& obj);
	GUserData(GUserData&& obj);
	const GUserData& operator=(const GUserData& obj);
	const GUserData& operator=(GUserData&& obj);
	bool operator==(const GUserData& obj) const;

	bool isNull() const;
	bool isValid() const;

	void* get() const;
	void construct();
	void destruct();
	GUserData clone() const;
	GUserData move() const;
	bool compare(const GUserData& obj) const;

	size_t size() const;

	GUserTypePtr getUserType() const;

private:
	void init(const void* pObject);
	void* m_pUserData;
	GUserTypePtr m_userType;
};

class GValue
{
public:
	using GType = GType;
	typedef std::vector<GValue> GValueVector;
	typedef std::unordered_map<GString, GValue> GValueMap;
	typedef std::unordered_set<GValue> GValueSet;

	static bool IsValueType(GType type);
	static bool IsReferenceType(GType type);

	static GValue fromBool(bool bValue);
	static GValue fromChar(char val);
	static GValue fromUChar(unsigned char val);
	static GValue fromShort(short val);
	static GValue fromUShort(unsigned short val);
	static GValue fromInt(int val);
	static GValue fromUInt(unsigned int val);
	static GValue fromFloat(float val);
	static GValue fromDouble(double val);
	static GValue fromInt64(__int64 val);
	static GValue fromUInt64(unsigned __int64 val);
	static GValue fromString(const GString& str);
	static GValue fromString(GString&& str);
	static GValue fromVector(const GValueVector& vec);
	static GValue fromVector(GValueVector&& vec);
	static GValue fromMap(const GValueMap& map);
	static GValue fromMap(GValueMap&& map);
	static GValue fromSet(const GValueSet& set);
	static GValue fromSet(GValueSet&& set);
	static GValue fromUserData(const GUserData& userdata);
	static GValue fromUserData(GUserData&& userdata);
	static GValue fromUserData(const void* userdata, const GUID& typeId);
	static GValue fromUserData(const void* userdata, const char* typeName);
	static GValue fromUserData(const void* userdata, GUserTypePtr userType);
	static GValue fromObject(GReflectionObjectPtr pObject);


	GValue();
	explicit GValue(bool val);
	explicit GValue(char val);
	explicit GValue(unsigned char val);
	explicit GValue(short val);
	explicit GValue(unsigned short val);
	explicit GValue(int val);
	explicit GValue(unsigned int val);
	explicit GValue(float val);
	explicit GValue(double val);
	explicit GValue(__int64 val);
	explicit GValue(unsigned __int64 val);
	explicit GValue(const GString& str);
	explicit GValue(GString&& str);
	explicit GValue(const GValueVector& vecVal);
	explicit GValue(GValueVector&& vecVal);
	explicit GValue(const GValueMap& mapVal);
	explicit GValue(GValueMap&& mapVal);
	explicit GValue(const GValueSet& setVal);
	explicit GValue(GValueSet&& setVal);

	explicit GValue(GReflectionObjectPtr pObject);
	explicit GValue(const GUserData& userdata);
	explicit GValue(GUserData&& userdata);
	explicit GValue(const void* pData, const GUID& typeId);
	explicit GValue(const void* pData, const char* typeName);
	explicit GValue(const void* pData, GUserTypePtr userType);

	~GValue();

	GValue(const GValue& val);
	GValue(GValue&& val);
	const GValue& operator=(const GValue& val);
	const GValue& operator=(GValue&& val);

	bool operator==(const GValue& val) const;
	bool operator!=(const GValue& val) const;

	void reset();

	void setBool(bool val);
	void setChar(char val);
	void setUChar(unsigned char val);
	void setShort(short val);
	void setUShort(unsigned short val);
	void setInt(int val);
	void setUInt(unsigned int val);
	void setFloat(float val);
	void setDouble(double val);
	void setInt64(__int64 val);
	void setUInt64(unsigned __int64 val);
	void setString(const GString& val);
	void setString(GString&& val);
	void setObject(GReflectionObjectPtr pObject);
	void setUserData(const GUserData& object);
	void setUserData(GUserData&& object);
	void setUserData(const void* pData, const GUID& typeId);
	void setUserData(const void* pData, const char* typeName);

	void setVector(const GValueVector& vec);
	void setVector(GValueVector&& vec);
	void setMap(const GValueMap& map);
	void setMap(GValueMap&& map);
	void setSet(const GValueSet& set);
	void setSet(GValueSet&& set);

	bool getBool() const;
	char getChar() const;
	unsigned char getUChar() const;
	short getShort() const;
	unsigned short getUShort() const;
	int getInt() const;
	unsigned int getUInt() const;
	float getFloat() const;
	double getDouble() const;
	__int64 getInt64() const;
	unsigned __int64 getUInt64() const;
	const GString& getString() const;
	GString& getString();
	void getUserData(void* pData, size_t numOfBytes) const;
	GReflectionObjectPtr getObject() const;

	const GValueVector& getVector() const;
	GValueVector& getVector();
	const GValueMap& getMap() const;
	GValueMap& getMap();
	const GValueSet& getSet() const;
	GValueSet& getSet();

	GType getType() const;
	GUserTypePtr getUserType() const;

	bool isValueType() const;
	bool isReferenceType() const;

	std::string getTypeName() const;
	size_t getValueSize() const;

	bool isValid() const;
	bool isBool() const;
	bool isChar() const;
	bool isUChar() const;
	bool isShort() const;
	bool isUShort() const;
	bool isInt() const;
	bool isUInt() const;
	bool isFloat() const;
	bool isDouble() const;
	bool isInt64() const;
	bool isUInt64() const;
	bool isString() const;
	bool isObject() const;
	bool isUserData() const;
	bool isVector() const;
	bool isMap() const;
	bool isSet() const;

	void inspect(size_t space = 2, std::ostream& os = std::cout) const;
	size_t hashCode() const;

	template<typename _Ty>
	void setUserValue(const _Ty& val)
	{
		setUserData(&val, GUserTypeGetGuid<_Ty>()());
	}

	template<typename _Ty>
	void getUserValue(_Ty* pData)
	{
		getUserData(pData, sizeof(_Ty));
	}


private:
	void assign(const GStringPiece& str);
	void assign(const GString& str);
	void assign(GString&& str);
	void assign(const GValueVector& val);
	void assign(GValueVector&& val);
	void assign(const GValueMap& val);
	void assign(GValueMap&& val);
	void assign(const GValueSet& val);
	void assign(GValueSet&& val);

	union
	{
		bool m_bValue;
		char m_cValue;
		unsigned char m_ucValue;
		short m_sValue;
		unsigned short m_usValue;
		int m_iValue;
		unsigned int m_uiValue;
		float m_fValue;
		double m_dValue;
		__int64 m_i64Value;
		unsigned __int64 m_ui64Value;
		GString* m_strValue;
		GReflectionObject* m_pObject;
		GUserData* m_pUserData;
		GValueVector* m_vecValue;
		GValueMap* m_mapValue;
		GValueSet* m_setValue;
	};
	GType m_type;
};

template<>
struct std::hash<GValue>
{
	size_t operator()(const GValue& val) const
	{
		return val.hashCode();
	}
};
#endif