#ifndef __GSTREAMDEVICE_H__
#define __GSTREAMDEVICE_H__

#include "..\io\GIODevice.h"

class GStreamDevice : public virtual GObject
{
public:
	~GStreamDevice();
	void close();
	bool isClosed() const;
	GIODevicePtr getDevice() const;
protected:
	void setDevice(GIODevicePtr pDevice);
private:
	GIODevicePtr m_pDevice;
};
#endif