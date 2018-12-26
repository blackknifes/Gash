#include "GAsyncIO.h"
#include "../platform/GEventPool.h"

static std::atomic<size_t> s_ioCount = 0;

GAsyncIOPtr GAsyncIO::GetAsyncIO(void* overlapped)
{
	return CONTAINING_RECORD(overlapped, GAsyncIO, m_overlapped);
}

GAsyncIOPtr GAsyncIO::CreateIO(GIODevicePtr pDevice, IOType type, const Callback& compCall /*= nullptr*/)
{
	return GAsyncIO::CreatePooled(pDevice, type, compCall);
}

GAsyncIO::GAsyncIO(GIODevicePtr pDevice, IOType type, const Callback& compCall /*= nullptr*/)
	:m_type(type), 
	m_dwTransferSize(0),
	m_pIODevice(pDevice),
	m_pBufferData(nullptr), m_bufferSize(0),
	m_state(IO_Pending),
	m_callback(compCall)
{
	memset(&m_overlapped, 0, sizeof(m_overlapped));
	++s_ioCount;
}

GAsyncIO::~GAsyncIO()
{
	freeBuffer();
	clearEvent();
	--s_ioCount;
}

LPOVERLAPPED GAsyncIO::getOverlapped()
{
	return &m_overlapped;
}

DWORD GAsyncIO::getSize() const
{
	return m_dwTransferSize;
}

void GAsyncIO::allocBuffer(size_t size)
{
	if (size == m_bufferSize)
		return;
	if (size > GBufferIO::buffer_size)
	{
		freeBuffer();
		m_pBufferData = malloc(size);
	}
	else
	{
		if (m_bufferSize <= GBufferIO::buffer_size)
			return;
		m_pBufferData = GBufferIO::AllocBuffer();
	}
	m_bufferSize = size;
}

void GAsyncIO::freeBuffer()
{
	if (m_pBufferData)
	{
		if (m_bufferSize > GBufferIO::buffer_size)
			free(m_pBufferData);
		else
			GBufferIO::DeallocBuffer(m_pBufferData);
		m_pBufferData = nullptr;
		m_bufferSize = 0;
	}
}

GBufferIOPtr GAsyncIO::getBuffer() const
{
	return m_bufferIO;
}

const void* GAsyncIO::getBufferData() const
{
	return m_pBufferData;
}

void* GAsyncIO::getBufferData()
{
	return m_pBufferData;
}

GIODevicePtr GAsyncIO::getDevice() const
{
	return m_pIODevice;
}

void GAsyncIO::setState(State state)
{
	m_state = state;
}

GAsyncIO::State GAsyncIO::getState() const
{
	return m_state;
}

void GAsyncIO::setupEvent()
{
	if (!m_event)
	{
		m_event = GEventPool::PopEvent();
		m_overlapped.hEvent = m_event->getNativeHandle();
	}
}

bool GAsyncIO::waitEvent(DWORD timeout)
{
	if (m_event)
		return m_event->wait(timeout);
	return false;
}

void GAsyncIO::clearEvent()
{
	if (!m_event)
		return;
	GEventPool::PushEvent(m_event);
	m_event = nullptr;
	m_overlapped.hEvent = nullptr;
}

void GAsyncIO::OnComplete(LPOVERLAPPED pOverlapped, DWORD dwTransferSize, ULONG_PTR compKey)
{
	GAsyncIOPtr pAsyncIO = GAsyncIO::GetAsyncIO(pOverlapped);
	pAsyncIO->Release();
	if (pAsyncIO->m_state == IO_End)
		return;

	pAsyncIO->m_dwTransferSize += dwTransferSize;

	switch (pAsyncIO->m_type)
	{
	case GAsyncIO::IO_Read:
		//¶Á²Ù×÷£¬Ð´Èë»º³å
		pAsyncIO->m_bufferIO->writeSync(pAsyncIO->m_pBufferData, dwTransferSize);
		break;
	default:
		break;
	}
	if(pAsyncIO->m_callback)
		pAsyncIO->m_callback(pAsyncIO);
}

bool GAsyncIO::HasPending()
{
	return s_ioCount == 0;
}
