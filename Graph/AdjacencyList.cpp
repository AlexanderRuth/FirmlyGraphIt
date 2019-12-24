#include "AdjacencyList.h"

AdjacencyList::AdjacencyList()
	: neg_edges(0)
{
}

//Returns the numerical ID of the vertex
int AdjacencyList::add_vertex()
{
	int size = adj_list.size();
	adj_list.push_back(set< Edge >());
	return size;
}

//Adds an edge if it does not already exist
bool AdjacencyList::add_edge(int u, int v, int w)
{
	//If either vertex is invalid
	if (u >= adj_list.size() || v >= adj_list.size())
		return false;

	//If the isertion is successful
	if (adj_list.at(u).insert(Edge(v, w)).second)
	{
		if (w < 0) neg_edges++;
		return true;
	}
	//If a duplicate exists
	else
	{
		return false;
	}
}

//Removes an edge if it exists.
void AdjacencyList::remove_edge(int u, int v)
{
	adj_list.at(u).erase(Edge(v, 0));
}

//Debug output
void AdjacencyList::print_debug()
{
	int cur = 0;
	for (set<Edge> vertex : adj_list)
	{
		cout << cur << " -> ";
		for (Edge edge : vertex)
			cout << edge << " ";
		cout << endl;
		cur++;
	}
}

//Begin iterator for vertices.
vector< set<Edge> >::iterator AdjacencyList::begin()
{
	return adj_list.begin();
}

//End iterator for vertices.
vector< set<Edge> >::iterator AdjacencyList::end()
{
	return adj_list.end();
}

//Begin iterator for edges.
set<Edge>::iterator AdjacencyList::vertex_begin(int v)
{
	return adj_list.at(v).begin();
}

//End iterator for edges.
set<Edge>::iterator AdjacencyList::vertex_end(int v)
{
	return adj_list.at(v).end();
}

//The next minimum distance vertex.
int next_min(const vector<double> & dist, const vector<bool> & dequeued)
{
	double min = std::numeric_limits<double>::infinity();
	int min_i = -1;

	for (int i = 0; i < dist.size(); i++)
	{
		if (dist[i] < min && !dequeued.at(i))
		{
			min = dist[i];
			min_i = i;
		}
	}

	return min_i;
}

//Returns the shortest path using Djikstra's algorithm.
pair<vector<double>, vector<int> > AdjacencyList::sp_djikstra(int s)
{
	//Distance Array (all but s are infinity at start)
	vector<double> dist(adj_list.size(), std::numeric_limits<double>::infinity());
	
	//Predecessor Array (no predecessors at start)
	vector<int> pred(adj_list.size(), -1);
	
	//Bitvector of vertices already checked (all 0 at start)
	vector<bool> dequeued(adj_list.size());

	double alt; //The (possibly) new distance for v

	int u;	//Current vertex
	int v;	//Vertex pointed to by u

	set<Edge> *vertex;	//Pointer to current edge set

	//Start vertex has dist of 0
	dist[s] = 0;

	//While a minimum vertex can be checked.
	while((u = next_min(dist, dequeued)) != -1)
	{
		vertex = &adj_list.at(u);
		dequeued[u] = true;

		//For each edge, see if dist[u] + w(u,v) < dist[v]
		for (Edge edge : *vertex)
		{
			v = edge.to();
			alt = dist.at(u) + (double)edge.weight();
			if (alt < dist[v])
			{
				dist[v] = alt;
				pred[v] = u;
			}
		}
	}

	//Return the pair of dist[] and pred[]
	return pair< vector<double>, vector<int> >(dist, pred);
}

//Returns the four coloring of the graph, if it exists
vector<int> AdjacencyList::sat_four_color()
{
	//The available colors (all four available at start)
	vector< vector<int> > available(adj_list.size(), vector<int> {0, 1, 2, 3});

	//The solution
	vector< int > solution(adj_list.size(), -1);

	//Recursive backtracking
	sat_four_color_recursive(available, solution, 0);
	
	return solution;
}

//Recursive backtracker for four coloring
bool AdjacencyList::sat_four_color_recursive(vector< vector<int> > options, vector<int> & solution, int cur)
{
	//Temporary storage of the vertex coloring options
	vector< vector<int> > temp_options(options);

	bool skip;	//Skip the color option, since makes a later vertex invalid

	//If not options, then no possible solution
	if (options.at(cur).size() == 0)
		return false;

	//Base Case: Last vertex, choose any available color choice.
	if (cur == adj_list.size() - 1)
	{ 
		solution[cur] = options[cur][0];
		return true;
	}

	//For each possible vertex coloring
	for (int i = 0; i < options.at(cur).size(); i++)
	{
		int color = options.at(cur).at(i);
		skip = false;

		//Remove the color option from all adjacent vertices
		for (Edge edge : adj_list.at(cur))
		{
			vector<int> *edge_opts = &temp_options.at(edge.to());
			edge_opts->erase(remove(edge_opts->begin(), edge_opts->end(), color), edge_opts->end());
			if (edge_opts->size() == 0) 
				skip = true;
		}

		//If vertices cur + 1 thru |V| can be colored, then solution is found
		if (!skip && sat_four_color_recursive(temp_options, solution, cur + 1))
		{
			solution.at(cur) = color;
			return true;
		}

		//Otherwise, restart
		temp_options = options;
	}

	//No solution exists with the current combination
	return false;


}

			