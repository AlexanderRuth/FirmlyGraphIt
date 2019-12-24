#include "Edge.h"

Edge::Edge(int v, int w)
	: m_edge(v, w) {}

bool Edge::operator<(const Edge& rhs) const
{
	return rhs.m_edge.first > this->m_edge.first;
}

ostream& operator<<(ostream& output, const Edge& E)
{
	output << "(" << E.m_edge.first << ", " << E.m_edge.second << ")";
	return output;
}