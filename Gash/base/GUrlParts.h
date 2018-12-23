#ifndef __GURLPARTS_H__
#define __GURLPARTS_H__
#include "GString.h"
#include "GStringPiece.h"

class GUrlParts
{
public:
	GUrlParts();
	GUrlParts(const GString& url);

	const GString& getUrl() const;
	const GStringPiece& getOrigin() const;
	const GStringPiece& getProtocol() const;
	const GStringPiece& getHost() const;
	const GStringPiece& getHostname() const;
	const GStringPiece& getUsername() const;
	const GStringPiece& getPassword() const;
	const GStringPiece& getPathname() const;
	const GStringPiece& getSearch() const;
	const GStringPiece& getHash() const;
	const unsigned short& getPort() const;

	bool isSecureProtocol() const;

	void parse(const GString& val);
	void inspect() const;
private:
	GString m_strUrl;
	GStringPiece m_strOrigin;
	GStringPiece m_strProtocol;
	GStringPiece m_strUsername;
	GStringPiece m_strPassword;
	GStringPiece m_strHost;
	GStringPiece m_strHostname;
	GStringPiece m_strPathname;
	GStringPiece m_strSearch;
	GStringPiece m_strHash;
	unsigned short m_iPort;
};
#endif