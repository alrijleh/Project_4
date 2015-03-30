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
	void setMap(Graph &g, int i, int j, int n);
	int getMap(int i, int j);

	//Gets individual row and col of mapping node
	int getMapI(int n);
	int getMapJ(int n);

	//Add edges to graph
	void addEdges(Graph &g, int i, int j);

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

	bool findPathDFSRecursive(Graph &g, Graph::vertex_descriptor v);
	void findPathDFSStack(Graph &g, stack<Graph::vertex_descriptor> &stack);
	void findShortestPathDFS(Graph &g, stack<Graph::vertex_descriptor> &currentStack);
	void findShortestPathBFS(Graph &g, queue<Graph::vertex_descriptor> &queue);

private:
	int rows; //number of rows in the maze
	int cols; //number of columns in the maze

	Graph::vertex_descriptor target;

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
void maze::setMap(Graph &g, int i, int j, int n)
{
	node newNode = node();
	newNode.mark();
	newNode.setId(n);

	Graph::vertex_descriptor v = add_vertex(g);
	g[v].marked = true;
	g[v].visited = false;

	if (i == cols - 1 && j == rows - 1)
	{
		target = v;
	}
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
	if (i < 0 || i >= rows || j < 0 || j >= cols) return false;
	else return value[i][j];
}

//Create a graph g that represents the legal moves in the maze m.
void maze::mapMazeToGraph(Graph &g)
{
	int counter = 0;

	//Mapping of graph
	for (int i = 0; i < numRows(); i++)
	{
		for (int j = 0; j < numCols(); j++)
		{
			if (isLegal(i, j))
			{
				setMap(g, i, j, counter++);
				addEdges(g, i, j);
			}
		}
	}
}

void maze::addEdges(Graph &g, int i, int j)
{
	/*
	if (i - 1 > 0) if (vMap[i - 1][j] != LargeValue) //left
	{
		Graph::vertex_descriptor u, v;
		u = vMap[i][j];
		v = vMap[i - 1][j];
		pair<Graph::edge_descriptor, bool> newEdge = add_edge(u, v, g);
	}
	*/
	for (int x = -1; x <= 1; x++) //x and y are vertical and horizantal offsets
	{
		for (int y = -1; y <= 1; y++)
		{
			if (i != j && isLegal(i+x, j+y) && vMap[i+x][j+y] != LargeValue && x != y)
			{
				Graph::vertex_descriptor u, v;
				u = vMap[i][j];
				v = vMap[i+x][j+y];
				add_edge(u, v, g);
				add_edge(v, u, g); //make bidirectional edges
			}
		}
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
bool maze::findPathDFSRecursive(Graph &g, Graph::vertex_descriptor v)
{
	g[v].visited = true;

	pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrAdjRange = adjacent_vertices(v, g);
	Graph::adjacency_iterator w = vItrAdjRange.first;
	for (w = vItrAdjRange.first; w != vItrAdjRange.second; w++)
	{
		if (g[*w].visited == false)
		{
			print(rows - 1, cols - 1, g[*w].cell.first, g[*w].cell.second);
			if (*w == target) return true; //if at end of the maze
			else return findPathDFSRecursive(g, *w);
		}
	}
	return false;
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
			if (*w == target) //if at end of the maze
			{
				cout << "Reached Goal" << endl;
				return;
			}
			if (g[*w].visited == false)
			{
				g[*w].visited = true;
				stack.push(*w);
			}
			else
			{
				stack.pop();
			}
			print(rows - 1, cols - 1, g[*w].cell.first, g[*w].cell.second);
		}
	}
	cout << "No path exists" << endl;
}

//Looks for shortest path from start to goal using DFS
void maze::findShortestPathDFS(Graph &g, stack<Graph::vertex_descriptor> &currentStack)
{
	clearVisited(g);
	stack<Graph::vertex_descriptor> shortestStack;

	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	Graph::vertex_iterator vItr = vItrRange.first; //vITR points to first node
	currentStack.push(*vItr);
	g[*vItr].visited = true;
	while (currentStack.size() != 0)
	{
		Graph::vertex_descriptor v = currentStack.top();
		pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vItrAdjRange = adjacent_vertices(v, g);
		Graph::adjacency_iterator w = vItrAdjRange.first;
		for (w = vItrAdjRange.first; w != vItrAdjRange.second; w++)
		{
			if (*w == target) //if at end of the maze
			{
				if (shortestStack.size() == 0 || currentStack.size() < shortestStack.size())
				{
					shortestStack = currentStack;
				}
			}
			if (g[*w].visited == false)
			{
				g[*w].visited = true;
				currentStack.push(*w);
			}
			else
			{
				currentStack.pop();
			}
			print(rows - 1, cols - 1, g[*w].cell.first, g[*w].cell.second);
		}
		w--;
		g[*w].visited == false;
	}
	if (shortestStack.size() == 0) cout << "No path exists" << endl;
	else cout << "Shortest path is " << shortestStack.size() << " steps long" << endl;
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
			if (*w == target) //if at end of the maze
			{
				cout << "Reached Goal" << endl;
				return;
			}

			if (g[*w].visited == false)
			{
				print(rows - 1, cols - 1, g[*w].cell.first, g[*w].cell.second);
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

		ostr << "Node " << *vItr;
		ostr << " | Marked: " << g[*vItr].marked;
		ostr << " | Visited: " << g[*vItr].visited << endl;
	}
	cout << endl;

	pair<Graph::edge_iterator, Graph::edge_iterator> eItrRange = edges(g);
	for (Graph::edge_iterator eItr = eItrRange.first; eItr != eItrRange.second; ++eItr)
	{

		ostr << "Edge " << *eItr;
		ostr << " | Marked: " << g[*eItr].marked;
		ostr << " | Visited: " << g[*eItr].visited << endl;
		//ostr << "Weight: " << g[*eItr].weight << endl << endl;
	}

	return ostr;
}
