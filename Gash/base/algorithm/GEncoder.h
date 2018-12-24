#ifndef __GENCODER_H__
#define __GENCODER_H__
#include "../GDataArray.h"

class GEncoder
{
public:
	enum Type
	{
		Md5,
		Base64
	};

	GEncoder(Type type);
	~GEncoder();

	void encode(const void* pData, size_t bytes, GDataArray& dataArray);
	void end(GDataArray& dataArray);
private:
	void init();

	Type m_type;
	void* m_pContext;
	bool m_isInit;
};
#endif