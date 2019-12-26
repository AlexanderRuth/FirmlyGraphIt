#pragma once
#include <vector>
#include <set>
#include <iostream>
#include <queue>
#include <bitset>
#include <algorithm>
#include <list>
#include "Edge.h"

using namespace std;

class AdjacencyList {
public:

	AdjacencyList();

	/*
		Adds a vertex to the adjacency list.
		
		@return the integer key of the created vertex 
	*/
	int add_vertex();
	
	/*
		Adds an edge from a vertex to a vertex.
		Does not add duplicate edges (i.e., same (u, v)).

		@param u the source vertex.
		@param v the destination vertex.
		@param w the weight of the edge.
		@return true on success, false on failure.
	*/
	bool add_edge(int u, int v, int w=0);

	/*
		Removes an edge.

		@param u the source vertex.
		@param v the destination vertex.
	*/
	void remove_edge(int u, int v);

	/*
		Returns if a negative edge is present in the graph.

		@return true if negative edge exists, false otherwise.
	*/
	bool negative_edge_exists() { return neg_edges > 0; }

	/*
		Debug output of the adjacency list.
	*/
	void print_debug();

	/*
		Returns iterator to first vertex edge set.

		@return iterator pointing to first vertex.
	*/
	vector< set<Edge> >::iterator begin();

	/*
		Returns the end iterator of the vertex vector.

		@return end iterator of the vertex vector.
	*/
	vector< set<Edge> >::iterator end();

	/*
		Returns iterator to beginning of a vertex's
		edge set.

		@param v the vertex.
		@return begin iterator for the edge set.
	*/
	set<Edge>::iterator vertex_begin(int v);

	/*
		Returns the end iterator of a vertex's edge set.

		@param v the vertex.
		@return end iterator for the edge set.
	*/
	set<Edge>::iterator vertex_end(int v);

	/*
		Performs Djikstra's shortest path algorithm on the
		graph starting at some vertex.

		@param s the start vertex.
		@return A pair of vectors, the first is dist[]
		and the second is pred[].
	*/
	pair< vector<double>, vector<int> > sp_djikstra(int s);

	/*
		Performs Bellman-Ford's shortest path algorithm on the
		graph starting at some vertex.

		@param s the start vertex.
		@return A pair of vectors, the first is dist[]
		and the second is pred[]
	*/
	pair< vector<double>, vector<int> > sp_bellman_ford(int s);

	/*
		Finds all pairs shortest paths in the graph, using Floyd-Warshall's
		dynamic programming algorithm. Takes big-theta(|V|^3).

		@return A 2D vector of doubles containing the minimum distances.
		dist[u][v] = min distance from u to v.
	*/
	vector< vector<double> > sp_floyd_warshall();

	/*
		Finds the four coloring of the graph, if it exists.

		@return A vector of values 0 -> 3, corresponding to
		the color of the vertex at the index.
	*/
	vector<int> sat_four_color();

	/*
		Finds the four of the graph, if it exists, using AC-3.

		@return A vector of values 0 -> 3, corresponding to
		the color of the vertex at the index.
	*/
	vector<int> sat_four_color_ac3();

	void sat_four_color_ac3_kernel(vector< vector<int> > &domain);

private:

	//The actual adjacency list structure (a vector of Edge sets)
	vector< set< Edge > > adj_list;

	//The number of negative edges in the graph.
	int neg_edges;
	
	/*
		Recursive portion of the four coloring algorithm.

		@param options The available colors for each vertex.
		@param solution The current partial solution.
		@param cur The current vertex being colored.
		@return true if coloring found for vertices cur thru n, false otherwise.
	*/
	bool sat_four_color_recursive(vector< vector<int> > options, vector<int> & solution, int cur);

	bool sat_ac3_arc_reduce(vector< vector<int> >& domain, int x, int y);
};