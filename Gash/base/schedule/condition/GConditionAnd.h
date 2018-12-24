#ifndef __GCONDITIONAND_H__
#define __GCONDITIONAND_H__
#include "GCondition.h"

class GConditionAnd: public GCondition
{
public:
	virtual bool check() override;
	virtual void update() override;
private:
	GObjectClassPooled(GConditionAnd);
	GConditionAnd(GConditionPtr pCondition, ...);
	std::vector<GConditionPtr> m_conditionList;
};
#endif