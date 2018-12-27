#include "GGUncompressStream.h"
#include "zlib.h"
#include "zconf.h"



GGUncompressStream::GGUncompressStream(GOutputStreamPtr os)
	:m_pOutputStream(os)
{
	m_pGZipContext = GNewObject<z_stream>();
	z_streamp stream = (z_streamp)m_pGZipContext;
	memset(stream, 0, sizeof(z_stream));
	if (inflateInit2(stream, 47) != Z_OK)
	{
		GDestroyObject(stream);
		m_pGZipContext = nullptr;
	}
}

GGUncompressStream::~GGUncompressStream()
{
	GDestroyObject((z_stream*)m_pGZipContext);
}

void GGUncompressStream::write(const void* pData, size_t _size)
{
	if (_size == 0)
		return;
	z_streamp stream = (z_streamp)m_pGZipContext;

	unsigned char buf[0x4000];
	int result;
	stream->next_in = (Bytef*)pData;
	stream->avail_in = _size;
	do 
	{
		stream->next_out = buf;
		stream->avail_out = sizeof(buf);
		result = inflate(stream, Z_SYNC_FLUSH);
		m_pOutputStream->write(buf, sizeof(buf) - stream->avail_out);
	} while (result == Z_BUF_ERROR || (result != Z_STREAM_END && stream->avail_in != 0));
	if (result == Z_STREAM_END)
		inflateEnd(stream);
}

void GGUncompressStream::flush()
{
	
}
