#include "GParser.h"
#include <vector>


size_t GParser::GParserNode::operator()(const GParserNode& node) const
{
	return std::_Hash_bytes((const unsigned char*)&node, sizeof(node));
}

bool GParser::GParserNode::operator()(const GParserNode& left, const GParserNode& right) const
{
	return left.parent == right.parent && left.index == right.index && left.state == right.state && left.ch == right.ch;
}
