#include "GStringPiece.h"
#include "GString.h"

inline static bool CompareWCharWith(wchar_t ch1, wchar_t ch2)
{
	return ch1 == ch2;
}

inline static bool CompareWCharWithIgnoreCase(wchar_t ch1, wchar_t ch2)
{
	return ::tolower(ch1) == ::tolower(ch2);
}

inline static int CompareWChar(wchar_t ch1, wchar_t ch2)
{
	if (ch1 > ch2)
		return 1;
	else if (ch1 < ch2)
		return -1;
	return 0;
}

inline static int CompareWCharIgnoreCase(wchar_t ch1, wchar_t ch2)
{
	ch1 = ::tolower(ch1);
	ch2 = ::tolower(ch2);
	if (ch1 > ch2)
		return 1;
	else if (ch1 < ch2)
		return -1;
	return 0;
}

GStringPiece GStringPiece::From(const GString& str)
{
	return GStringPiece(str.begin(), str.end());
}

GStringPiece GStringPiece::From(const GString& str, size_t size)
{
	return GStringPiece(str, size);
}

GStringPiece GStringPiece::From(const GString& str, size_t _start, size_t _end)
{
	return GStringPiece(str, _start, _end);
}

GStringPiece GStringPiece::From(const std::wstring& str)
{
	return GStringPiece(str.begin(), str.end());
}

GStringPiece GStringPiece::From(const std::wstring& str, size_t size)
{
	return GStringPiece(str, size);
}

GStringPiece GStringPiece::From(const std::wstring& str, size_t _start, size_t _end)
{
	return GStringPiece(str, _start, _end);
}

GStringPiece GStringPiece::From(const wchar_t* str)
{
	return GStringPiece(str);
}

GStringPiece GStringPiece::From(const wchar_t* _start, const wchar_t* _end)
{
	return GStringPiece(_start, _end);
}

GStringPiece GStringPiece::From(const wchar_t* str, size_t size)
{
	return GStringPiece(str, str + size);
}

GStringPiece GStringPiece::From(const wchar_t* str, size_t _start, size_t _end)
{
	return GStringPiece(str + _start, str + _end);
}

GStringPiece::GStringPiece()
{

}

GStringPiece::GStringPiece(const wchar_t* str)
{
	const wchar_t* _begin = str;
	const wchar_t* _end = str + wcslen(str);
	m_begin = GStringPieceIterator(_begin, _end);
	m_end = GStringPieceIterator(_begin, _end, _end);
}

GStringPiece::GStringPiece(const wchar_t* _begin, const wchar_t* _end)
{
	m_begin = GStringPieceIterator(_begin, _end);
	m_end = GStringPieceIterator(_begin, _end, _end);
}

GStringPiece::GStringPiece(const wchar_t* _begin, size_t size)
{
	const wchar_t* _end = _begin + size;
	m_begin = GStringPieceIterator(_begin, _end);
	m_end = GStringPieceIterator(_begin, _end, _end);
}

GStringPiece::GStringPiece(const GStringPieceIterator& _begin, const GStringPieceIterator& _end)
	:m_begin(_begin), m_end(_end)
{

}

GStringPiece::GStringPiece(const GString& str)
	:m_begin(str.begin()), m_end(str.end())
{

}

GStringPiece::GStringPiece(const GString& str, size_t size)
{
	m_begin = str.begin();
	m_end = str.begin() + size;
}

GStringPiece::GStringPiece(const GString& str, size_t _start, size_t _end)
{
	m_begin = str.begin() + _start;
	m_end = str.begin() + _end;
}

GStringPiece::GStringPiece(const std::wstring& str)
{
	m_begin = str.begin();
	m_end = str.end();
}

GStringPiece::GStringPiece(const std::wstring& str, size_t size)
{
	m_begin = str.begin();
	m_end = str.begin() + size;
}

GStringPiece::GStringPiece(const std::wstring& str, size_t _start, size_t _end)
{
	m_begin = str.begin() + _start;
	m_end = str.begin() + _end;
}

GStringPiece::GStringPiece(const_iterator _begin, const_iterator _end)
	:m_begin(_begin), m_end(_end)
{

}

GStringPieceIterator GStringPiece::begin() const
{
	return m_begin;
}

GStringPieceIterator GStringPiece::end() const
{
	return m_end;
}

size_t GStringPiece::size() const
{
	return this->m_end - this->m_begin;
}

bool GStringPiece::empty() const
{
	return this->size() == 0;
}

bool GStringPiece::isEmptyOrBlank() const
{
	if (this->empty())
		return true;
	for (auto itor = this->begin(); itor != this->end(); ++itor)
	{
		if (!::isblank(*itor))
			return false;
	}
	return true;
}

bool GStringPiece::operator==(const GStringPiece& str) const
{
	return this->compare(str) == 0;
}

bool GStringPiece::operator!=(const GStringPiece& str) const
{
	return this->compare(str) != 0;
}

bool GStringPiece::operator>(const GStringPiece& str) const
{
	return this->compare(str) > 0;
}

bool GStringPiece::operator>=(const GStringPiece& str) const
{
	return this->compare(str) >= 0;
}

bool GStringPiece::operator<(const GStringPiece& str) const
{
	return this->compare(str) < 0;
}

bool GStringPiece::operator<=(const GStringPiece& str) const
{
	return this->compare(str) <= 0;
}

GString GStringPiece::operator+(const GStringPiece& str) const
{
	return this->toString().append(str);
}

wchar_t GStringPiece::operator[](size_t index) const
{
	return *(this->m_begin + index);
}

wchar_t GStringPiece::at(size_t index) const
{
	return *(this->m_begin + index);
}

int GStringPiece::compare(const GStringPiece& str, bool caseSentive /*= true*/) const
{
	auto itor1 = this->begin();
	auto itor2 = str.begin();

	auto cmpWith = caseSentive ? CompareWCharWith : CompareWCharWithIgnoreCase;
	auto cmp = caseSentive ? CompareWChar : CompareWCharIgnoreCase;

	while (itor1 != this->end() && itor2 != str.end() && cmpWith(*itor1, *itor2))
	{
		++itor1;
		++itor2;
	}

	if (itor1 == this->end())
	{
		if (itor2 == str.end())
			return 0;
		else
			return -1;
	}
	else if (itor2 == str.end())
		return 1;

	return cmp(*itor1, *itor2);
}

bool GStringPiece::compareWith(const GStringPiece& str, bool caseSentive /*= true*/) const
{
	auto itor1 = this->begin();
	auto itor2 = str.begin();

	auto cmpWith = caseSentive ? CompareWCharWith : CompareWCharWithIgnoreCase;

	while (itor1 != this->end() && itor2 != str.end() && cmpWith(*itor1, *itor2))
	{
		++itor1;
		++itor2;
	}

	return itor2 == str.end();
}

bool GStringPiece::startsWith(const GStringPiece& str, bool caseSentive /*= true*/) const
{
	return this->substr(0, str.size()).compareWith(str, caseSentive);
}

bool GStringPiece::endsWith(const GStringPiece& str, bool caseSentive /*= true*/) const
{
	return this->substr(this->size() - str.size(), str.size()).compareWith(str, caseSentive);
}

bool GStringPiece::endsWithSeperator() const
{
	return *m_end == '/' || *m_end == '\\';
}

bool GStringPiece::startsWithSeperator() const
{
	return *m_begin == '/' || *m_begin == '\\';
}

GString GStringPiece::addStartsWith(const GStringPiece& str, bool caseSentive /*= true*/) const
{
	if (startsWith(str, caseSentive))
		return toString();
	return str.toString() + toString();
}

GString GStringPiece::addEndsWith(const GStringPiece& str, bool caseSentive /*= true*/) const
{
	if (endsWith(str, caseSentive))
		return toString();
	return toString() + str.toString();
}

GString GStringPiece::addEndsWithSeperator() const
{
	if (endsWithSeperator())
		return toString();
	return toString() + L"/";
}

GString GStringPiece::addStartsWithSeperator() const
{
	if (endsWithSeperator())
		return toString();
	return GString(L"/") + toString();
}

bool GStringPiece::isUpper() const
{
	for (auto itor = begin(); itor != end(); ++itor)
	{
		if (!::isupper(*itor))
			return false;
	}
	return true;
}

bool GStringPiece::isLower() const
{
	for (auto itor = begin(); itor != end(); ++itor)
	{
		if (!::islower(*itor))
			return false;
	}
	return true;
}

bool GStringPiece::isNumber() const
{
	bool dot = true;
	for (auto itor = begin(); itor != end(); ++itor)
	{
		if (!::isdigit(*itor))
		{
			if (*itor == '.')
			{
				if (dot)
					return false;
				dot = true;
				continue;
			}
			return false;
		}
	}
	return true;
}

bool GStringPiece::isBool() const
{
	return compareWith(L"true", false) || compareWith(L"false", false);
}

GString GStringPiece::toUpper() const
{
	return toString().toUpper();
}

GString GStringPiece::toLower() const
{
	return toString().toLower();
}

bool GStringPiece::toBool() const
{
	return !compareWith(L"false", false);
}

int GStringPiece::toInt() const
{
	wchar_t buf[256];
	copy(buf, sizeof(buf) - 1);
	buf[255] = 0;
	wchar_t* ptr;
	return wcstol(buf, &ptr, 10);
}

unsigned int GStringPiece::toUInt() const
{
	wchar_t buf[256];
	copy(buf, sizeof(buf) - 1);
	buf[255] = 0;
	wchar_t* ptr;
	return wcstoul(buf, &ptr, 10);
}

unsigned int GStringPiece::toHex() const
{
	wchar_t buf[256];
	copy(buf, sizeof(buf) - 1);
	buf[255] = 0;
	wchar_t* ptr;
	return wcstoul(buf, &ptr, 16);
}

__int64 GStringPiece::toInt64() const
{
	wchar_t buf[256];
	copy(buf, sizeof(buf) - 1);
	buf[255] = 0;
	wchar_t* ptr;
	return wcstoll(buf, &ptr, 10);
}

unsigned __int64 GStringPiece::toUInt64() const
{
	wchar_t buf[256];
	copy(buf, sizeof(buf) - 1);
	buf[255] = 0;
	wchar_t* ptr;
	return wcstoull(buf, &ptr, 10);
}

double GStringPiece::toDouble() const
{
	wchar_t buf[256];
	copy(buf, sizeof(buf) - 1);
	buf[255] = 0;
	wchar_t* ptr;
	return wcstof(buf, &ptr);
}

GStringPiece GStringPiece::substr(size_t offset, size_t count /*= npos*/) const
{
	size_t _size = size();
	if (offset > _size)
		return GStringPiece();
	if (count + offset > _size)
		count = _size - offset;
	return GStringPiece(m_begin + offset, m_begin + offset + count);
}

GStringPiece GStringPiece::slice(size_t _start, size_t _end /*= npos*/) const
{
	return substr(_start, _end - _start);
}

size_t GStringPiece::copy(wchar_t* buffer, size_t bufsize) const
{
	size_t readsize = 0;
	for (auto itor = begin(); itor != end() && readsize < bufsize; ++itor)
	{
		buffer[readsize++] = *itor;
	}
	if (bufsize > readsize)
		buffer[readsize] = 0;
	return readsize;
}

size_t GStringPiece::insert(GString& str, size_t offset, size_t count /*= npos*/) const
{
	if (count == npos)
		count = this->size();
	str.insert(offset, count, 0);
	for (size_t i = 0; i < count; ++i)
		str[i + offset] = at(i);
	return count;
}

size_t GStringPiece::append(GString& str, size_t count /*= npos*/) const
{
	size_t offset = str.size();
	str.append(count, 0);
	for (size_t i = 0; i < count; ++i)
		str[i + offset] = at(i);
	return count;
}

size_t GStringPiece::find(const GStringPiece& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	return findFirst(str, offset, caseSentive);
}

size_t GStringPiece::findFirst(const GStringPiece& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	size_t _size = this->size();
	if (offset >= _size)
		offset = _size - 1;
	size_t findOffset = GStringPiece::npos;
	auto cmpWith = caseSentive ? CompareWCharWith : CompareWCharWithIgnoreCase;
	for (auto itor = this->begin() + offset; itor != this->end(); ++itor)
	{
		//剩余字符不够比较
		if (this->end() - itor < str.size())
			break;
		auto itor1 = itor;
		bool result = true;
		//逐个比较字符
		for (auto itor2 = str.begin(); itor2 != str.end(); ++itor2, ++itor1)
		{
			if (!cmpWith(*itor1, *itor2))
			{
				result = false;
				break;
			}
		}
		//比较成功
		if (result)
		{
			findOffset = itor - this->begin();
			break;
		}
	}
	return findOffset;
}

size_t GStringPiece::findFirstNot(const GStringPiece& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	size_t _size = this->size();
	if (offset >= _size)
		offset = _size - 1;
	size_t findOffset = GStringPiece::npos;
	auto cmpWith = caseSentive ? CompareWCharWith : CompareWCharWithIgnoreCase;
	for (auto itor = this->begin() + offset; itor != this->end(); ++itor)
	{
		//剩余字符不够比较
		if (this->end() - itor < str.size())
		{
			findOffset = itor - this->begin();
			break;
		}
		auto itor1 = itor;
		bool result = false;
		//逐个比较字符
		for (auto itor2 = str.begin(); itor2 != str.end(); ++itor2, ++itor1)
		{
			if (!cmpWith(*itor1, *itor2))
			{
				result = true;
				break;
			}
		}
		//比较成功
		if (result)
		{
			findOffset = itor - this->begin();
			break;
		}
	}
	return findOffset;
}

size_t GStringPiece::findLast(const GStringPiece& str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	size_t _size = this->size();
	if (offset >= _size)
		offset = _size - 1;
	if (str.empty())
	{
		if (this->empty())
			return npos;
		return this->size() - 1;
	}
	if (this->size() < str.size())
		return npos;
	size_t i = offset;
	do 
	{
		//片段比较成功
		if (str.substr(i, str.size()).compareWith(str, caseSentive))
			return i;
		--i;
	} while (i != npos);
	return npos;
}

size_t GStringPiece::findLastNot(const GStringPiece& str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	size_t _size = this->size();
	if (offset >= _size)
		offset = _size - 1;
	if (str.empty())
	{
		if (this->empty())
			return npos;
		return this->size() - 1;
	}
	if (this->size() < str.size())
		return npos;
	size_t i = offset;
	do
	{
		//片段比较成功
		if (!str.substr(i, str.size()).compareWith(str, caseSentive))
			return i;
		--i;
	} while (i != npos);
	return npos;
}

GString GStringPiece::toString() const
{
	size_t _size = this->size();
	if (_size == 0)
		return L"";
	GString str;
	str.resize(_size);
	size_t i = 0;
	for (auto itor = begin(); itor != end(); ++itor, ++i)
		str[i] = *itor;
	return std::move(str);
}
