#ifndef __GGZIPSTREAM_H__
#define __GGZIPSTREAM_H__
#include "GStream.h"
#include "../GDataArray.h"

GREFPTR_DEF(GGUncompressStream);

class GGUncompressStream: public GOutputStream
{
public:	
	GGUncompressStream(GOutputStreamPtr os);
	~GGUncompressStream();
	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;
private:
	GObjectClassPooled(GGUncompressStream);
	GOutputStreamPtr m_pOutputStream;
	void* m_pGZipContext;
};
#endif