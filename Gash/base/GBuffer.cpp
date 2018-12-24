#include "GBuffer.h"



GBufferPtr GBuffer::CreateBuffer(size_t size)
{
	unsigned char* pData = (unsigned char*)malloc(size);
	if (pData == nullptr)
		return nullptr;
	GBufferPtr pBuffer = CreatePooled();
	pBuffer->m_pData = pData;
	pBuffer->m_size = size;
	return pBuffer;
}

const unsigned char* GBuffer::data() const
{
	return m_pData;
}

unsigned char* GBuffer::data()
{
	return m_pData;
}

size_t GBuffer::size() const
{
	return m_size;
}

GBuffer::GBuffer()
{

}

GBuffer::~GBuffer()
{
	if(m_pData)
		free(m_pData);
}
