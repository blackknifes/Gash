#ifndef __GPARSER_H__
#define __GPARSER_H__
#include <unordered_set>

class GParser
{
public:
	struct GParserNode
	{
		GParserNode* parent;					//¸¸½Úµã
		int index;								//Ë÷Òý
		int state;								//×´Ì¬
		char ch;								//×Ö·û

		size_t operator()(const GParserNode& node) const;
		bool operator()(const GParserNode& left, const GParserNode& right) const;
	};
	typedef std::unordered_set<GParserNode, GParserNode, GParserNode> NodeSet;
private:
	NodeSet m_nodes;
};
#endif