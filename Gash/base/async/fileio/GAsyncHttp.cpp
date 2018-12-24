// #include "GAsyncHttp.h"
// #include <regex>
// #include <intsafe.h>
// 
// #define HTTP_END 0
// #define HTTP_STATUSLINE 1
// #define HTTP_HEADERS 2
// #define HTTP_RECV_CONTENT 3
// #define HTTP_RECV_CHUNKED_LENGTH 4
// #define HTTP_RECV_CHUNKED_CONTENT 5
// #define HTTP_WANT_RECV_MORE 0xF001
// 
// 
// #define HTTP_ERRPR_PARSE_STATUSLINE (-HTTP_STATUSLINE)
// #define HTTP_ERROR_PARSE_HEADERS (-HTTP_HEADERS)
// #define HTTP_ERROR_PARSE_RECV_CONTENT (-HTTP_RECV_CONTENT)
// #define HTTP_ERROR_PARSE_RECV_CHUNKED_LENGTH (-HTTP_RECV_CHUNKED_LENGTH)
// #define HTTP_ERROR_PARSE_RECV_CHUNKED_CONTENT (-HTTP_RECV_CHUNKED_CONTENT)
// 
// 
// void GAsyncHttp::setState(int state)
// {
// 	m_recvStatus = state;
// }
// 
// size_t GAsyncHttp::onRead(const void* pData, size_t size)
// {
// 	bool result = true;
// 	while (result)
// 	{
// 		switch (m_recvStatus)
// 		{
// 		case HTTP_END:
// 			break;
// 		case HTTP_STATUSLINE:
// 			m_bufferStream.write(pData, size);
// 			readStatusLine();
// 			break;
// 		case HTTP_HEADERS:
// 			m_bufferStream.write(pData, size);
// 			readHeader();
// 			break;
// 		case HTTP_RECV_CONTENT:
// 			readContent(pData, size);
// 			break;
// 		case HTTP_RECV_CHUNKED_LENGTH:
// 			readChunkedLength();
// 			break;
// 		case HTTP_RECV_CHUNKED_CONTENT:
// 			readChunkedData(pData, size);
// 			break;
// 		default:
// 			break;
// 		}
// 	}
// }
// 
// bool GAsyncHttp::readStatusLine()
// {
// 	static std::regex re = std::regex(R"(([^/]+/\d\.\d)\s+(\d)+\s+([^\r\n]+)");
// 	char buf[1024] = {};
// 	char tmp[64] = {};
// 
// 	size_t size = m_bufferStream.readline(buf, sizeof(buf));
// 	if (size == -1)
// 		return false;
// 	std::match_results<char*> results;
// 	std::regex_match(buf, buf + size, results, re);
// 	if (results.size() < 3)
// 	{
// 		setState(HTTP_ERRPR_PARSE_STATUSLINE);
// 		m_bufferStream.close();
// 		return false;
// 	}
// 
// 	std::copy(results[1].first, results[1].second, tmp);
// 	atoi(tmp);
// 	m_statusMessage.assign(results[2].first, results[2].second);
// 	setState(HTTP_HEADERS);
// 	return true;
// }
// 
// 
// bool GAsyncHttp::readHeader()
// {
// 	static std::regex re = std::regex(R"(\s*([^ :\t\v]+)\s*:\s*(\S+)\s*)");
// 
// 	m_body.resize(USHORT_MAX);
// 	size_t size;
// 	while ((size = m_bufferStream.readline((char*)m_body.data(), m_body.size())) != -1)
// 	{
// 		if (size == 0)
// 		{
// 			setState(m_isChunked ? HTTP_RECV_CHUNKED_LENGTH : HTTP_RECV_CHUNKED_CONTENT);
// 			return true;
// 		}
// 		m_body.resize(size);
// 		std::match_results<std::string::iterator> results;
// 		if (results.size() != 2)
// 			continue;
// 		std::string headerKey;
// 		std::string headerValue;
// 		std::copy(results[1].first, results[1].second, headerKey);
// 		std::copy(results[2].first, results[2].second, headerValue);
// 		for (auto itor = headerKey.begin(); itor != headerKey.end(); ++itor)
// 			*itor = ::tolower(*itor);
// 
// 		if (headerKey == "content-length")
// 			m_contentLength = (size_t)atoll(headerValue.data());
// 
// 		auto itor = m_headers.find(headerKey);
// 		if (itor == m_headers.end())
// 			itor = m_headers.insert(HeaderMap::value_type(headerKey, "")).first;
// 		else
// 			itor->second += "; ";
// 		itor->second += headerValue;
// 	}
// 	return true;
// }
// 
// bool GAsyncHttp::readContent(const void* pData, size_t size)
// {
// 	if (m_contentLength <= m_body.size() + size)
// 	{
// 		setState(HTTP_END);
// 		size_t writeSize = m_contentLength - m_body.size();
// 		m_body.append((const char*)pData, writeSize);
// 		m_bufferStream.write((unsigned char*)pData + writeSize, size - writeSize);
// 		return false;
// 	}
// 	else
// 		m_body.append((const char*)pData, size);
// 	return false;
// }
// 
// bool GAsyncHttp::readChunkedLength()
// {
// 	char buf[64] = {};
// 	size_t len = m_bufferStream.readline(buf, sizeof(buf));
// 	if (len == 0)
// 	{
// 		setState(HTTP_ERROR_PARSE_RECV_CHUNKED_LENGTH);
// 		return true;
// 	}
// 	char* ptr;
// 	m_nextChunkedLength = strtoull(buf, &ptr, 16);
// 	if (m_nextChunkedLength == 0)
// 		setState(HTTP_END);
// 	else
// 		setState(HTTP_RECV_CHUNKED_CONTENT);
// 	return true;
// }
// 
// bool GAsyncHttp::readChunkedData(const void* pData, size_t size)
// {
// 	if (size <= m_nextChunkedLength)
// 		m_body.append((const char*)pData, size);
// 	else
// 	{
// 		m_body.append((const char*)pData, m_nextChunkedLength);
// 		m_bufferStream.write((const char*)pData + m_nextChunkedLength + 2, size - m_nextChunkedLength - 2);
// 		setState(HTTP_RECV_CHUNKED_LENGTH);
// 		return true;
// 	}
// 	if ((m_nextChunkedLength -= size) == 0)
// 		setState(HTTP_RECV_CHUNKED_LENGTH);
// 	return true;
// }