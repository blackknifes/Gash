#ifndef __GCONDITIONCALLBACK_H__
#define __GCONDITIONCALLBACK_H__
#include <functional>
#include "GCondition.h"

class GConditionCallback: public GCondition
{
public:
	typedef std::function<bool()> Callback;
	typedef std::function<void()> Updater;

	virtual bool check() override;
	virtual void update() override;
private:
	GObjectClassPooled(GConditionCallback);
	GConditionCallback(const Callback& callback, const Updater& updater = nullptr);
	Callback m_callback;
	Updater m_updater;
};
#endif