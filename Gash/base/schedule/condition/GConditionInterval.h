#ifndef __GCONDITIONINTERVAL_H__
#define __GCONDITIONINTERVAL_H__
#include "GCondition.h"

class GConditionInterval: public GCondition
{
public:
	virtual bool check() override;

private:
	GObjectClassPooled(GConditionInterval);
	GConditionInterval(__int64 inteval);

	unsigned __int64 m_inteval;
	unsigned __int64 m_pretime;
};
#endif