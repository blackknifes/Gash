#ifndef __GPARSER_H__
#define __GPARSER_H__
#include <unordered_set>

class GParser
{
public:
	struct GParserNode
	{
		GParserNode* parent;					//���ڵ�
		int index;								//����
		int state;								//״̬
		char ch;								//�ַ�

		size_t operator()(const GParserNode& node) const;
		bool operator()(const GParserNode& left, const GParserNode& right) const;
	};
	typedef std::unordered_set<GParserNode, GParserNode, GParserNode> NodeSet;
private:
	NodeSet m_nodes;
};
#endif