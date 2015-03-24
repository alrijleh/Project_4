/*
Properties.h
Contains typedef and struct definitions

Fouad Al-Rijleh, Rachel Rudolph
*/
#pragma once
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

struct VertexProperties;
struct EdgeProperties;

typedef adjacency_list<vecS, vecS, bidirectionalS, VertexProperties, EdgeProperties> Graph;

//Create a struct to hold properties for each vertex
struct VertexProperties
{
	std::pair<int, int> cell; // maze cell (x,y) value
	Graph::vertex_descriptor pred;
	bool visited;
	bool marked;
	int weight;
};

//Create a struct to hold properties for each edge
struct EdgeProperties
{
	int weight;
	bool visited;
	bool marked;
};