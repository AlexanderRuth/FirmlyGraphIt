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
		//sat_four_color_ac3_kernel(temp_options);
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

vector<int> AdjacencyList::sat_four_color_ac3()
{
	vector< vector<int> > domain(adj_list.size(), vector<int>{0, 1, 2, 3});
	
	domain.at(0) = vector<int>{ 0 };
	bool done = false;

	while (!done)
	{
		sat_four_color_ac3_kernel(domain);

		done = true;
		for (int i = 0; i < domain.size(); i++)
		{
			if (domain.at(i).size() > 1)
			{
				done = false;
				domain.at(i) = vector<int>(1, domain.at(i).at(0));
				break;
			}
		}
	}

	vector<int> solution(adj_list.size());

	for (int i = 0; i < domain.size(); i++)
	{
		if (!domain.at(i).size())
			continue;
		solution[i] = domain[i][0];
	}
	return solution;
}

void AdjacencyList::sat_four_color_ac3_kernel(vector< vector<int> >& domain)
{
	vector< vector<int> > options = domain;
	list<int> worklist;
	int c;
	int x = 0;


	for (set<Edge> vertex : adj_list)
	{
		//No unary constraints

		//Add all binary constraints
		for (Edge e : vertex)
			worklist.push_back(e.to());

		while (worklist.size())
		{
			c = worklist.front();

			if (sat_ac3_arc_reduce(options, x, c))
			{
				if (!options.at(x).size())
					//Failure
					cout << "Failure" << endl;
				else
				{
					for (Edge v : vertex)
						if (v.to() != c)
							worklist.push_back(v.to());
				}
			}

			worklist.pop_front();
		}
		x++;
	}

	domain = options;
}

bool AdjacencyList::sat_ac3_arc_reduce(vector< vector<int> >& domain, int x, int y)
{
	bool change = false;
	bool color_allowed;

	for (int x_color : domain.at(x))
	{
		color_allowed = false;
		for (int y_color : domain.at(y))
		{
			if (y_color != x_color)
				color_allowed = true;
		}
		
		if (!color_allowed)
		{
			vector<int> *x_domain = &domain.at(x);
			x_domain->erase(remove(x_domain->begin(), x_domain->end(), x_color), x_domain->end());
			cout << "ERASING: " << x_color << endl;
			change = true;
		}
	}

	return change;
}

//All pairs shortest paths using Floyd-Warshall Algorithm (Dynamic Programming)
vector< vector<double> > AdjacencyList::sp_floyd_warshall()
{
	size_t v_len = adj_list.size();

	//Create AdjacencyMatrix
	vector< vector<double> > adj_matrix(v_len, vector<double>(v_len, 0));

	vector< vector<double> > dist(v_len, vector<double>(v_len, numeric_limits<double>::infinity()));

	int u = 0;
	for (set<Edge> vertex : adj_list)
	{
		dist[u][u] = 0;
		for (Edge edge : vertex)
		{
			adj_matrix[u][edge.to()] = edge.weight();
			dist[u][edge.to()] = edge.weight();
		}
		u++;
	}

	for (int k = 0; k < v_len; k++)
		for (int i = 0; i < v_len; i++)
			for (int j = 0; j < v_len; j++)
				if (dist[i][j] > dist[i][k] + dist[k][j])
					dist[i][j] = dist[i][k] + dist[k][j];

	return dist;
}

pair< vector<double>, vector<int> > AdjacencyList::sp_bellman_ford(int s)
{
	vector<double> dist(adj_list.size(), numeric_limits<double>::infinity());
	vector<int> pred(adj_list.size(), -1);
	dist[s] = 0;

	int u;

	//Relax |V| - 1 times
	for (int v = 0; v < adj_list.size() - 1; v++)
	{
		u = 0;
		for (set<Edge> vertex : adj_list)
		{
			for (Edge e : vertex)
			{
				if (dist[u] + e.weight() < dist[e.to()])
				{
					dist[e.to()] = dist[u] + e.weight();
					pred[e.to()] = u;
				}
			}
			u++;
		}
	}

	u = 0;

	//One more run through. If RELAX succeeds, then there is a negative cost cyle
	for (set<Edge> vertex : adj_list)
	{
		for (Edge e : vertex)
		{
			if (dist[u] + e.weight() < dist[e.to()])
			{
				//Uh oh, negative weight cycle!
				throw logic_error("Negative weight/cost cycle in graph");
			}
		}
		u++;
	}

	return pair< vector<double>, vector<int>>(dist, pred);
}

			