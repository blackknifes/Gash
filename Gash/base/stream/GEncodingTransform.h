#ifndef __GENCODINGTRANSFORM_H__
#define __GENCODINGTRANSFORM_H__
#include "GStream.h"

GREFPTR_DEF(GEncodingTransform);

class GEncodingTransform: public GOutputStream
{
public:
	using Encoding = GDataArray::Encoding;

	GEncodingTransform(GOutputStreamPtr pOut, Encoding from, Encoding to);

	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;

private:
	GObjectClassPooled(GEncodingTransform);
	GOutputStreamPtr m_pOutputStream;
	Encoding m_from;
	Encoding m_to;
};
#endif
