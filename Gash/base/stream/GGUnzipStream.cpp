#include "GGUnzipStream.h"
#include "zlib.h"



GGUnzipStream::GGUnzipStream(GOutputStreamPtr os)
	:m_pOutputStream(os)
{
	m_pGZipContext = GNewObject<z_stream>();
	z_streamp stream = (z_streamp)m_pGZipContext;
	memset(stream, 0, sizeof(z_stream));
	if (inflateInit2(stream, 32) != Z_OK)
	{
		GDestroyObject(stream);
		m_pGZipContext = nullptr;
	}
}

GGUnzipStream::~GGUnzipStream()
{
	GDestroyObject((z_stream*)m_pGZipContext);
}

void GGUnzipStream::write(const void* pData, size_t _size)
{
	z_streamp stream = (z_streamp)m_pGZipContext;

	unsigned char buf[0x4000];
	int result;
	do
	{
		stream->next_in = (Bytef*)pData;
		stream->avail_in = _size;
		stream->next_out = buf;
		stream->avail_out = 0;

		result = inflate(stream, Z_NO_FLUSH);
		m_pOutputStream->write(buf, stream->avail_out);
	} while (result == Z_OK);
	if (result == Z_STREAM_END)
		inflateEnd(stream);
}

void GGUnzipStream::flush()
{
	
}
