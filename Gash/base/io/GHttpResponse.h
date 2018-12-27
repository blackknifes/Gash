#ifndef __GHTTPRESPONSE_H__
#define __GHTTPRESPONSE_H__
#include "../GObject.h"
#include <map>

GREFPTR_DEF(GHttpResponse);

class GHttpResponse: public virtual GObject
{
public:
	typedef std::map<std::string, std::string> HeadersMap;
	typedef std::map<std::string, std::string> CookiesMap;
	GHttpResponse();
	~GHttpResponse();


	void addHeader(const char* key, const char* value);
	const char* getHeader(const char* key) const;
	void addCookie(const char* key, const char* value);
	const char* getCookie(const char* key) const;

private:
	GObjectClassPooled(GHttpResponse);
	HeadersMap m_headers;
	CookiesMap m_cookiesMap;
};
#endif