#ifndef __GREGEX_H__
#define __GREGEX_H__
#include <regex>
#include "GString.h"
#include <unordered_map>
#include "GStringPiece.h"

class GRegex
{
public:
	typedef typename std::unordered_map<GString, GString> MatchMap;
	typedef typename std::vector<GString> MatchList;
	typedef typename std::match_results<GString::const_iterator> MatchResults;
	typedef typename std::regex_token_iterator<GString::const_iterator> MatchTokenResult;
	typedef typename std::regex_token_iterator<GString::const_iterator> const_iterator;
	typedef typename std::regex_constants::match_flag_type MatchFlag;
	typedef typename MatchResults::value_type SubMatchResults;

	static GStringPiece Merge(const SubMatchResults& sub1, const SubMatchResults& sub2);
	static GStringPiece StringPiece(const SubMatchResults& sub);

	GRegex();
	GRegex(const wchar_t* re);
	GRegex(const std::wstring& re);
	GRegex(const GString& re);
	GRegex(const GRegex& re);
	GRegex(GRegex&& re);

	void setMatchFlag(MatchFlag flag);
	MatchFlag getMatchFlag() const;

	bool test(const GString& str) const;
	GString search(const GString& str) const;
	MatchResults match(const GString& str) const;

	const_iterator begin(const GString& str) const;
	const_iterator end() const;

	void replace(GString& str, const GString& replacement) const;

private:
	std::wregex m_regex;
	MatchFlag m_matchFlag;
};
#endif