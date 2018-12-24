#include "GRegex.h"



GRegex::GRegex()
	:m_matchFlag(MatchFlag::match_default)
{

}

GRegex::GRegex(const wchar_t* re)
	:m_regex(re), m_matchFlag(MatchFlag::match_default)
{
}

GRegex::GRegex(const std::wstring& re)
	: m_regex(re), m_matchFlag(MatchFlag::match_default)
{

}

GRegex::GRegex(const GString& re)
	: m_regex(re.data()), m_matchFlag(MatchFlag::match_default)
{

}

GRegex::GRegex(const GRegex& re)
	: m_regex(re.m_regex), m_matchFlag(MatchFlag::match_default)
{

}

GRegex::GRegex(GRegex&& re)
	: m_regex(std::move(re.m_regex)), m_matchFlag(MatchFlag::match_default)
{

}

void GRegex::setMatchFlag(MatchFlag flag)
{
	m_matchFlag = flag;
}

GRegex::MatchFlag GRegex::getMatchFlag() const
{
	return m_matchFlag;
}

bool GRegex::test(const GString& str) const
{
	return std::regex_match(str.data(), m_regex, m_matchFlag);
}

GString GRegex::search(const GString& str) const
{
	MatchResults result;
	if(!std::regex_search(str.begin(), str.end(), result, m_regex, m_matchFlag))
		return L"";

	return result.begin()->str().data();
}

GRegex::MatchResults GRegex::match(const GString& str) const
{
	typedef typename std::regex_token_iterator<GString::const_iterator> iterator;

	MatchResults results;

	std::regex_match(str.begin(), str.end(), results, m_regex, m_matchFlag);
	return results;
}

GRegex::const_iterator GRegex::begin(const GString& str) const
{
	return const_iterator(str.begin(), str.end(), m_regex, m_matchFlag);
}

GRegex::const_iterator GRegex::end() const
{
	return const_iterator();
}

void GRegex::replace(GString& str, const GString& replacement) const
{
	str.m_string = std::regex_replace(str.m_string, m_regex, replacement.m_string, m_matchFlag);
}

GStringPiece GRegex::Merge(const SubMatchResults& sub1, const SubMatchResults& sub2)
{
	GStringPiece::const_iterator _start, _end;
	if (sub1.matched)
	{
		_start = sub1.first;
		_end = sub2.matched ? sub2.second : sub1.second;
	}
	else
	{
		_start = sub2.first;
		_end = sub2.matched ? sub2.second : sub2.first;
	}
	return GStringPiece(_start, _end);
}

GStringPiece GRegex::StringPiece(const SubMatchResults& sub)
{
	if (!sub.matched)
		return GStringPiece();
	return GStringPiece(sub.first, sub.second);
}
