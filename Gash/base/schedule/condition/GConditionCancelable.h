#ifndef __GCONDITIONCANCELABLE_H__
#define __GCONDITIONCANCELABLE_H__
#include "GCondition.h"

GREFPTR_DEF(GConditionCancelable);

class GConditionCancelable: public GCondition
{
public:
	void cancel();
	virtual bool check() override;

private:
	GObjectClassPooled(GConditionCancelable);
	GConditionCancelable();
	bool m_checkflag;
};
#endif