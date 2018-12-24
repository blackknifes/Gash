#ifndef __GCONDITIONEVENT_H__
#define __GCONDITIONEVENT_H__
#include "GCondition.h"

class GConditionEvent: public GCondition
{
public:
	virtual bool check() override;
	void active();
private:
	GObjectClassPooled(GConditionEvent);
	GConditionEvent();
	GConditionEvent(const GString& str, bool active = false);
	GConditionEvent(HANDLE hEvent);
	~GConditionEvent();
	HANDLE m_hEvent;
	bool m_needDestroy;
};
#endif