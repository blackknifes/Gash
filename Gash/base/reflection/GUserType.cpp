#include "GUserType.h"
#pragma warning(disable: 4244)

struct GUIDHasher
{
	size_t operator()(const GUID& guid) const
	{
		return std::_Hash_bytes((const unsigned char*)&guid, sizeof(guid));
	}
};

GUID GuidFromString(const char* str)
{
	GUID guid = {};
	if (strlen(str) != 36)
		return std::move(guid);
	char buf[36] = {};
	char* ptr = nullptr;
	*std::copy(str, str + 8, buf) = 0;
	guid.Data1 = strtoul(buf, &ptr, 16);
	*std::copy(str + 9, str + 13, buf) = 0;
	guid.Data2 = strtoul(buf, &ptr, 16);
	*std::copy(str + 14, str + 18, buf) = 0;
	guid.Data3 = strtoul(buf, &ptr, 16);

	*std::copy(str + 19, str + 21, buf) = 0;
	guid.Data4[0] = strtoul(buf, &ptr, 16);
	*std::copy(str + 21, str + 23, buf) = 0;
	guid.Data4[1] = strtoul(buf, &ptr, 16);

	for (size_t i = 0; i < 6; ++i)
	{
		*std::copy(str + 24 + i * 2, str + 24 + (i + 1) * 2, buf) = 0;
		guid.Data4[i + 2] = strtoul(buf, &ptr, 16);
	}
	return std::move(guid);
}

typedef std::unordered_map<GUID, GUserTypePtr, GUIDHasher> UserTypeIdMap;
typedef std::unordered_map<std::string, GUserTypePtr> UserTypeNameMap;

static UserTypeIdMap s_userTypeIdMap;
static UserTypeNameMap s_userTypeNameMap;

bool GUserType::RegisterUserType(
	const char* name, 
	const GUID& typeId,
	size_t size,
	Constructor pConstructor /*= nullptr*/, 
	Destructor pDestructor /*= nullptr*/, 
	Cloner pCloner /*= nullptr*/,
	Mover pMover /*= nullptr*/,
	Comparetor pComparetor /*= nullptr*/)
{
	auto itor = s_userTypeIdMap.find(typeId);
	if (itor != s_userTypeIdMap.end())
		return false;

	GUserType* userType = new GUserType();
	userType->m_strName = name;
	userType->m_userTypeId = typeId;
	userType->m_sSizeOfUserData = size;
	userType->m_pConstructor = pConstructor;
	userType->m_pDestructor = pDestructor;
	userType->m_pMover = pMover;
	userType->m_pCloner = pCloner;
	userType->m_pComparetor = pComparetor;

	s_userTypeIdMap.insert(UserTypeIdMap::value_type(typeId, userType));
	s_userTypeNameMap.insert(UserTypeNameMap::value_type(name, userType));
	return true;
}

void GUserType::UnregisterUserType(const GUID& typeId)
{
	auto itor = s_userTypeIdMap.find(typeId);
	if (itor == s_userTypeIdMap.end())
		return;
	
	auto itor2 = s_userTypeNameMap.find(itor->second->m_strName);
	if (itor2 != s_userTypeNameMap.end())
		s_userTypeNameMap.erase(itor2);

	s_userTypeIdMap.erase(itor);
}

void GUserType::UnregisterUserType(const char* name)
{
	auto itor = s_userTypeNameMap.find(name);
	if (itor == s_userTypeNameMap.end())
		return;

	auto itor2 = s_userTypeIdMap.find(itor->second->m_userTypeId);
	if (itor2 != s_userTypeIdMap.end())
		s_userTypeIdMap.erase(itor2);

	s_userTypeNameMap.erase(itor);
}

GUserTypePtr GUserType::FindUserType(const GUID& guid)
{
	auto itor = s_userTypeIdMap.find(guid);
	if (itor == s_userTypeIdMap.end())
		return nullptr;
	return itor->second;
}

GUserTypePtr GUserType::FindUserType(const char* name)
{
	auto itor = s_userTypeNameMap.find(name);
	if (itor == s_userTypeNameMap.end())
		return nullptr;
	return itor->second;
}

const char* GUserType::getName() const
{
	return m_strName.data();
}

const GUID& GUserType::getTypeId() const
{
	return m_userTypeId;
}

size_t GUserType::getSize() const
{
	return m_sSizeOfUserData;
}

void GUserType::construct(void* pData) const
{
	if (canConstructor())
		m_pConstructor(pData);
}

void GUserType::destruct(void* pData) const
{
	if (canDestructor())
		m_pDestructor(pData);
}

void GUserType::clone(void* pDest, const void* pSrc) const
{
	if (canClone())
		m_pCloner(pDest, pSrc);
}

void GUserType::move(void* pDest, void* pSrc) const
{
	if (!canMove())
		return clone(pDest, pSrc);
	m_pMover(pDest, pSrc);
}

bool GUserType::compare(const void* pLeft, const void* pRight) const
{
	if (!canCompare())
		return false;
	return m_pComparetor(pLeft, pRight);
}

bool GUserType::canConstructor() const
{
	return m_pConstructor != nullptr;
}

bool GUserType::canDestructor() const
{
	return m_pDestructor != nullptr;
}

bool GUserType::canClone() const
{
	return m_pCloner != nullptr;
}

bool GUserType::canMove() const
{
	return m_pMover != nullptr;
}

bool GUserType::canCompare() const
{
	return m_pComparetor != nullptr;
}