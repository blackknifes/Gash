#ifndef __GTASK_H__
#define __GTASK_H__
#include "../GObject.h"
#include "../GIdGenerator.h"

GREFPTR_DEF(GTask);

class GTask: public virtual GObject
{
public:
	GTask(int type) 
		: m_id(GenerateId<GTask, int>()), m_iType(type)
	{};
	virtual void execute() = 0;

private:
	int m_id;
	int m_iType;
};
#endif