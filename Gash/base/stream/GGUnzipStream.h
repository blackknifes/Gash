#ifndef __GGZIPSTREAM_H__
#define __GGZIPSTREAM_H__
#include "GStream.h"

GREFPTR_DEF(GGUnzipStream);

class GGUnzipStream: public GOutputStream
{
public:	
	GGUnzipStream(GOutputStreamPtr os);
	~GGUnzipStream();
	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;
private:
	GObjectClassPooled(GGUnzipStream);
	GOutputStreamPtr m_pOutputStream;
	void* m_pGZipContext;
};
#endif