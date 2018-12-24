#ifndef __GSTRINGS_H__
#define __GSTRINGS_H__
#include <string>

namespace GStrings
{
	static size_t Strlen(const char* str)
	{
		return strlen(str);
	}
	static size_t Strlen(const wchar_t* str)
	{
		return wcslen(str);
	}

	template<typename _Iter1, typename _Iter2>
	static bool CompareString(_Iter1 _begin, _Iter1 _end, _Iter2 _begin2, _Iter2 _end2, bool caseSentive = true)
	{
		if (caseSentive)
		{
			while (_begin != _end && _begin2 != _end2)
			{
				if (*_begin++ != *_begin2++)
					return false;
			}
			return true;
		}

		while (_begin != _end && _begin2 != _end2)
		{
			if (::tolower(*_begin++) != ::tolower(*_begin2++))
				return false;
		}
		return true;
	}

	template<typename _elemTy>
	bool StartsWith(const std::basic_string<_elemTy>& str, const _elemTy* starts, bool caseSentive = true)
	{
		size_t _size = Strlen(starts);
		if (str.size() < _size)
			return false;
		return CompareString(str.begin(), str.begin() + _size, starts, starts + _size, caseSentive);
	}

	template<typename _elemTy>
	bool EndsWith(const std::basic_string<_elemTy>& str, const _elemTy* ends, bool caseSentive = true)
	{
		size_t _size = Strlen(ends);
		if (str.size() < _size)
			return false;
		return CompareString(str.end() - _size, str.end(), ends, ends + _size, caseSentive);
	}

	template<typename _elemTy>
	std::basic_string<_elemTy>& AddStartsWith(std::basic_string<_elemTy>& str, const _elemTy* starts, bool caseSentive = true)
	{
		if (!StartsWith(str, starts, caseSentive))
			str.insert(0, starts);
		return str;
	}

	template<typename _elemTy>
	std::basic_string<_elemTy>& RemoveStartsWith(std::basic_string<_elemTy>& str, const _elemTy* starts, bool caseSentive = true)
	{
		if (StartsWith(str, starts, caseSentive))
			str.erase(0, Strlen(starts));
		return str;
	}

	template<typename _elemTy>
	std::basic_string<_elemTy>& AddEndsWith(std::basic_string<_elemTy>& str, const _elemTy* ends, bool caseSentive = true)
	{
		if (!EndsWith(str, ends, caseSentive))
			str.append(ends);
		return str;
	}

	template<typename _elemTy>
	std::basic_string<_elemTy>& RemoveEndsWith(std::basic_string<_elemTy>& str, const _elemTy* ends, bool caseSentive = true)
	{
		size_t _size = Strlen(ends);
		if (EndsWith(str, ends, caseSentive))
			str.erase(str.size() - _size, _size);
		return str;
	}

	template<typename _elemTy>
	std::basic_string<_elemTy>& ToLower(std::basic_string<_elemTy>& str)
	{
		for (auto itor = str.begin(); itor != str.end(); ++itor)
			*itor = ::tolower(*itor);
		return str;
	}

	template<typename _elemTy>
	std::basic_string<_elemTy>& ToUpper(std::basic_string<_elemTy>& str)
	{
		for (auto itor = str.begin(); itor != str.end(); ++itor)
			*itor = ::toupper(*itor);
		return str;
	}
};
#endif