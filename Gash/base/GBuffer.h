#ifndef __GBUFFER_H__
#define __GBUFFER_H__
#include "GObject.h"

GREFPTR_DEF(GBuffer);

class GBuffer: public virtual GObject
{
public:
	static GBufferPtr CreateBuffer(size_t size);
	GBuffer();

	const unsigned char* data() const;
	unsigned char* data();
	size_t size() const;
private:
	GObjectClassPooled(GBuffer);
	~GBuffer();

	unsigned char* m_pData;
	size_t m_size;
};
#endif