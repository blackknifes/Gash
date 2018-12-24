#ifndef __GMULTISTREAM_H__
#define __GMULTISTREAM_H__
#include "GStream.h"

class GMultiOutputStream : public GOutputStream
{
public:
	GMultiOutputStream();
	GMultiOutputStream(GOutputStreamPtr os, ...);
	void add(GOutputStreamPtr os, ...);
	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;

	virtual void end() override;

private:
	GObjectClassPooled(GMultiOutputStream);
	std::vector<GOutputStreamPtr> m_outputStreamList;
};
#endif