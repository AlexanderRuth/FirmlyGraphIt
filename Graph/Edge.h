#pragma once
#include <set>
#include <iostream>

using namespace std;

class Edge 
{
public:
	Edge(int v, int w = 0);
	bool operator<(const Edge& rhs) const;
	friend ostream& operator<<(ostream& output, const Edge& E);
	int to() { return m_edge.first; }
	int weight() { return m_edge.second; }

private:
	pair<int, int> m_edge;
};
