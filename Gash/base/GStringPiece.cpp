#include "GStringPiece.h"
#include "GString.h"

typedef GString::const_iterator const_iterator;
static int CompareString(const_iterator lbegin, const_iterator lend, const_iterator rbegin, const_iterator rend, bool caseSentive = true)
{
	if (caseSentive)
	{
		while (lbegin != lend && rbegin != rend && *lbegin == *rbegin)
		{
			++lbegin;
			++rbegin;
		}
		if (*lbegin > *rbegin)
			return 1;
		else if (*lbegin < *rbegin)
			return -1;
		return 0;
	}
	while (lbegin != lend && rbegin != rend && ::tolower(*lbegin) == ::tolower(*rbegin))
	{
		++lbegin;
		++rbegin;
	}
	if (::tolower(*lbegin) > ::tolower(*rbegin))
		return 1;
	else if (::tolower(*lbegin) < ::tolower(*rbegin))
		return -1;
	return 0;
}

static int CompareString(const_iterator lbegin, const_iterator lend, const wchar_t* rbegin, const wchar_t* rend, bool caseSentive = true)
{
	if (caseSentive)
	{
		while (lbegin != lend && rbegin != rend && *lbegin == *rbegin)
		{
			++lbegin;
			++rbegin;
		}
		if (*lbegin > *rbegin)
			return 1;
		else if (*lbegin < *rbegin)
			return -1;
		return 0;
	}
	while (lbegin != lend && rbegin != rend && ::tolower(*lbegin) == ::tolower(*rbegin))
	{
		++lbegin;
		++rbegin;
	}
	if (::tolower(*lbegin) > ::tolower(*rbegin))
		return 1;
	else if (::tolower(*lbegin) < ::tolower(*rbegin))
		return -1;
	return 0;
}

GStringPiece::GStringPiece()
{

}

GStringPiece::GStringPiece(const GString& str)
	:m_begin(str.begin()), m_end(str.end())
{

}

GStringPiece::GStringPiece(const GString& str, size_t size)
	: m_begin(str.begin()), m_end(str.begin() + size)
{

}

GStringPiece::GStringPiece(const GString& str, size_t _start, size_t _end)
	: m_begin(str.begin() + _start), m_end(str.begin() + _end)
{

}

GStringPiece::GStringPiece(const std::wstring& str)
	: m_begin(str.begin()), m_end(str.end())
{

}

GStringPiece::GStringPiece(const std::wstring& str, size_t size)
	: m_begin(str.begin()), m_end(str.begin() + size)
{

}

GStringPiece::GStringPiece(const std::wstring& str, size_t _start, size_t _end)
	: m_begin(str.begin() + _start), m_end(str.begin() + _end)
{

}

GStringPiece::GStringPiece(const_iterator _begin, const_iterator _end)
	: m_begin(_begin), m_end(_end)
{

}

GStringPiece::const_iterator GStringPiece::begin() const
{
	return m_begin;
}

GStringPiece::const_iterator GStringPiece::end() const
{
	return m_end;
}

size_t GStringPiece::size() const
{
	return m_end - m_begin;
}

bool GStringPiece::empty() const
{
	return size() == 0;
}

bool GStringPiece::isEmptyOrBlank() const
{
	if (empty())
		return true;
	for (auto itor = m_begin; itor != m_end; ++itor)
	{
		if (!::isblank(*itor))
			return false;
	}
	return true;
}

bool GStringPiece::operator>(const GStringPiece& str) const
{
	return CompareString(m_begin, m_end, str.m_begin, str.m_end) > 0;
}

bool GStringPiece::operator>=(const GStringPiece& str) const
{
	return CompareString(m_begin, m_end, str.m_begin, str.m_end) >= 0;
}

bool GStringPiece::operator<(const GStringPiece& str) const
{
	return CompareString(m_begin, m_end, str.m_begin, str.m_end) < 0;
}

bool GStringPiece::operator<=(const GStringPiece& str) const
{
	return CompareString(m_begin, m_end, str.m_begin, str.m_end) <+ 0;
}

GString GStringPiece::operator+(const GStringPiece& str) const
{
	GString tmp = toString();
	tmp.append(str.begin(), str.end());
	return std::move(tmp);
}

wchar_t GStringPiece::operator[](size_t index) const
{
	return *(m_begin + index);
}

wchar_t GStringPiece::at(size_t index) const
{
	return *(m_begin + index);
}

int GStringPiece::compare(const GStringPiece& str, bool caseSentive /*= true*/) const
{
	return CompareString(m_begin, m_end, str.begin(), str.end(), caseSentive);
}

bool GStringPiece::compareWith(const GStringPiece& str, bool caseSentive /*= true*/) const
{
	return CompareString(m_begin, m_end, str.begin(), str.end(), caseSentive) == 0;
}

bool GStringPiece::startsWith(const GStringPiece& str, bool caseSentive /*= true*/) const
{
	size_t len = str.size();
	if (this->size() < len)
		return false;
	if (empty() || str.empty())
		return true;
	return CompareString(m_begin, m_begin + len - 1, str.begin(), str.end() - 1, caseSentive) == 0;
}

bool GStringPiece::endsWith(const GStringPiece& str, bool caseSentive /*= true*/) const
{
	size_t len = str.size();
	if (this->size() < len)
		return false;
	if (empty() || str.empty())
		return true;
	return CompareString(m_end - len, m_end - 1, str.begin(), str.end() - 1, caseSentive) == 0;
}

bool GStringPiece::endsWithSeperator() const
{
	if (empty())
		return false;
	return *(m_end - 1) == '/' || *(m_end - 1) == '\\';
}

bool GStringPiece::startsWithSeperator() const
{
	if (empty())
		return false;
	return *m_begin == '/' || *m_begin == '\\';
}

bool GStringPiece::isUpper() const
{
	for (auto& elem: *this)
	{
		if (!::isupper(elem))
			return false;
	}
	return true;
}

bool GStringPiece::isLower() const
{
	for (auto& elem : *this)
	{
		if (!::islower(elem))
			return false;
	}
	return true;
}

bool GStringPiece::isNumber() const
{
	for (auto& elem : *this)
	{
		if (!::isdigit(elem))
			return false;
	}
	return true;
}

bool GStringPiece::toBool() const
{
	if (size() != 4)
		return false;
	return ::tolower(*m_begin) == 't' && ::tolower(*(m_begin + 1)) == 'r' && 
		::tolower(*(m_begin + 2)) == 'u' && ::tolower(*(m_begin + 3)) == 'e';
}

int GStringPiece::toInt() const
{
	wchar_t buf[32] = {};
	std::copy(m_begin, size() > 32? m_begin + 30 : m_end, buf);
	return _wtoi(buf);
}

unsigned int GStringPiece::toUInt() const
{
	wchar_t buf[32] = {};
	std::copy(m_begin, size() > 32 ? m_begin + 30 : m_end, buf);
	return (unsigned int)_wtoi(buf);
}

unsigned int GStringPiece::toHex() const
{
	wchar_t buf[32] = {};
	std::copy(m_begin, size() > 32 ? m_begin + 30 : m_end, buf);
	wchar_t* ptr;
	return wcstol(buf, &ptr, 16);
}

__int64 GStringPiece::toInt64() const
{
	wchar_t buf[64] = {};
	std::copy(m_begin, size() > 64 ? m_begin + 62 : m_end, buf);
	return _wtoll(buf);
}

unsigned __int64 GStringPiece::toUInt64() const
{
	wchar_t buf[64] = {};
	std::copy(m_begin, size() > 64 ? m_begin + 62 : m_end, buf);
	return (unsigned __int64)_wtoll(buf);
}

double GStringPiece::toDouble() const
{
	wchar_t buf[64] = {};
	std::copy(m_begin, size() > 64 ? m_begin + 62 : m_end, buf);
	return _wtof(buf);
}

GStringPiece GStringPiece::substr(size_t offset, size_t count /*= npos*/) const
{
	size_t srclen = this->size();
	if (offset >= srclen)
		offset = srclen - 1;
	if (count + offset > srclen)
		count = srclen - offset;
	return GStringPiece(m_begin + offset, m_begin + offset + count);
}

GStringPiece GStringPiece::slice(size_t _start, size_t _end /*= npos*/) const
{
	return substr(_start, _end - _start);
}

size_t GStringPiece::find(const GStringPiece& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	size_t len = str.size();
	size_t srclen = this->size();
	if (srclen < len)
		return npos;
	for (size_t i = 0; i <= srclen - len; ++i)
	{
		if (CompareString(
			this->begin() + i, this->begin() + i + len - 1,
			str.begin(), str.begin() + len - 1,
			caseSentive) == 0)
			return i;
	}
	return -1;
}

size_t GStringPiece::find(const wchar_t* str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	size_t len = wcslen(str);
	size_t srclen = this->size();
	if (srclen < len)
		return npos;
	for (size_t i = 0; i <= srclen - len; ++i)
	{
		if (CompareString(
			this->begin() + i, this->begin() + i + len - 1,
			str, str + len - 1,
			caseSentive) == 0)
			return i;
	}
	return -1;
}

size_t GStringPiece::findFirst(const GStringPiece& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	return find(str, offset, caseSentive);
}

size_t GStringPiece::findFirst(const wchar_t* str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	return find(str, offset, caseSentive);
}

size_t GStringPiece::findFirstNot(const GStringPiece& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	size_t len = str.size();
	size_t srclen = this->size();
	if (srclen < len)
		return npos;
	for (size_t i = 0; i <= srclen - len; ++i)
	{
		if (CompareString(
			this->begin() + i, this->begin() + i + len - 1,
			str.begin(), str.begin() + len - 1,
			caseSentive) != 0)
			return i;
	}
	return -1;
}

size_t GStringPiece::findFirstNot(const wchar_t* str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	size_t len = wcslen(str);
	size_t srclen = this->size();
	if (srclen < len)
		return npos;
	for (size_t i = 0; i <= srclen - len; ++i)
	{
		if (CompareString(
			this->begin() + i, this->begin() + i + len - 1,
			str, str + len - 1,
			caseSentive) != 0)
			return i;
	}
	return -1;
}

size_t GStringPiece::findLast(const GStringPiece& str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	size_t len = str.size();
	size_t srclen = this->size();
	if (srclen < len)
		return npos;

	if (offset >= srclen)
		offset = srclen - len;
	for (size_t i = offset; i != npos; --i)
	{
		if (CompareString(
			this->begin() + i, this->begin() + i + len - 1,
			str.begin(), str.begin() + len - 1,
			caseSentive) == 0)
			return i;
	}
	return -1;
}

size_t GStringPiece::findLast(const wchar_t* str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	size_t len = wcslen(str);
	size_t srclen = this->size();
	if (srclen < len)
		return npos;

	if (offset >= srclen)
		offset = srclen - len;
	for (size_t i = offset; i != npos; --i)
	{
		if (CompareString(
			this->begin() + i, this->begin() + i + len - 1,
			str, str + len - 1,
			caseSentive) == 0)
			return i;
	}
	return -1;
}

size_t GStringPiece::findLastNot(const GStringPiece& str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	size_t len = str.size();
	size_t srclen = this->size();
	if (srclen < len)
		return npos;

	if (offset >= srclen)
		offset = srclen - len;
	for (size_t i = offset; i != npos; --i)
	{
		if (CompareString(
			this->begin() + i, this->begin() + i + len - 1,
			str.begin(), str.begin() + len - 1,
			caseSentive) != 0)
			return i;
	}
	return -1;
}

size_t GStringPiece::findLastNot(const wchar_t* str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	size_t len = wcslen(str);
	size_t srclen = this->size();
	if (srclen < len)
		return npos;

	if (offset >= srclen)
		offset = srclen - len;
	for (size_t i = offset; i != npos; --i)
	{
		if (CompareString(
			this->begin() + i, this->begin() + i + len - 1,
			str, str + len - 1,
			caseSentive) != 0)
			return i;
	}
	return -1;
}

GString GStringPiece::toString() const
{
	return GString(m_begin, m_end);
}

size_t GStringPiece::read(wchar_t* pOut, size_t readsize) const
{
	size_t srclen = this->size();
	if (srclen < readsize)
		readsize = srclen;
	std::copy(begin(), begin() + srclen, pOut);
	return readsize;
}

bool GStringPiece::operator!=(const GStringPiece& str) const
{
	return CompareString(m_begin, m_end, str.m_begin, str.m_end) != 0;
}

bool GStringPiece::operator==(const GStringPiece& str) const
{
	return CompareString(m_begin, m_end, str.m_begin, str.m_end) == 0;
}
