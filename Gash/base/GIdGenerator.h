#ifndef __GIDGENERATOR_H__
#define __GIDGENERATOR_H__
#include "GSingleton.h"
#include <atomic>

template<typename _Ty, typename _idTy = __int64>
class GIdGenerator: public GSingleton<GIdGenerator<_Ty, _idTy>>
{
public:
	typedef _idTy id_type;
	GIdGenerator() :m_id(0) {}

	static id_type GenerateId()
	{
		return GIdGenerator::getInstance()->genId();
	}

	id_type genId()
	{
		return ++m_id;
	}
private:
	std::atomic<_idTy> m_id;
};

template<typename _Ty, typename _idTy>
_idTy GenerateId()
{
	return GIdGenerator<_Ty, _idTy>::GenerateId();
}
#endif