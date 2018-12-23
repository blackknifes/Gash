#include "GUrlParts.h"
#include "GRegex.h"
#include <unordered_map>

namespace
{
	struct WChartArrayHash
	{
		size_t operator()(const wchar_t* str) const
		{
			return std::_Hash_array_representation(str, wcslen(str));
		}

		bool operator()(const wchar_t* left, const wchar_t* right) const
		{
			return wcscmp(left, right) == 0;
		}
	};
}

static std::unordered_map<const wchar_t*, int, WChartArrayHash, WChartArrayHash> defaultProtocols = {
	{L"http", 80},
	{L"https", 443},
	{L"ftp", 21},
	{L"ssh", 22},
	{L"git", 22},
	{L"ws", 80},
	{L"wss", 443}
};

GUrlParts::GUrlParts()
{

}

GUrlParts::GUrlParts(const GString& url)
{
	parse(url);
}

const GString& GUrlParts::getUrl() const
{
	return m_strUrl;
}

const GStringPiece& GUrlParts::getOrigin() const
{
	return m_strOrigin;
}

const GStringPiece& GUrlParts::getProtocol() const
{
	return m_strProtocol;
}

const GStringPiece& GUrlParts::getHost() const
{
	return m_strHost;
}

const GStringPiece& GUrlParts::getHostname() const
{
	return m_strHostname;
}

const GStringPiece& GUrlParts::getUsername() const
{
	return m_strUsername;
}

const GStringPiece& GUrlParts::getPassword() const
{
	return m_strPassword;
}

const GStringPiece& GUrlParts::getPathname() const
{
	return m_strPathname;
}

const GStringPiece& GUrlParts::getSearch() const
{
	return m_strSearch;
}

const GStringPiece& GUrlParts::getHash() const
{
	return m_strHash;
}

const unsigned short& GUrlParts::getPort() const
{
	return m_iPort;
}

bool GUrlParts::isSecureProtocol() const
{
	return m_iPort == 443;
}

void GUrlParts::parse(const GString& val)
{
	//						protocol        username@     password      hostname:    port      pathname     search     hash
	static GRegex s_regex(L"([^:/@\\?#]+://)?([^:/@\\?#]+:[\\s\\S]+@)?([^:/\\?#]+)?(:[0-9]+)?(/[^\\?#]+)?(\\?[^#]+)?(#.+)?");
	m_strUrl = val;
	GRegex::MatchResults results = s_regex.match(m_strUrl);
	if (results.size() != 8)
		return;
	GStringPiece tmp = GRegex::StringPiece(results[1]);
	if(!tmp.empty())
		m_strProtocol = GStringPiece(tmp.begin(), tmp.end() - 2);
	tmp = GRegex::StringPiece(results[2]);
	if (!tmp.empty())
	{
		size_t offset = tmp.find(L":");
		if (offset != -1)
		{
			m_strUsername = tmp.substr(0, offset);
			m_strPassword = tmp.substr(offset + 1, tmp.size() - 1);
		}
	}
	m_strHostname = GRegex::StringPiece(results[3]);

	tmp = GRegex::StringPiece(results[4]);
	if (!tmp.empty())
	{
		tmp = GStringPiece(tmp.begin() + 1, tmp.end());
		m_iPort = tmp.toInt();
	}

	m_strPathname = GRegex::StringPiece(results[5]);
	m_strSearch = GRegex::StringPiece(results[6]);
	if(!m_strSearch.empty())
		m_strSearch = GStringPiece(m_strSearch.begin() + 1, m_strSearch.end());
	m_strHash = GRegex::StringPiece(results[7]);
	if (!m_strHash.empty())
		m_strHash = GStringPiece(m_strHash.begin() + 1, m_strHash.end());

	if(!m_strHostname.empty())
		m_strHost = GStringPiece(m_strHostname.begin().toStdIterator(), results[4].second);
	if(!m_strHost.empty())
		m_strOrigin = GStringPiece(m_strUrl.begin(), m_strHost.end().toStdIterator());

	if (m_iPort == 0)
	{
		wchar_t protocol[128];
		size_t readsize = m_strProtocol.copy(protocol, 128);
		if (readsize == 0 || readsize == 128)
			return;
		protocol[readsize - 1] = 0;
		auto itor = defaultProtocols.find(protocol);
		if (itor != defaultProtocols.end())
			m_iPort = itor->second;
	}
}

void GUrlParts::inspect() const
{
	wprintf_s(L"url: %s\n", m_strUrl.data());
	wprintf_s(L"origin: %s\n", m_strOrigin.toString().data());
	wprintf_s(L"protocol: %s\n", m_strProtocol.toString().data());
	wprintf_s(L"username: %s\n", m_strUsername.toString().data());
	wprintf_s(L"password: %s\n", m_strPassword.toString().data());
	wprintf_s(L"host: %s\n", m_strHost.toString().data());
	wprintf_s(L"hostname: %s\n", m_strHostname.toString().data());
	wprintf_s(L"port: %d\n", m_iPort);
	wprintf_s(L"pathname: %s\n", m_strPathname.toString().data());
	wprintf_s(L"search: %s\n", m_strSearch.toString().data());
	wprintf_s(L"hash: %s\n", m_strHash.toString().data());
}
