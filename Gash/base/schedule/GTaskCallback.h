#ifndef __GTASKCALLBACK_H__
#define __GTASKCALLBACK_H__
#include "GTask.h"
#include <functional>

class GTaskCallback: public GTask
{
public:
	typedef std::function<void()> Callback;
	static const int ID = 1;
	GTaskCallback(Callback callback);
	virtual void execute() override;

private:
	GObjectClassPooled(GTaskCallback);
	Callback m_callback;
};
#endif