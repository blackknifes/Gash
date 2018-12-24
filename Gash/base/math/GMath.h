#ifndef __GMATH_H__
#define __GMATH_H__

namespace GMath
{
	template<typename _Ty, typename _Ty2>
	_Ty Align(_Ty val, _Ty2 alignSize)
	{
		return ((val + (_Ty)alignSize - 1) & ~((_Ty)alignSize - 1));
	}

	template<typename _Ty, typename _Ty2, typename... _argsTy>
	_Ty Max(_Ty val, _Ty2 val2, _argsTy... args)
	{
		return Max(val > val2 ? val : val2, args...);
	}

	template<typename _Ty, typename _Ty2>
	_Ty Max(_Ty val, _Ty2 val2)
	{
		return val > val2 ? val : val2;
	}

	template<typename _Ty, typename _Ty2, typename... _argsTy>
	_Ty Min(_Ty val, _Ty2 val2, _argsTy... args)
	{
		return Min(val < val2 ? val : val2, args...);
	}

	template<typename _Ty, typename _Ty2>
	_Ty Min(_Ty val, _Ty2 val2)
	{
		return val < val2 ? val : val2;
	}
};
#endif