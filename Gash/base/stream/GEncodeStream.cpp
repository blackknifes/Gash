#include "GEncodeStream.h"
#include "../algorithm/md5/md5.h"
#include "../GMemoryPool.h"
#include "libbase64.h"

GEncodeStream::GEncodeStream(Type type, GOutputStreamPtr pOut)
	:m_type(type), m_pOutputStream(pOut)
{
	switch (m_type)
	{
	case GEncodeStream::Md5:
		m_pContext = GNewObject<MD5_CONTEXT>();
		MD5Init((MD5_CONTEXT*)m_pContext);
		break;
	case GEncodeStream::Base64:
		m_pContext = GNewObject<base64_state>();
		base64_stream_encode_init((base64_state*)m_pContext, 0);
		break;
	default:
		break;
	}
}

void GEncodeStream::write(const void* pData, size_t _size)
{
	switch (m_type)
	{
	case GEncodeStream::Md5:
		MD5Update((MD5_CONTEXT*)m_pContext, (unsigned char*)pData, (unsigned int)_size);
		break;
	case GEncodeStream::Base64:
	{
		char buf[2048] = {};
		const char* pBytes = (const char*)pData;
		while (_size != 0)
		{
			size_t outsize = 0;
			size_t writeSize = _size > 1024 ? 1024 : _size;
			base64_stream_encode((base64_state*)m_pContext, pBytes, writeSize, buf, &outsize);
			m_pOutputStream->write(buf, outsize);
			_size -= writeSize;
			pBytes += writeSize;
		}
	}
		break;
	default:
		break;
	}
}

void GEncodeStream::flush()
{
	
}

void GEncodeStream::end()
{
	switch (m_type)
	{
	case GEncodeStream::Md5:
		MD5 md5;
		MD5Final(&md5, (MD5_CONTEXT*)m_pContext);

		char buf[40];
		for (size_t i = 0; i < sizeof(md5); ++i)
		{
			size_t offset = i << 1;
			char* ptr = buf + offset;
			sprintf_s(ptr, 40 - offset, "%02X", md5.md5[i]);
		}

		m_pOutputStream->write(buf, 32);
		MD5Init((MD5_CONTEXT*)m_pContext);
		break;
	case GEncodeStream::Base64:
	{
		char buf[1024];
		size_t outlen = 0;
		base64_stream_encode_final((base64_state*)m_pContext, buf, &outlen);
		if (outlen != 0)
			m_pOutputStream->write(buf, outlen);
		base64_stream_encode_init((base64_state*)m_pContext, 0);
	}
	break;
	default:
		break;
	}
}