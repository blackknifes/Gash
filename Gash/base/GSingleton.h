#ifndef __GSINGLETON_H__
#define __GSINGLETON_H__

template<typename _Ty>
class GSingleton
{
public:
	static _Ty* getInstance()
	{
		if (s_pInstance == nullptr)
			s_pInstance = new _Ty();
		return s_pInstance;
	}

	static void destroyInstance()
	{
		if (s_pInstance)
		{
			delete s_pInstance;
			s_pInstance = nullptr;
		}
	}
private:
	static _Ty* s_pInstance;
};

template<typename _Ty>
__declspec(selectany) _Ty* GSingleton<_Ty>::s_pInstance = nullptr;

#endif