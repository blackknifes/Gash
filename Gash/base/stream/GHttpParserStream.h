#ifndef __GHTTPPARSERSTREAM_H__
#define __GHTTPPARSERSTREAM_H__
#include "GStream.h"
#include <functional>
#include <map>
#include "../io/GBufferIO.h"

GREFPTR_DEF(GHttpParserStream);

class GHttpParserStream: public GOutputStream
{
public:
	typedef std::function<void(const char* version, int statusCode, const char* statusMessage)> StatusLineCallback;
	typedef std::function<void(const char* key, const char* value)> HeaderCallback;
	typedef std::function<void(const char* key, const char* value)> CookieCallback;
	typedef std::function<void(const void* data, size_t size)> BodyCallback;
	typedef std::map<std::string, std::string> CookiesMap;

	static GHttpParserStreamPtr Create();

	GHttpParserStream();
	~GHttpParserStream();

	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;

	void parseStatusLine(const void* pData, size_t _size);
	void parseHeaderLine(const void* pData, size_t _size);
	void parseContent(const void* pData, size_t _size);
	void parseChunkedLength(const void* pData, size_t _size);
	void parseChunkedContent(const void* pData, size_t _size);

	void parseHeaderValue(const char* key, const char* value);
	void parseContentLength(const char* value);
	void parseTransferEncoding(const char* value);
	void parseContentEncoding(const char* value);
	void parseCookie(const char* value);


	void setStatusLineCallback(const StatusLineCallback& cb);
	void setHeaderCallback(const HeaderCallback& cb);
	void setBodyCallback(const BodyCallback& cb);
	void setCookieCallback(const CookieCallback& cb);
	
	void writeData(const void* pData, size_t _size);
	void writeLine();
	void parseData(const void* pData, size_t _size);
private:
	GObjectClassPooled(GHttpParserStream);
	void setError();
	StatusLineCallback m_statusLineCallback;
	HeaderCallback m_headerCallback;
	BodyCallback m_bodyCallback;
	CookieCallback m_cookieCallback;
	GBufferIO m_bufferIO;
	size_t m_expectIndex;
	int m_state;
	size_t m_contentLength;
	size_t m_nextChunkedLength;
	void* m_inflateStream;
	bool m_hasContentLength;
	bool m_hasChunked;
	bool m_isGZip;
};
#endif