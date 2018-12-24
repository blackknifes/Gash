#ifndef __GSERVICE_H__
#define __GSERVICE_H__
#include "../GObject.h"

GREFPTR_DEF(GService);

class GService: public virtual GObject
{
public:
	enum State
	{
		Unknown = 0,
		Stopped,
		Starting,
		Running,
		Stopping,
		Pausing,
		Paused,
		Resuming
	};

	virtual bool start(const std::vector<GString>& args) = 0;
	virtual bool stop() = 0;
	virtual bool restart() { return stop() && start({}); }
	virtual State getState() const = 0;
	virtual GString getName() const = 0;
	virtual GString getDisplayName() const = 0;
	virtual void* getNativeHandle() const = 0;
};
#endif