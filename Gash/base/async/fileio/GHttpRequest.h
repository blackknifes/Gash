#ifndef __GHTTPREQUEST_H__
#define __GHTTPREQUEST_H__
#include <map>
#include <string>
#include <vector>
#include "../../stream/GStream.h"

class GHttpRequest: public GOutputStream
{
public:
	typedef std::map<std::string, std::string> HeaderMap;
	typedef std::vector<std::string> RawHeaderList;

	void addHeader(const char* key, const char* val);

private:
	RawHeaderList m_rawHeaderList;
};
#endif
