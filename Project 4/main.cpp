#include <iostream>
#include <limits.h>
#include <vector>
#include <list>
#include <fstream>
#include <queue>

#include <boost/graph/adjacency_list.hpp>

//#include "heapV.h"
#include "maze.h"

using namespace boost;
using namespace std;



// typedef property<edge_weight_t, int> EdgeProperty;

#define LargeValue 99999999

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