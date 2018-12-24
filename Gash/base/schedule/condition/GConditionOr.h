#ifndef __GCONDITIONOR_H__
#define __GCONDITIONOR_H__
#include "GCondition.h"

class GConditionOr: public GCondition
{
public:
	virtual bool check() override;
	virtual void update() override;
private:
	GObjectClassPooled(GConditionOr);
	GConditionOr(GConditionPtr pCondition, ...);
	std::vector<GConditionPtr> m_conditionList;
};
#endif