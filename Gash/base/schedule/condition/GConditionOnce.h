#ifndef __GCONDITIONONCE_H__
#define __GCONDITIONONCE_H__
#include "GConditionCancelable.h"

class GConditionOnce: public GCondition
{
public:
	virtual bool check() override;
	virtual void update() override;

private:
	GObjectClassPooled(GConditionOnce);
	GConditionOnce();
	bool m_once;
};
#endif