// #include "GHttpClient.h"
// #include "../GStringPiece.h"
// #include "../GPath.h"
// 
// static size_t HeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata)
// {
// 	GHttpClient* pClient = (GHttpClient*)userdata;
// 	return pClient->onRecvHeader(buffer, size, nitems, userdata);
// }
// 
// static size_t StreamWriteData(char * ptr, size_t size, size_t nmemb, void* userdata)
// {
// 	size_t writeSize = size * nmemb;
// 	GOutputStreamPtr pOut = (GOutputStream*)userdata;
// 	pOut->write(ptr, writeSize);
// 	return writeSize;
// }
// 
// static size_t StreamReadData(char* buffer, size_t size, size_t nitems, void* userdata)
// {
// 	GInputStreamPtr pInput = (GInputStream*)userdata;
// 	return pInput->read(buffer, size * nitems);
// }
// 
// GHttpClient::GHttpClient()
// 	:m_pCurl(curl_easy_init())
// {
// 	init();
// }
// 
// GHttpClient::GHttpClient(const GString& url)
// 	: m_pCurl(curl_easy_init())
// {
// 	init();
// 	setUrl(url);
// }
// 
// GHttpClient::~GHttpClient()
// {
// 	curl_easy_cleanup(m_pCurl);
// }
// 
// void GHttpClient::reset()
// {
// 	curl_easy_reset(m_pCurl);
// }
// 
// void GHttpClient::setDebug(bool enabled)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, enabled? 1 : 0);
// }
// 
// void GHttpClient::setUrl(const GString& url)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_URL, url.toUtf8().data());
// }
// 
// void GHttpClient::setDnsTimeout(int timeout /*= 5000*/)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, timeout);
// }
// 
// void GHttpClient::useGlobalDnsCache(bool enabled)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_DNS_USE_GLOBAL_CACHE, (int)enabled);
// }
// 
// void GHttpClient::setBufferSize(size_t size)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_BUFFERSIZE, (long)size);
// }
// 
// void GHttpClient::setPort(int port)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_PORT, port);
// 
// }
// 
// void GHttpClient::setTcpNodelay(bool enabled)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_TCP_NODELAY, (int)enabled);
// }
// 
// void GHttpClient::setKeepAlive(bool enabled)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_TCP_KEEPALIVE, (int)enabled);
// }
// 
// void GHttpClient::setKeepIdle(int time)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_TCP_KEEPIDLE, time);
// }
// 
// void GHttpClient::setKeepInterval(int time)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_TCP_KEEPINTVL, time);
// }
// 
// void GHttpClient::skipCertificationCheck(bool enabled)
// {
// 	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, (int)!enabled);
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, (int)!enabled);
// }
// 
// void GHttpClient::setFollowLocation(bool enabled)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, (int)enabled);
// }
// 
// 
// void GHttpClient::setMaxRedirect(int numRedirect)
// {
// 	m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_MAXREDIRS, numRedirect);
// }
// 
// 
// void GHttpClient::setMethod(Method method)
// {
// 	switch (method)
// 	{
// 	case GHttpClient::Get:
// 		m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_HTTPGET, 1l);
// 		break;
// 	case GHttpClient::Post:
// 		m_errorCode = curl_easy_setopt(m_pCurl, CURLOPT_HTTPPOST, 1l);
// 		break;
// 	default:
// 		break;
// 	}
// }
// 
// void GHttpClient::setSessionId(const GString& str)
// {
// 	setHeader(L"Cookie", GString(L"jsessionid=") + str);
// }
// 
// void GHttpClient::setHeader(const GString& key, const GString& value)
// {
// 	auto itor = m_headers.find(GString(key).toLower());
// 	if (itor == m_headers.end())
// 		itor = m_headers.insert(HeadersMap::value_type(key, GString())).first;
// 	if(!itor->second.empty())
// 		itor->second.addEndsWith(L";");
// 	itor->second.append(value);
// }
// 
// bool GHttpClient::hasHeader(const GString& key) const
// {
// 	auto itor = m_headers.find(GString(key).toLower());
// 	return itor != m_headers.end();
// }
// 
// const GString& GHttpClient::getHeader(const GString& key) const
// {
// 	return m_headers.at(key);
// }
// 
// 
// void GHttpClient::setBody(GInputStreamPtr pInput)
// {
// 	m_pBodyStream = pInput;
// }
// 
// bool GHttpClient::execute()
// {
// 	curl_slist* pList = nullptr;
// 	if (!m_headers.empty())
// 	{
// 		GString str;
// 		str.reserve(1024);
// 		for (const auto& pair : m_headers)
// 		{
// 			str = pair.first;
// 			str.push_back(':');
// 			str += pair.second;
// 			pList = curl_slist_append(pList, str.toUtf8().data());
// 			str.clear();
// 		}
// 		curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, pList);
// 	}
// 	if (m_pBodyStream != nullptr)
// 	{
// 		curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, StreamReadData);
// 		curl_easy_setopt(m_pCurl, CURLOPT_READDATA, m_pBodyStream.get());
// 	}
// 
// 	m_errorCode = curl_easy_perform(m_pCurl);
// 	if(pList)
// 		curl_slist_free_all(pList);
// 	curl_easy_reset(m_pCurl);
// 	m_pBodyStream = nullptr;
// 	return m_errorCode == CURLE_OK;
// }
// 
// 
// bool GHttpClient::execute(GOutputStreamPtr pOut)
// {
// 	if (pOut != nullptr)
// 	{
// 		curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, StreamWriteData);
// 		curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, pOut.get());
// 	}
// 	bool result = execute();
// 	pOut->end();
// 	return result;
// }
// 
// size_t GHttpClient::read(void* pData, size_t bytes)
// {
// 	size_t readSize = 0;
// 	m_errorCode = curl_easy_recv(m_pCurl, pData, bytes, &readSize);
// 	return readSize;
// }
// 
// size_t GHttpClient::write(const void* pData, size_t bytes)
// {
// 	size_t writeSize = 0;
// 	m_errorCode = curl_easy_send(m_pCurl, pData, bytes, &writeSize);
// 	return writeSize;
// }
// 
// GDataArray GHttpClient::readAll()
// {
// 	return GDataArray();
// }
// 
// void GHttpClient::flush()
// {
// 	
// }
// 
// __int64 GHttpClient::seekRead(__int64 offset, SeekType pos /*= SeekCur*/)
// {
// 	return 0;
// }
// 
// __int64 GHttpClient::seekWrite(__int64 offset, SeekType pos /*= SeekCur*/)
// {
// 	return 0;
// }
// 
// void GHttpClient::close()
// {
// 	curl_easy_reset(m_pCurl);
// }
// 
// bool GHttpClient::isClosed() const
// {
// 	curl_socket_t s;
// 	return curl_easy_getinfo(m_pCurl, CURLINFO_ACTIVESOCKET, &s) != CURL_SOCKET_BAD;
// }
// 
// int GHttpClient::getErrorCode() const
// {
// 	return m_errorCode;
// }
// 
// GString GHttpClient::getErrorMessage() const
// {
// 	return GString::FromEncoding(m_strErrorBuffer, EncodingUtf8);
// }
// 
// __int64 GHttpClient::tellRead() const
// {
// 	return 0;
// }
// 
// __int64 GHttpClient::tellWrite() const
// {
// 	return 0;
// }
// 
// void GHttpClient::init()
// {
// 	curl_easy_setopt(m_pCurl, CURLOPT_HEADERFUNCTION, HeaderCallback);
// 	curl_easy_setopt(m_pCurl, CURLOPT_HEADERDATA, this);
// 	curl_easy_setopt(m_pCurl, CURLOPT_ERRORBUFFER, m_strErrorBuffer);
// 	curl_easy_setopt(m_pCurl, CURLOPT_DNS_USE_GLOBAL_CACHE, 1l);
// 	curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, 3000l);
// 	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1l);
// 	curl_easy_setopt(m_pCurl, CURLOPT_MAXREDIRS, 10l);
// 	curl_easy_setopt(m_pCurl, CURLOPT_DNS_CACHE_TIMEOUT, 3000l);
// 	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1l);
// 	curl_easy_setopt(m_pCurl, CURLOPT_AUTOREFERER, 1l);
// #ifdef _DEBUG
// 	GString str = GPath::Resolve(GPath::GetExeDir().data(), L"../../cacert.pem", nullptr);
// 	curl_easy_setopt(m_pCurl, CURLOPT_CAINFO, str.toAnsi().data());
// #endif
// }
// 
// size_t GHttpClient::onRecvHeader(char *buffer, size_t size, size_t nitems, void *userdata)
// {
// 	size_t realSize = size * nitems;
// 	GString str = GString::FromEncoding(buffer, realSize, GEncoding::EncodingUtf8);
// 	size_t offset = str.find(L":", 0);
// 	if (offset == GString::npos)
// 		return realSize;
// 	GString key = str.substr(0, offset).trim().toLower();
// 	GString value = str.substr(offset + 1).trim().removeEndsWith(L"\r\n");
// 	auto itor = m_headers.find(key);
// 	if (itor == m_headers.end())
// 		itor = m_headers.insert(HeadersMap::value_type(key, GString())).first;
// 	if (!itor->second.empty())
// 		itor->second.push_back(L';');
// 	itor->second.append(value);
// 	return realSize;
// }
