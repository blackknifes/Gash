#ifndef __GCONDITIONNOT_H__
#define __GCONDITIONNOT_H__
#include "GCondition.h"

class GConditionNot: public GCondition
{
public:
	virtual bool check() override;
	virtual void update() override;

private:
	GObjectClassPooled(GConditionNot);
	GConditionNot(GConditionPtr pCondition);
	GConditionPtr m_pCondition;
};
#endif