// Sample solution for project #5

#include <iostream>
#include <limits.h>
#include <list>
#include <fstream>
#include <queue>
#include <vector>
#include <stack>

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
	void print(int,int,int,int);
   
	//Determine if legal to move to cell
	bool isLegal(int i, int j);

	//Create mapping of board to graph
	void mapMazeToGraph(Graph &g);
	void setMap(int i, int j, int n);
	int getMap(int i, int j);
   
	//Gets individual row and col of mapping node
	int getMapI(int n);
	int getMapJ(int n);
   
	void printPath(Graph::vertex_descriptor end,
						stack<Graph::vertex_descriptor> &s,
						Graph g);
   
	//Number of rows and columns
	int numRows(){return rows;};
	int numCols(){return cols;};

	// Mark all nodes in g as not visited.
	void clearVisited(Graph &g);
	
	// Set all node weights to w.
	void setNodeWeights(Graph &g, int w);
	
	// Mark all nodes in g as not marked
	void clearMarked(Graph &g);
   

private:
   int rows; // number of rows in the maze
   int cols; // number of columns in the maze12 a
   
   matrix<bool> value;
   matrix<Graph::vertex_descriptor> vMap;

   Graph g;
};

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
   fin >> rows;
   fin >> cols;
   
   char x;
   
   value.resize(rows,cols);
   for (int i = 0; i <= rows-1; i++)
      for (int j = 0; j <= cols-1; j++)
      {
         fin >> x;
         if (x == 'O')
            value[i][j] = true;
         else
            value[i][j] = false;
      }
   
   vMap.resize(rows,cols);
}

void maze::setMap(int i, int j, int n)
{
	node newNode = node();
	if (value[i][j])
	{
		newNode.mark();
	}
	newNode.setId(n);
}
int maze::getMap(int i, int j)
{
	return i * cols + j;
}

int maze::getMapI(int n) 
// Return i value of node mapping
{
	return n / numCols();
}

int maze::getMapJ(int n) 
// Return j value of node mapping
{
	return n % numCols();
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
   cout << endl;

   if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
      throw rangeError("Bad value in maze::print");

   if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
      throw rangeError("Bad value in maze::print");

   for (int i = 0; i <= rows-1; i++)
   {
      for (int j = 0; j <= cols-1; j++)
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

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze, indicating
// whether it is legal to go to cell (i,j).
{
   if (i < 0 || i > rows || j < 0 || j > cols)
      throw rangeError("Bad value in maze::isLegal");

   return value[i][j];
}

void maze::mapMazeToGraph(Graph &g)
// Create a graph g that represents the legal moves in the maze m.
{
	g = Graph(); //Wouldn't this create a new Graph object?

	int counter = 0;

	//Mapping of graph
	for (int i = 0; i < numRows(); i++)
	{
		for (int j = 0; j < numCols(); j++)
		{
			setMap(i, j, counter++);
		}
	}
}

void maze::printPath(Graph::vertex_descriptor end, stack<Graph::vertex_descriptor> &s, Graph g)
{
	for (int i = s.size(); i > 0; i--)
	{
		print(numRows() - 1, numCols() - 1, getMapI(s._Get_container[i]), getMapJ(s._Get_container[i]));

		if (i == 0)
			cout << "Complete" << endl;

		else if (i > 0 && s._Get_container[i - 1] == s._Get_container[i] + 1)
			cout << "Move Right" << endl;

		else if (i > 0 && s._Get_container[i - 1] == s._Get_container[i] - 1)
			cout << "Move Left" << endl;

		else if (i > 0 && s._Get_container[i - 1] == s._Get_container[i] - numCols())
			cout << "Move Up" << endl;

		else if (i > 0 && s._Get_container[i - 1] == s._Get_container[i] + numCols())
			cout << "Move Down" << endl;
	}
}

// Mark all nodes in g as not visited.
void maze::clearVisited(Graph &g)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].visited = false;
	}
}

// Set all node weights to w.
void maze::setNodeWeights(Graph &g, int w)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].weight = w;
	}
}


void maze::clearMarked(Graph &g)
{
	pair<Graph::vertex_iterator, Graph::vertex_iterator> vItrRange = vertices(g);
	for (Graph::vertex_iterator vItr = vItrRange.first; vItr != vItrRange.second; ++vItr)
	{
		g[*vItr].marked = false;
	}
}