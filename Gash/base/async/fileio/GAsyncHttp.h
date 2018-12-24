// #ifndef __GASYNCHTTP_H__
// #define __GASYNCHTTP_H__
// #include "GAsyncTCPSocket.h"
// #include "../../GDataArray.h"
// #include "../../stream/GBufferStream.h"
// #include <map>
// 
// class GAsyncHttp: public GAsyncTCPSocket
// {
// public:
// 	typedef std::map<std::string, std::string> HeaderMap;
// 
// private:
// 	
// protected:
// 	virtual size_t onRead(const void* pData, size_t size) override;
// 	virtual bool readStatusLine();
// 	virtual bool readHeader();
// 	virtual bool readContent(const void* pData, size_t size);
// 	virtual bool readChunkedLength();
// 	virtual bool readChunkedData(const void* pData, size_t size);
// 	void setState(int state);
// 	int m_recvStatus;
// 	int m_statusCode;
// 	bool m_isChunked;
// 	std::string m_statusMessage;
// 	std::string m_body;
// 	HeaderMap m_headers;
// 	size_t m_nextChunkedLength;
// 	size_t m_contentLength;
// 	GBufferStream m_bufferStream;
// };
// #endif