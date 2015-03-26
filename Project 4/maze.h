/*
Maze.h
Contains declarations and definitions for the maze class

Fouad Al-Rijleh, Rachel Rudolph
*/
#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <stack>

#include "Properties.h"
#include "d_except.h"
#include "d_matrix.h"
#include "node.h"
#include <boost/graph/adjacency_list.hpp>

#define LargeValue 99999999

using namespace boost;
using namespace std;

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

class maze
{
public:
	//Constructor
	maze(ifstream &fin);

	//Print
	void print(int, int, int, int);

	//Determine if legal to move to cell
	bool isLegal(int i, int j);

	//Create mapping of board to graph
	void mapMazeToGraph(Graph &g);
	void setMap(int i, int j, int n);
	int getMap(int i, int j);

	//Gets individual row and col of mapping node
	int getMapI(int n);
	int getMapJ(int n);

	//Add edges to graph
	void checkEdges(Graph g, int i, int j);

	//Print moves to complete maze
	void printPath(Graph::vertex_descriptor end,
		stack<Graph::vertex_descriptor> &stack,
		Graph g);

	//Number of rows and columns
	int numRows(){ return rows; };
	int numCols(){ return cols; };

	//Mark all nodes in g as not visited.
	void clearVisited(Graph &g);

	//Set all node weights to w.
	void setNodeWeights(Graph &g, int w);

	//Mark all nodes in g as not marked
	void clearMarked(Graph &g);

	void findPathDFSRecursive(Graph &g, Graph::vertex_descriptor v);
	void findPathDFSStack(Graph &g, stack<Graph::vertex_descriptor> &stack);
	void findShortestPathDFS();
	void findShortestPathBFS(Graph &g, queue<Graph::vertex_descriptor> &queue);

private:
	int rows; //number of rows in the maze
	int cols; //number of columns in the maze

	matrix<bool> value;
	matrix<Graph::vertex_descriptor> vMap;

	Graph g;
};

//Initializes a maze by reading values from fin.  Assumes that the
//number of rows and columns indicated in the file are correct.
maze::maze(ifstream &fin)
{
	fin >> rows;
	fin >> cols;

	char x;

	value.resize(rows, cols, LargeValue);
	for (int i = 0; i <= rows - 1; i++)
		for (int j = 0; j <= cols - 1; j++)
		{
			fin >> x;
			if (x == 'O')
				value[i][j] = true;
			else
				value[i][j] = false;
		}

	vMap.resize(rows, cols);
}

//Sets Map
void maze::setMap(int i, int j, int n)
{
	node newNode = node();
	if (value[i][j])
	{
		newNode.mark();
	}
	newNode.setId(n);

	typedef graph_traits<Graph>::vertex_descriptor Vertex;
	Vertex v = add_vertex(g);
	vMap[i][j] = v;
}

//Return Map
int maze::getMap(int i, int j)
{
	return i * cols + j;
}

// Return i value of node mapping
int maze::getMapI(int n)
{
	return n / cols;
}

//Return j value of node mapping
int maze::getMapJ(int n)
{
	return n % cols;
}

//Print out a maze, with the goal and current cells marked on the board.
void maze::print(int goalI, int goalJ, int currI, int currJ)
{
	cout << endl;

	if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
		throw rangeError("Bad value in maze::print");

	if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
		throw rangeError("Bad value in maze::print");

	for (int i = 0; i <= rows - 1; i++)
	{
		for (int j = 0; j <= cols - 1; j++)
		{
			if (i == goalI && j == goalJ)
				cout << "*";
			else
				if (i == currI && j == currJ)
					cout << "+";
				else
					if (value[i][j])
						cout << " ";
					else
						cout << "X";
		}
		cout << endl;
	}
	cout << endl;
}

//Return the value stored at the (i,j) entry in the maze, indicating
//whether it is legal to go to cell (i,j).
bool maze::isLegal(int i, int j)
{
	if (i < 0 || i > rows || j < 0 || j > cols)
		throw rangeError("Bad value in maze::isLegal");

	return value[i][j];
}

//Create a graph g that represents the legal moves in the maze m.
void maze::mapMazeToGraph(Graph &g)
{
	g = Graph();
	int counter = 0;

	//Mapping of graph
	for (int i = 0; i < numRows(); i++)
	{
		for (int j = 0; j < numCols(); j++)
		{
			if (value[i][j])
			{
				setMap(i, j, counter++);
				//checkEdges(g, i, j);
			}
		}
	}
}

void maze::checkEdges(Graph g, int i, int j)
{
	if (i - 1 > 0) if (vMap[i - 1][j] != LargeValue) //left
	{
		add_edge(i - 1, j, g);
	}
	if (i + 1 < cols) if (vMap[i + 1][j] != LargeValue) //right
	{
		add_edge(i + 1, j, g);
	}
	if (j - 1 > 0) if (vMap[i][j - 1] != LargeValue) //up
	{
		add_edge(i, j - 1, g);
	}
	if (j + 1 < rows) if (vMap[i][j + 1] != LargeValue) //down
	{
		add_edge(i, j + 1, g);
	}
}

//Prints the path represented by the vertices in stack s. Repeatedly
//calls print() to show each step of the path.
void maze::printPath(Graph::vertex_descriptor end, stack<Graph::vertex_descriptor> &stack, Graph g)
{
	int goalI, goalJ, currI, currJ;
	goalI = rows - 1;
	goalJ = cols - 1;

	for (int i = stack.size(); i > 0; i--)
	{
		currI = getMapI(stack.top());
		currJ = getMapJ(stack.top());
		stack.pop();
		print(goalI, goalJ, currI, currJ);
		/*
		if (i == 0)
		cout << "Complete" << endl;

		else if (i > 0 && stack._Get_container[i - 1] == stack._Get_container[i] + 1)
		cout << "Move Right" << endl;

		else if (i > 0 && stack._Get_container[i - 1] == stack._Get_container[i] - 1)
		cout << "Move Left" << endl;

		else if (i > 0 && stack._Get_container[i - 1] == stack._Get_container[i] - numCols())
		cout << "Move Up" << endl;

		else if (i > 0 && stack._Get_container[i - 1] == stack._Get_container[i] + numCols())
		cout << "Move Down" << endl;
		*/
	}
}

//Mark all nodes in g as not visited.
void maze::clearVisited(Graph &g)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].visited = false;
	}
}

//Set all node weights to w.
void maze::setNodeWeights(Graph &g, int w)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].weight = w;
	}
}

//Unmark all nodes
void maze::clearMarked(Graph &g)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].marked = false;
	}
}

//Looks for path from start to goal using DFS using recursion
void maze::findPathDFSRecursive(Graph &g, Graph::vertex_descriptor v)
{
	g[v].visited = true;

	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrAdjRange = adjacent_vertices(v, g);
	for (Graph::adjacency_iterator w = vItrAdjRange.first; w != vItrAdjRange.second; w++)
	{
		if (g[*w].visited == false)
		{
			findPathDFSRecursive(g, *w);
		}
	}
}

//Looks for path from start to goal using DFS using a stack not recursion
void maze::findPathDFSStack(Graph &g, stack<Graph::vertex_descriptor> &stack)
{
	clearVisited(g);
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	Graph::vertex_iterator vItr = vItrRange.first;
	stack.push(*vItr);
	g[*vItr].visited = true;
	while (stack.size() != 0)
	{
		Graph::vertex_descriptor v = stack.top();
		pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrAdjRange = adjacent_vertices(v, g);
		for (Graph::adjacency_iterator w = vItrAdjRange.first; w != vItrAdjRange.second; w++)
		{
			if (g[*w].visited == false)
			{
				g[*w].visited = true;
				stack.push(*w);
			}
			else
			{
				stack.pop();
			}			
		}
	}
}

//Looks for shortest path from start to goal using DFS
void findShortestPathDFS()
{
}

//Looks for shortest path from start to goal using BFS
void maze::findShortestPathBFS(Graph &g, queue<Graph::vertex_descriptor> &queue)
{
	clearVisited(g);
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	Graph::vertex_iterator vItr = vItrRange.first;
	queue.push(*vItr);
	g[*vItr].visited = true;
	while (queue.size() != 0)
	{
		Graph::vertex_descriptor v = queue.front();
		pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrAdjRange = adjacent_vertices(v, g);
		for (Graph::adjacency_iterator w = vItrAdjRange.first; w != vItrAdjRange.second; w++)
		{
			if (g[*w].visited == false)
			{
				g[*w].visited = true;
				queue.push(*w);
			}
		}
		queue.pop();
	}
}

//Output operator for the Graph class. Prints out all nodes and their properties, 
//and all edges and their properties.
ostream &operator<<(ostream &ostr, const Graph &g)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{

		ostr << "Node " << *vItr << " Properties" << endl;
		ostr << "Marked: " << g[*vItr].marked << endl;
		ostr << "Visited: " << g[*vItr].visited << endl;
		ostr << "Weight: " << g[*vItr].weight << endl << endl;
	}

	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);
	for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr)
	{

		ostr << "Edge " << *eItr << " Properties" << endl;
		ostr << "Marked: " << g[*eItr].marked << endl;
		ostr << "Visited: " << g[*eItr].visited << endl;
		ostr << "Weight: " << g[*eItr].weight << endl << endl;
	}

	return ostr;
}
