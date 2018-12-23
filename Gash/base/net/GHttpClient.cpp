#include "GHttpClient.h"
#include "../GStringPiece.h"
#include "../GPath.h"

static size_t HeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata)
{
	GHttpClient* pClient = (GHttpClient*)userdata;
	return pClient->onRecvHeader(buffer, size, nitems, userdata);
}

GHttpClient::GHttpClient()
	:m_pCurl(curl_easy_init())
{
	init();
}

GHttpClient::GHttpClient(const GString& url)
	: m_pCurl(curl_easy_init())
{
	init();
	setUrl(url);
}

GHttpClient::~GHttpClient()
{
	curl_easy_cleanup(m_pCurl);
}

void GHttpClient::reset()
{
	curl_easy_reset(m_pCurl);
}

void GHttpClient::setDebug(bool enabled)
{
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, enabled? 1 : 0);
}

void GHttpClient::setUrl(const GString& url)
{
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_URL, url.toUtf8().data());
}

void GHttpClient::setDnsTimeout(int timeout /*= 5000*/)
{
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, timeout);
}

void GHttpClient::useGlobalDnsCache(bool enabled)
{
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_DNS_USE_GLOBAL_CACHE, (int)enabled);
}

void GHttpClient::setBufferSize(size_t size)
{
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_BUFFERSIZE, (long)size);
}

void GHttpClient::setPort(int port)
{
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_PORT, port);

}

void GHttpClient::setTcpNodelay(bool enabled)
{
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_TCP_NODELAY, (int)enabled);
}

void GHttpClient::setKeepAlive(bool enabled)
{
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_TCP_KEEPALIVE, (int)enabled);
}

void GHttpClient::setKeepIdle(int time)
{
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_TCP_KEEPIDLE, time);
}

void GHttpClient::setKeepInterval(int time)
{
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_TCP_KEEPINTVL, time);
}

void GHttpClient::skipCertificationCheck(bool enabled)
{
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, (int)!enabled);
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, (int)!enabled);
}

void GHttpClient::setFollowLocation(bool enabled)
{
	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, (int)enabled);
}

void GHttpClient::setHeader(const GString& key, const GString& value)
{
	auto itor = m_headers.find(key);
	if (itor == m_headers.end())
		itor = m_headers.insert(HeadersMap::value_type(key, GString())).first;
	if(!itor->second.empty())
		itor->second.addEndsWith(L";");
	itor->second.append(GStringPiece(value));
}

bool GHttpClient::hasHeader(const GString& key) const
{
	auto itor = m_headers.find(key);
	return itor != m_headers.end();
}

const GString& GHttpClient::getHeader(const GString& key) const
{
	return m_headers.at(key);
}

bool GHttpClient::execute()
{
	curl_slist* pList = nullptr;
	if (!m_headers.empty())
	{
		GString str;
		str.reserve(1024);
		for (const auto& pair : m_headers)
		{
			str = pair.first;
			str.push_back(':');
			str += GStringPiece(pair.second);
			pList = curl_slist_append(pList, str.toUtf8().data());
			str.clear();
		}
		curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, pList);
	}

	m_errorCode = curl_easy_perform(m_pCurl);
	if(pList)
		curl_slist_free_all(pList);
	return m_errorCode == CURLE_OK;
}

size_t GHttpClient::read(void* pData, size_t bytes)
{
	size_t readSize = 0;
	m_errorCode = curl_easy_recv(m_pCurl, pData, bytes, &readSize);
	return readSize;
}

size_t GHttpClient::write(const void* pData, size_t bytes)
{
	size_t writeSize = 0;
	m_errorCode = curl_easy_send(m_pCurl, pData, bytes, &writeSize);
	return writeSize;
}

GDataArray GHttpClient::readAll()
{
	return GDataArray();
}

void GHttpClient::flush()
{
	
}

__int64 GHttpClient::seekRead(__int64 offset, SeekType pos /*= SeekCur*/)
{
	return 0;
}

__int64 GHttpClient::seekWrite(__int64 offset, SeekType pos /*= SeekCur*/)
{
	return 0;
}

void GHttpClient::close()
{
	curl_easy_reset(m_pCurl);
}

bool GHttpClient::isClosed() const
{
	curl_socket_t s;
	return curl_easy_getinfo(m_pCurl, CURLINFO_ACTIVESOCKET, &s) != CURL_SOCKET_BAD;
}

int GHttpClient::getErrorCode() const
{
	return m_errorCode;
}

GString GHttpClient::getErrorMessage() const
{
	return GString::FromUtf8(m_strErrorBuffer);
}

__int64 GHttpClient::tellRead() const
{
	return 0;
}

__int64 GHttpClient::tellWrite() const
{
	return 0;
}

void GHttpClient::init()
{
	curl_easy_setopt(m_pCurl, CURLOPT_HEADERFUNCTION, HeaderCallback);
	curl_easy_setopt(m_pCurl, CURLOPT_HEADERDATA, this);
	curl_easy_setopt(m_pCurl, CURLOPT_ERRORBUFFER, m_strErrorBuffer);
	curl_easy_setopt(m_pCurl, CURLOPT_DNS_USE_GLOBAL_CACHE, 1l);
	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 3000l);
	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1l);
	curl_easy_setopt(m_pCurl, CURLOPT_MAXREDIRS, 10l);
	curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, 3000l);
	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1l);
	curl_easy_setopt(m_pCurl, CURLOPT_AUTOREFERER, 1l);
#ifdef _DEBUG
	GString str = GPath::Resolve(GPath::GetExeDir().data(), L"../../../cacert.pem", nullptr);
	curl_easy_setopt(m_pCurl, CURLOPT_CAINFO, str.toAnsi().data());
#endif
}

size_t GHttpClient::onRecvHeader(char *buffer, size_t size, size_t nitems, void *userdata)
{
	size_t realSize = size * nitems;
	GString str = GString::FromUtf8(buffer, realSize);
	size_t offset = str.find(L":", 0);
	if (offset == GString::npos)
		return realSize;
	GString key = str.substr(0, offset).trim().toLower();
	GString value = str.substr(offset + 1).trim().removeEndsWith(L"\r\n");
	auto itor = m_headers.find(key);
	if (itor == m_headers.end())
		itor = m_headers.insert(HeadersMap::value_type(key, GString())).first;
	if (!itor->second.empty())
		itor->second.push_back(L';');
	itor->second.append(GStringPiece(value));
	return realSize;
}
