#ifndef __GNEWHANDLER_H__
#define __GNEWHANDLER_H__
#include "../GSingleton.h"
#include <Windows.h>
#include <vector>

class GNewHandler: public GSingleton<GNewHandler>
{
public:
	typedef void(*Callback)(void* pUserData);
	struct Handler
	{
		void* pUserData;
		Callback callback;
	};

	GNewHandler();
	~GNewHandler();

	void addHandler(Callback callback, void* pUserData = nullptr);
	void removeHandler(Callback callback, void* pUserData = nullptr);
	void clearHandler();
private:
	std::vector<Handler> m_handlerList;
	CRITICAL_SECTION m_section;

	static int Handle(size_t size);
};

void GAddNewHandler(GNewHandler::Callback callback, void* pUserData = nullptr);
void GRemoveNewHandler(GNewHandler::Callback callback, void* pUserData = nullptr);
#endif