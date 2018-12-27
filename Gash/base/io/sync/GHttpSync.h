#ifndef __GHTTPSYNC_H__
#define __GHTTPSYNC_H__
#include "GSocketSync.h"
#include "../../stream/GHttpParserStream.h"
#include "../../stream/GUnpackStream.h"

class GHttpSync: public GSocketSync
{
public:
	GHttpSync();
	~GHttpSync();
private:
	GUnpackStreamPtr m_unpackStream;
	GHttpParserStreamPtr m_httpParserStream;
};
#endif