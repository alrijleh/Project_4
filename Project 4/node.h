/*
Node.h
Contains declarations and definitions for the node class

Fouad Al-Rijleh, Rachel Rudolph
*/
#pragma once
#include <iostream>
#include <fstream>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <stack>

using namespace std; 

typedef int NodeWeight;

class node
{
public:
	node();
	node(const node &);
	node &operator=(const node &);

	void setId(int i);
	int getId() const;

	void setWeight(NodeWeight);
	NodeWeight getWeight() const;

	void setNode(int, NodeWeight, bool, bool);

	void mark();
	void unMark();
	bool isMarked() const;

	void visit();
	void unVisit();
	bool isVisited() const;

private:
	int id;
	NodeWeight weight;
	bool marked;
	bool visited;
};

//Constructor that creates a new, uninitialized node
node::node()
{
	unMark();
	unVisit();
	setWeight(0);
}

//Copy constructor
node::node(const node &n)
{
	setNode(n.getId(), n.getWeight(), n.isMarked(), n.isVisited());
}

//Overloaded assignment operator.
node &node::operator=(const node &n)
{
	setNode(n.getId(), n.getWeight(), n.isMarked(), n.isVisited());
	return *this;
}

//Return node weight
NodeWeight node::getWeight() const
{
	return weight;
}

//Set node weight to w
void node::setWeight(NodeWeight w)
{
	weight = w;
}

//Set node's id to i and throws an exception if i < 0
void node::setId(int i)
{
	if (i < 0)
		throw rangeError("Bad value in node::setId");

	id = i;
}

int node::getId() const
// Return node's id
{
	return id;
}

void node::setNode(int id, NodeWeight w = 0, bool m = false, bool v = false)
// Initialize a node;
{
	setId(id);
	setWeight(w);

	if (m)
		mark();
	else
		unMark();

	if (v)
		visit();
	else
		unVisit();
}

void node::mark()
// Mark node.
{
	marked = true;
}

void node::unMark()
// Un-mark node.
{
	marked = false;
}

bool node::isMarked() const
// Return true if node is marked, and false otherwise.
{
	return marked;
}

void node::visit()
// Set visited to true;
{
	visited = true;
}

void node::unVisit()
// Set visited to false;
{
	visited = false;
}

bool node::isVisited() const
// Return true if node is visited, and false otherwise.
{
	return visited;
}