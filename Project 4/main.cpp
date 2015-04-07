/*
Project 4c
Main.cpp
Contains function calls to perform maze solution

Fouad Al-Rijleh, Rachel Rudolph
*/

#include <iostream>
#include <limits.h>
#include <vector>
#include <list>
#include <fstream>
#include <queue>

#include <boost/graph/adjacency_list.hpp>

#include "heapV.h"
#include "maze.h"

using namespace boost;
using namespace std;

void printPath(Graph &g, Graph::vertex_descriptor start, Graph::vertex_descriptor end)
{
	Graph::vertex_descriptor current = end;
	stack<Graph::vertex_descriptor> stack;
	//this loops reverses the vertices so it does not print in reverse order
	while (current != start)
	{
		stack.push(current);
		current = g[current].pred;
	}
	//this loop prints the path
	cout << "Path: ";
	while (stack.size() != 0)
	{
		cout << stack.top() << " ";
		stack.pop();
	}
	cout << endl;
}

void relax(Graph &g, Graph::edge_descriptor e)
{
	Graph::vertex_descriptor u, v;
	u = source(e, g);
	v = target(e, g);
	if (g[v].weight > g[u].weight + g[e].weight)
	{
		g[v].weight = g[u].weight + g[e].weight;
		g[v].pred = u;
	}
}

bool bellmanFord(Graph &g, Graph::vertex_descriptor s, Graph::vertex_descriptor e)
{
	Graph::vertex_descriptor u, v;
	setNodeWeights(g, LargeValue);
	g[s].weight = 0;

	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr) //for each node
	{
		pair<Graph::out_edge_iterator, Graph::out_edge_iterator> range = out_edges(*vItr, g); //for each edge adjacent to that vertex
		for (Graph::out_edge_iterator e = range.first; e != range.second; e++)
		{
			relax(g, *e);
		}
	}

	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);
	for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr) //check for negative cycle
	{
		u = source(*eItr, g);
		v = target(*eItr, g);
		if (g[v].weight > g[u].weight + g[*eItr].weight) return false;
	}
	printPath(g, s, e);
	return true;
}

bool dijkstra(Graph &g, Graph::vertex_descriptor start, Graph::vertex_descriptor end)
{
	Graph::vertex_descriptor v;
	clearVisited(g);
	setNodeWeights(g, LargeValue);
	g[start].weight = 0;
	heapV<Graph::vertex_descriptor, Graph> heap;
	heap.initializeMinHeap(g);

	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		heap.minHeapInsert(*vItr, g);
	}

	while (heap.size() != 0)
	{
		v = heap.extractMinHeapMinimum(g);	
		if (v == end)
		{
			printPath(g, start, end);
			return true;
		}

		else
		{
			//pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrRange = adjacent_vertices(v, g);
			//for (Graph::adjacency_iterator w = vItrRange.first; w != vItrRange.second; ++w)
			pair<Graph::out_edge_iterator, Graph::out_edge_iterator> range = out_edges(v, g);
			for (Graph::out_edge_iterator e = range.first; e != range.second; e++)
			{
				relax(g, *e);
			}
		}
	}
	return false;
}

void initializeGraph(Graph &g,
	Graph::vertex_descriptor &start,
	Graph::vertex_descriptor &end, ifstream &fin)
	// Initialize g using data from fin.  Set start and end equal
	// to the start and end nodes.
{
	EdgeProperties e;

	int n, i, j;
	int startId, endId;
	fin >> n;
	fin >> startId >> endId;
	Graph::vertex_descriptor v;

	// Add nodes.
	for (int x = 0; x < n; x++)
	{
		v = add_vertex(g);
		if (x == startId)
			start = v;
		if (x == endId)
			end = v;
	}
	while (fin.peek() != '.')
	{
		fin >> i >> j >> e.weight;
		add_edge(i, j, e, g);
	}
}

int main()
{
	ifstream fin;

	// Read file from the file.
	string fileName;
	//cout << "Enter filename: ";
	//cin >> fileName;
	fileName = "graph1.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	Graph g;
	Graph::vertex_descriptor start, end;
	initializeGraph(g, start, end, fin);
	fin.close();

	if ( !bellmanFord(g, start, end) ) cout << "no shortest path exists" << endl;
	system("pause");
	if ( !dijkstra(g, start, end) ) cout << "no shortest path exists" << endl;
	system("pause");
}