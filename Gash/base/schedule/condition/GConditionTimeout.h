#ifndef __GCONDITIONTIMEOUT_H__
#define __GCONDITIONTIMEOUT_H__
#include "GCondition.h"

GREFPTR_DEF(GConditionTimeout);

class GConditionTimeout: public GCondition
{
public:
	static GConditionTimeoutPtr CreateByMilliSeconds(DWORD millisecond);
	static GConditionTimeoutPtr CreateBySeconds(DWORD second, DWORD millisecond = 0);
	static GConditionTimeoutPtr CreateByMinutes(DWORD minutes, DWORD second = 0, DWORD millisecond = 0);
	static GConditionTimeoutPtr CreateByHours(DWORD hours, DWORD minutes = 0, DWORD second = 0, DWORD millisecond = 0);
	static GConditionTimeoutPtr CreateByDays(DWORD days, DWORD hours = 0, DWORD minutes = 0, DWORD second = 0, DWORD millisecond = 0);
	virtual bool check() override;

private:
	GObjectClassPooled(GConditionTimeout);
	GConditionTimeout();
	unsigned __int64 m_millisecond;
};
#endif