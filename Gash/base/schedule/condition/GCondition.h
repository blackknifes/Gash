#ifndef __GSCHEDULECONDITION_H__
#define __GSCHEDULECONDITION_H__
#include "../../GObject.h"

GREFPTR_DEF(GCondition);

class GCondition: public virtual GObject
{
public:
	enum Type
	{
		Cancelable = 0,
		Overcount,
		Timeout,
		Once,
		Inteval,
		Event,
		And,
		Not,
		Or,
		Callback
	};

	GCondition(Type type): m_type(type) {};
	virtual bool check() = 0;
	virtual void update() {};
private:
	Type m_type;
};
#endif