#include "GNetEngine.h"



GNetEngine::GNetEngine()
	:m_major(2), m_minor(2)
{
	while (WSAStartup(MAKEWORD(m_major, m_minor), &m_wsadata) != 0)
	{
		if (--m_minor < 0)
		{
			if (--m_major < 0)
				break;
			m_minor = 2;
		}
	}
}

GNetEngine::~GNetEngine()
{
	WSACleanup();
}

bool GNetEngine::init()
{
	return GNetEngine::getInstance()->m_major > 0;
}

int GNetEngine::getMajor() const
{
	return m_major;
}

int GNetEngine::getMinor() const
{
	return m_minor;
}
