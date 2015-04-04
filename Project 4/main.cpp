/*
Project 4b
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

#define LargeValue 99999999

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

bool bellmanFord(Graph &g, Graph::vertex_descriptor s)
{
	setNodeWeights(g, LargeValue);
	g[s].weight = 0;

	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);
	for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr)
	{
		relax(g, *eItr);
	}
}

bool dijkstra(Graph &g, Graph::vertex_descriptor s)
{
	Graph::vertex_descriptor v, w;
	clearVisited(g);
	setNodeWeights(g, LargeValue);
	g[s].weight = 0;

	heapV<Graph::vertex_descriptor, Graph> heap;
	heap.initializeMinHeap(g);

	while (heap.size() != 0)
	{
		v = heap.extractMinHeapMinimum(g);
		if (false) //v == end)
		{
			return true;
		}
		else
		{
			pair<Graph::adjacency_iterator, Graph::adjacency_iterator> range = adjacent_vertices(v, g);
			for (Graph::adjacency_iterator w = range.first; w != range.second; w++)
			{
				//get edge e
				relax(g, e);
			}
		}
	}

}


int main()
{
	ifstream fin;

	// Read the maze from the file.
	string fileName = "maze1.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	maze m(fin);
	fin.close();

	m.print(m.numRows() - 1, m.numCols() - 1, 0, 0);

	//initialzing
	Graph g = Graph();
	m.mapMazeToGraph(g);

	//Solving

	//DFS Rescursive
	Graph::vertex_descriptor startVertex = *vertices(g).first;
	if (!m.findPathDFSRecursive(g, startVertex))
	{
		cout << "No path exists" << endl;
	}
	else
	{
		cout << "Path found" << endl;
	}
	system("pause");

	//DFS Stack
	stack<Graph::vertex_descriptor> vertexStack;
	m.findPathDFSStack(g, vertexStack);
	system("pause");

	//BFS
	queue<Graph::vertex_descriptor> vertexQueue;
	m.findShortestPathBFS(g, vertexQueue);
	system("pause");

	//Shortest DFS
	m.findShortestPathDFS(g, vertexStack);
	system("pause");

	//cout << g << endl; //prints out all created vertices and edges
	//system("pause");
}