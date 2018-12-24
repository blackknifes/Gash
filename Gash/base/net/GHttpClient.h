// #ifndef __GHTTPCLIENT_H__
// #define __GHTTPCLIENT_H__
// #include "../io/GIODevice.h"
// #define CURL_STATICLIB 
// #include "curl/curl.h"
// #include <unordered_map>
// #include "../stream/GStream.h"
// 
// GREFPTR_DEF(GHttpClient);
// 
// class GHttpClient: public GIODevice
// {
// public:
// 	typedef std::unordered_map<GString, GString> HeadersMap;
// 
// 	enum Flags
// 	{
// 		AutoReferer,
// 		AcceptEncoding,
// 		TransferEncoding,
// 		FollowLocation,
// 		UnrestrictedAuth,
// 		MaxRedirets,
// 		PostRedirect,
// 		PostFields,
// 		PostFieldSize,
// 		PostFieldSizeLarge,
// 		CopyPostFields,
// 		Referer,
// 		UserAgent,
// 		Cookie,
// 		CookieFile,
// 		CookieJar,
// 		CookieSession,
// 		CookieList,
// 		RequestTarget,
// 		HttpVersion,
// 		IgnoreContentLength,
// 		HttpContentDecoding,
// 		HttpTransferDecoding,
// 		Expect100TimeoutMs,
// 		PipeWait,
// 		StreamDepends,
// 		StreamDependsE,
// 		StreamWeight
// 	};
// 
// 	enum Method
// 	{
// 		Get,
// 		Post
// 	};
// 
// 	GHttpClient();
// 	GHttpClient(const GString& url);
// 	~GHttpClient();
// 
// 	void reset();
// 	void setDebug(bool enabled);
// 	void setUrl(const GString& url);
// 	void setDnsTimeout(int timeout = 5000);
// 	void useGlobalDnsCache(bool enabled);
// 	void setBufferSize(size_t size);
// 	void setPort(int port);
// 	void setTcpNodelay(bool enabled);
// 	void setKeepAlive(bool enabled);
// 	void setKeepIdle(int time);
// 	void setKeepInterval(int time);
// 	void skipCertificationCheck(bool enabled);
// 	void setFollowLocation(bool enabled);
// 	void setMaxRedirect(int numRedirect);
// 	void setMethod(Method method);
// 	void setSessionId(const GString& str);
// 
// 	void setHeader(const GString& key, const GString& value);
// 	bool hasHeader(const GString& key) const;
// 	const GString& getHeader(const GString& key) const;
// 	void setBody(GInputStreamPtr pInput);
// 
// 	bool execute();
// 	bool execute(GOutputStreamPtr pOut);
// 
// 	virtual size_t read(void* pData, size_t bytes) override;
// 	virtual size_t write(const void* pData, size_t bytes) override;
// 	virtual GDataArray readAll() override;
// 	virtual void flush() override;
// 	virtual __int64 seekRead(__int64 offset, SeekType pos = SeekCur) override;
// 	virtual __int64 seekWrite(__int64 offset, SeekType pos = SeekCur) override;
// 	virtual void close() override;
// 	virtual bool isClosed() const override;
// 	virtual int getErrorCode() const override;
// 	virtual GString getErrorMessage() const override;
// 
// 	friend size_t HeaderCallback(char *buffer, size_t size, size_t nitems, void *userdata);
// 
// 	virtual __int64 tellRead() const override;
// 	virtual __int64 tellWrite() const override;
// 
// private:
// 	GObjectClassPooled(GHttpClient);
// 	void init();
// 
// 	size_t onRecvHeader(char *buffer, size_t size, size_t nitems, void *userdata);
// 	CURL* m_pCurl;
// 	GInputStreamPtr m_pBodyStream;
// 	HeadersMap m_headers;
// 	CURLcode m_errorCode;
// 	char m_strErrorBuffer[CURL_ERROR_SIZE];
// };
// #endif