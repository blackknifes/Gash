#ifndef __GWIN32SERVICE_H__
#define __GWIN32SERVICE_H__
#include "../GService.h"

class GWin32Service: public GService
{
public:
	GWin32Service(SC_HANDLE hService);
	~GWin32Service();

	virtual bool start(const std::vector<GString>& args) override;
	virtual bool stop() override;
	virtual State getState() const override;
	virtual GString getName() const override;
	virtual GString getDisplayName() const override;
	virtual void* getNativeHandle() const override;

private:
	GObjectClass(GWin32Service);
	SC_HANDLE m_hService;
	bool m_tempFlag;
};
#endif