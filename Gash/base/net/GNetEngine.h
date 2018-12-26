#ifndef __GNETENGINE_H__
#define __GNETENGINE_H__
#include "../GSingleton.h"
#include <WinSock2.h>

class GNetEngine: public GSingleton<GNetEngine>
{
public:
	GNetEngine();
	~GNetEngine();

	static bool init();
	int getMajor() const;
	int getMinor() const;
private:
	WSADATA m_wsadata;
	int m_major;
	int m_minor;
};
#endif