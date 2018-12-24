#ifndef __GENCODESTREAM_H__
#define __GENCODESTREAM_H__
#include "GStream.h"
#include "../GDataArray.h"

GREFPTR_DEF(GEncodeStream);

class GEncodeStream: public GOutputStream
{
public:
	enum Type
	{
		Md5,
		Base64
	};
	
	GEncodeStream(Type type, GOutputStreamPtr pOut);

	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;
	virtual void end() override;

private:
	GObjectClassPooled(GEncodeStream);
	GOutputStreamPtr m_pOutputStream;
	Type m_type;
	void* m_pContext;
};
#endif