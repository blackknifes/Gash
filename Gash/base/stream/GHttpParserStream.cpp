#include "GHttpParserStream.h"
#include <regex>
#include "../GHashConst.h"
#include "zlib.h"
#include "../GTempMemory.h"

#define HTTP_PARSER_STATUSLINE 1
#define HTTP_PARSER_HEADERLINE 2
#define HTTP_PARSER_CONTENT 3
#define HTTP_PARSER_CHUNKED_LENGTH 4
#define HTTP_PARSER_CHUNKED_CONTENT 5
#define HTTP_PARSER_END 5
#define HTTP_PARSER_ERROR -1

typedef std::pair<const char*, const char*> StrPair;

static char* ToLowerString(char* buf)
{
	size_t i = 0;
	while (buf[i] != 0)
	{
		buf[i] = ::tolower(buf[i]);
		++i;
	}
	return buf;
}

static char* TrimString(char* buf)
{
	size_t frontIndex = 0;
	size_t backIndex = strlen(buf);
	while (buf[frontIndex] != 0 && ::isblank(buf[frontIndex]))
		++frontIndex;
	while (backIndex > 0 && ::isblank(buf[backIndex - 1]))
		--backIndex;
	if (backIndex <= frontIndex)
	{
		buf[0] = 0;
		return buf;
	}
	size_t len = backIndex - frontIndex;
	if (frontIndex != 0)
	{
		for (size_t i = 0; i < len; ++i)
			buf[i] = buf[i + frontIndex];
	}
	buf[len] = 0;
	return buf;
}

static void TrimString(std::string& str)
{
	while (!str.empty() && ::isblank(str.back()))
		str.pop_back();
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (!::isblank(str[i]))
		{
			if(i != 0)
				str.erase(0, i);
			break;
		}
	}
}

static bool StrNCaseCmpWith(const char* left, const char* right)
{
	size_t i = 0;
	while (::tolower(left[i]) == ::tolower(right[i]) && left[i] != 0 && right[i] != 0)
		++i;
	return left[i] == right[i];
}

static size_t SplitString(const char* left, char split, StrPair* pairs, size_t len)
{
	size_t i = 0;
	const char* first = left;
	const char* second = left;

	while (*second != 0)
	{
		if (*second == split)
		{
			if (second != first)
			{
				pairs[i++] = StrPair(first, second);
				if (i == len)
					return i;
			}
			first = ++second;
			continue;
		}
		++second;
	}
	if(second != first)
		pairs[i++] = StrPair(first, second);
	return i;
}

GHttpParserStreamPtr GHttpParserStream::Create()
{
	return GHttpParserStream::CreatePooled();
}

GHttpParserStream::GHttpParserStream()
	: m_state(HTTP_PARSER_STATUSLINE),
	m_expectIndex(0),
	m_contentLength(0), m_remaingSize(0), 
	m_inflateStream(nullptr),
	m_hasContentLength(false), m_hasChunked(false), m_isGZip(false)
{
}

GHttpParserStream::~GHttpParserStream()
{

}

void GHttpParserStream::write(const void* pData, size_t _size)
{
	writeData(pData, _size);
}

void GHttpParserStream::flush()
{
	
}

void GHttpParserStream::parseStatusLine(const void* pData, size_t _size)
{
	static std::regex re = std::regex(R"(([^/]+/.+)\s+(\d+)\s+(.*))");

	std::match_results<const char*> results;
	std::regex_match((const char*)pData, (const char*)pData + _size, results, re);
	if (results.size() != 4 || results[1].length() >= 64 || results[2] >= 64)
		return setError();

	char version[64];
	char statusCodeStr[64];
	*std::copy(results[1].first, results[1].second, version) = 0;
	*std::copy(results[2].first, results[2].second, statusCodeStr) = 0;

	GTempMemory(results[3].length() + 1, [this, &results, &version, &statusCodeStr](void* pData) {
		char* statusMessage = (char*)pData;
		*std::copy(results[3].first, results[3].second, statusMessage) = 0;
		if (m_statusLineCallback)
			m_statusLineCallback(version, ::atoi(statusCodeStr), statusMessage);
	});
	m_state = HTTP_PARSER_HEADERLINE;
}

void GHttpParserStream::parseHeaderLine(const void* pData, size_t _size)
{
	static std::regex re = std::regex(R"(([^:\s]+):([\s\S]*))");
	if (_size == 0)
	{
		if (m_hasChunked)
			m_state = HTTP_PARSER_CHUNKED_LENGTH;
		else if (m_hasContentLength)
			m_state = HTTP_PARSER_CONTENT;
		else
			setError();
		return;
	}
	std::match_results<const char*> results;
	std::regex_match((const char*)pData, (const char*)pData + _size, results, re);
	if (results.size() != 3 || results[1].length() >= 1024)
		return setError();
	char key[1024] = {};
	*std::copy(results[1].first, results[1].second, key) = 0;
	ToLowerString(TrimString(key));

	GTempMemory(results[2].length() + 1, [this, &results, &key](void* pData) {
		char* value = (char*)pData;
		*std::copy(results[2].first, results[2].second, value) = 0;
		TrimString(value);
		parseHeaderValue(key, value);
		if (m_headerCallback)
			m_headerCallback(key, value);
	});
}

void GHttpParserStream::parseContent(const void* pData, size_t _size)
{
	if (!m_bodyCallback)
		return;
	if (m_contentLength < _size)
		_size = m_contentLength;
	m_contentLength -= _size;
	if (!m_isGZip)
		return m_bodyCallback(pData, _size);

	inflateData(pData, _size);
}

void GHttpParserStream::parseChunkedLength(const void* pData, size_t _size)
{
	if (_size >= 16)
		return setError();
	char buf[16];
	*std::copy((char*)pData, (char*)pData + _size, buf) = 0;
	m_remaingSize = strtoull(buf, nullptr, 16);
	if (m_remaingSize == 0)
	{
		m_state = HTTP_PARSER_END;
		return;
	}
	m_state = HTTP_PARSER_CHUNKED_CONTENT;
}

void GHttpParserStream::parseChunkedContent(const void* pData, size_t _size)
{
	if (!m_bodyCallback)
		return;
	if (_size != m_remaingSize)
		return setError();
	m_state = HTTP_PARSER_CHUNKED_LENGTH;
	if (!m_isGZip)
		return m_bodyCallback(pData, _size);
	inflateData(pData, _size);
}

void GHttpParserStream::parseHeaderValue(const char* key, const char* value)
{
	switch (GHashString(key))
	{
	case "content-length"_hash:
		parseContentLength(value);
		break;
	case "transfer-encoding"_hash:
		parseTransferEncoding(value);
		break;
	case "content-encoding"_hash:
		parseContentEncoding(value);
		break;
	case "set-cookie"_hash:
		parseCookie(value);
		break;
	default:
		break;
	}
}

void GHttpParserStream::parseContentLength(const char* value)
{
	m_hasContentLength = true;
	m_contentLength = ::strtoull(value, nullptr, 10);
}

void GHttpParserStream::parseTransferEncoding(const char* value)
{
	if(StrNCaseCmpWith(value, "chunked"))
		m_hasChunked = true;
}

void GHttpParserStream::parseContentEncoding(const char* value)
{
	if (StrNCaseCmpWith(value, "gzip"))
	{
		m_isGZip = true;
		m_inflateStream = GNewObject<z_stream>();
		z_streamp zlibStream = (z_streamp)m_inflateStream;
		if (inflateInit2(zlibStream, 47) != 0)
		{
			GDestroyObject(zlibStream);
			m_inflateStream = nullptr;
			setError();
			return;
		}
	}
}

void GHttpParserStream::parseCookie(const char* value)
{
	if (!m_cookieCallback)
		return;
	StrPair pairs[128];

	size_t len = SplitString(value, ';', pairs, 128);
	char key[256];
	for (size_t i = 0; i < len; ++i)
	{
		StrPair& cookiePair = pairs[i];

		const char* ptr = std::find(cookiePair.first, cookiePair.second, '=');
		if(ptr == nullptr || ptr - cookiePair.first >= 256)
			continue;
		*std::copy(cookiePair.first, ptr, key) = 0;
		ToLowerString(TrimString(key));

		++ptr;
		size_t valLen = cookiePair.second - ptr;

		GTempMemory(valLen + 1, [this, &ptr, &cookiePair, &key](void* pData) {
			char* valBuf = (char*)pData;
			*std::copy(ptr, cookiePair.second, valBuf) = 0;
			TrimString(valBuf);
			m_cookieCallback(key, valBuf);
		});
	}
}

void GHttpParserStream::setStatusLineCallback(const StatusLineCallback& cb)
{
	m_statusLineCallback = cb;
}

void GHttpParserStream::setHeaderCallback(const HeaderCallback& cb)
{
	m_headerCallback = cb;
}

void GHttpParserStream::setBodyCallback(const BodyCallback& cb)
{
	m_bodyCallback = cb;
}

void GHttpParserStream::setCookieCallback(const CookieCallback& cb)
{
	m_cookieCallback = cb;
}

void GHttpParserStream::writeData(const void* pData, size_t _size)
{
	static const char expectData[] = "\r\n";
	static constexpr size_t expectSize = sizeof(expectData) - 1;

	switch (m_state)
	{
	case HTTP_PARSER_STATUSLINE:
	case HTTP_PARSER_HEADERLINE:
	case HTTP_PARSER_CHUNKED_LENGTH:
	case HTTP_PARSER_CHUNKED_CONTENT:
		break;
	case HTTP_PARSER_CONTENT:
		parseContent(pData, _size);
		break;
	case HTTP_PARSER_ERROR:
		return;
	default:
		break;
	}

	const char* ptr = (const char*)pData;
	size_t preIndex = 0;
	for (size_t i = 0; i < _size; ++i)
	{
		if (ptr[i] != expectData[m_expectIndex])
		{
			if (ptr[i] == expectData[0] && ++m_expectIndex == expectSize)
			{
				m_expectIndex = 0;
				writeLine();
				continue;
			}
			if (m_expectIndex != 0)
			{
				m_bufferIO.writeSync(expectData, m_expectIndex);
				m_expectIndex = 0;
			}
			m_bufferIO.writeSync(ptr + i, 1);
		}
		else if (++m_expectIndex == expectSize)
		{
			m_expectIndex = 0;
			writeLine();
		}
	}
}

void GHttpParserStream::writeLine()
{
	GTempMemory(m_bufferIO.size(), [this](void* pData) {
		if (m_bufferIO.size() == 0)
			return parseData(nullptr, 0);
		size_t readsize;
		readsize = m_bufferIO.readSync(pData, m_bufferIO.size());
		parseData(pData, readsize);
	});
}

size_t GHttpParserStream::writeDataWithSize(const void* pData, size_t len)
{
	
}

void GHttpParserStream::parseData(const void* pData, size_t _size)
{
	switch (m_state)
	{
	case HTTP_PARSER_STATUSLINE:
		parseStatusLine(pData, _size);
		break;
	case HTTP_PARSER_HEADERLINE:
		parseHeaderLine(pData, _size);
		break;
	case HTTP_PARSER_CONTENT:
		parseContent(pData, _size);
		break;
	case HTTP_PARSER_CHUNKED_LENGTH:
		parseChunkedLength(pData, _size);
		break;
	case HTTP_PARSER_CHUNKED_CONTENT:
		parseChunkedContent(pData, _size);
		break;
	case HTTP_PARSER_ERROR:
		return;
	default:
		break;
	}
}

void GHttpParserStream::setError()
{
	m_state = HTTP_PARSER_ERROR;
}

void GHttpParserStream::inflateData(const void* pData, size_t _size)
{
	z_streamp zlibStream = (z_streamp)m_inflateStream;
	Bytef buf[0x4000] = {};

	int result;
	zlibStream->next_in = (Bytef*)pData;
	zlibStream->avail_in = _size;
	do
	{
		bool bBreak = false;
		zlibStream->next_out = buf;
		zlibStream->avail_out = sizeof(buf);
		result = inflate(zlibStream, Z_SYNC_FLUSH);
		switch (result)
		{
		case Z_OK:
		case Z_BUF_ERROR:
			m_bodyCallback(buf, sizeof(buf) - zlibStream->avail_out);
			break;
		default:
			bBreak = true;
			break;
		}
		if (bBreak)
			break;
	} while (zlibStream->avail_in != 0);
	if (result == Z_STREAM_END)
	{
		inflateEnd(zlibStream);
		GDestroyObject(zlibStream);
		zlibStream = nullptr;
	}
}
