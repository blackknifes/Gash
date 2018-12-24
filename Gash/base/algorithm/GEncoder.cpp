// #include "GEncoder.h"
// #include "md5/md5.h"
// #include "libbase64.h"
// #include "../GMemoryPool.h"
// 
// #define ENCODE_UNINIT 0
// #define ENCODE_INITED 1
// #define ENCODE_FINAL 2
// 
// GEncoder::GEncoder(Type type)
// 	:m_pContext(nullptr)
// {
// 	switch (type)
// 	{
// 	case GEncoder::Md5:
// 		m_pContext = PopFromGlobalPool<MD5_CONTEXT>();
// 		MD5Init((MD5_CONTEXT*)m_pContext);
// 		break;
// 	case GEncoder::Base64:
// 		m_pContext = PopFromGlobalPool<base64_state>();
// 		base64_stream_encode_init((base64_state*)m_pContext, 0);
// 		break;
// 	default:
// 		break;
// 	}
// }
// 
// GEncoder::~GEncoder()
// {
// 	switch (m_type)
// 	{
// 	case GEncoder::Md5:
// 		PushToGlobalPool((MD5_CONTEXT*)m_pContext);
// 		break;
// 	case GEncoder::Base64:
// 		PushToGlobalPool((base64_state*)m_pContext);
// 		break;
// 	default:
// 		break;
// 	}
// }
// 
// void GEncoder::encode(const void* pData, size_t bytes, GDataArray& dataArray)
// {
// 	init();
// 	switch (m_type)
// 	{
// 	case GEncoder::Md5:
// 		MD5Update((MD5_CONTEXT*)m_pContext, (unsigned char*)pData, (unsigned int)bytes);
// 		break;
// 	case GEncoder::Base64:
// 	{
// 		const char* pChar = (const char*)pData;
// 		size_t insize = bytes > 1024 ? 1024 : bytes;
// 		base64_stream_encode((base64_state*)m_pContext, pChar, insize, );
// 	}
// 		break;
// 	default:
// 		break;
// 	}
// }
// 
// void GEncoder::end(GDataArray& dataArray)
// {
// 
// }
// 
// void GEncoder::init()
// {
// 	if (m_isInit)
// 		return;
// 	switch (m_type)
// 	{
// 	case GEncoder::Md5:
// 		MD5Init((MD5_CONTEXT*)m_pContext);
// 		break;
// 	case GEncoder::Base64:
// 		base64_stream_encode_init((base64_state*)m_pContext, 0);
// 		break;
// 	default:
// 		break;
// 	}
// }
