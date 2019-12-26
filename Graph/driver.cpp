#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include "AdjacencyList.h"

void sp_states(string s, string d);

int main()
{
	sp_states("Washington", "Florida");
	return 0;
}

//Use AdjacencyList to find path from state s to state d that goes through the fewest states 
void sp_states(string s, string d)
{
	AdjacencyList g;
	map<string, int> state_to_int;
	map<int, string> int_to_state;

	fstream fs;
	fs.open("AdjStates.txt", std::fstream::in);

	string line;
	string state;
	string adj;
	int vertex;
	size_t pos = 0;

	while (getline(fs, line))
	{
		state = line.substr(0, line.find("\t"));
		vertex = g.add_vertex();

		state_to_int.insert(pair<string, int>(state, vertex));
		int_to_state.insert(pair<int, string>(vertex, state));
	}

	fs.close();
	fs.open("AdjStates.txt", std::fstream::in);

	while (getline(fs, line))
	{
		state = line.substr(0, line.find("\t"));
		line = line.substr(line.find("\t") + 1);

		vertex = state_to_int[state];

		stringstream s(line);
		while (getline(s, state, ','))
		{
			if (state[0] == ' ')
				state = state.substr(1);
			g.add_edge(vertex, state_to_int[state], 1);
		}
	}

	fs.close();

#if 0
	pair<vector<double>, vector<int> > sp = g.sp_djikstra(state_to_int[s]);

	cout << "From " << d << " to " << s << ": " << endl;

	int cur = state_to_int[d];
	int dest = state_to_int[s];

	while (cur != dest && cur != -1)
	{
		cout << int_to_state[cur] << " -> " << int_to_state[sp.second[cur]] << endl;
		cur = sp.second[cur];
	}

	cout << "FOUR COLORING: " << endl;

	vector<int> coloring = g.sat_four_color();
	
	cur = 0;
	for (int color : coloring)
	{
		cout << int_to_state[cur] << ": " << color << endl;
		cur++;
	}
#endif

	pair<vector<double>, vector<int>> sp_djik = g.sp_djikstra(state_to_int[s]);
	pair<vector<double>, vector<int>> sp_bell = g.sp_bellman_ford(state_to_int[s]);

	vector< vector<double> > sp = g.sp_floyd_warshall();

#if 0
	cout << '\t';

	for (map<string,int>::iterator it = state_to_int.begin() ; it != state_to_int.end(); it++)
	{
		cout << it->first << "\t ";
	}
	cout << endl;

	for (int i = 0; i < sp.size(); i++)
	{
		cout << int_to_state[i] << "\t ";
		for (int j = 0; j < sp.size(); j++)
			cout << sp[i][j] << "\t ";
		cout << endl;
	}
#endif

	bool good = true;
	for (int i = 0; i < sp_djik.first.size(); i++)
		if (sp_djik.first[i] != sp_bell.first[i] || sp_djik.first[i] != sp[state_to_int[s]][i])
			good = false;

	if (good)
		cout << "Output matched!" << endl;
	if (!good)
		cout << "Output different!" << endl;
}