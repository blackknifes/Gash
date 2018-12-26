#ifndef __GWIN32EVENT_H__
#define __GWIN32EVENT_H__
#include "../GEvent.h"

class GWin32Event: public GEvent
{
public:
	GWin32Event();
	~GWin32Event();

	virtual GString getName() const override;
	virtual void setName(const GString& name) override;
	virtual bool open() override;
	virtual void close() override;
	virtual bool wait(DWORD timeout = INFINITE) override;
	virtual bool isClosed() const override;
	virtual void* getNativeHandle() const override;
	virtual void signal() override;

private:
	GObjectClassPooled(GWin32Event);
	HANDLE m_hEvent;
	GString m_strName;
};
#endif