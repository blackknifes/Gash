#ifndef __GCONDITIONOVERCOUNT_H__
#define __GCONDITIONOVERCOUNT_H__
#include "GCondition.h"

class GConditionOvercount: public GCondition
{
public:
	virtual bool check() override;
	virtual void update() override;

private:
	GObjectClassPooled(GConditionOvercount);
	GConditionOvercount(size_t maxCount);
	size_t m_count;
	size_t m_maxCount;
};
#endif