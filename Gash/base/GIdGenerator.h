#ifndef __GIDGENERATOR_H__
#define __GIDGENERATOR_H__
#include "GSingleton.h"
#include <atomic>

template<typename _Ty, typename _idTy = __int64>
class GIdGenerator: public GSingleton<GIdGenerator>
{
public:
	typedef _idTy id_type;
	GIdGenerator() :m_id(0) {}

	static id_type GenerateId()
	{
		return getInstance()->genId();
	}

	id_type genId()
	{
		return ++m_id;
	}
private:
	std::atomic<id_type> m_id;
};
#endif