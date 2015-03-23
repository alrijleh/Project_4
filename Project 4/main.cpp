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
   //try
   //{
      // Read the maze from the file.
	ifstream fin;
      string fileName = "maze1.txt";  
      fin.open(fileName.c_str());
      if (!fin)
      {
         cerr << "Cannot open " << fileName << endl;
         exit(1);
      }
	  fin.close();

	  //initialize maze
      maze m(fin);
      
	  //print maze
      m.print(m.numRows()-1,m.numCols()-1,0,0);

	  //map stuff -- should be moved
      Graph g;
      m.mapMazeToGraph(g);

		//print nodes
	  cout << g << endl;

	  system("pause");
   //}
}
