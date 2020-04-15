#ifndef STSPTREE_H
#define STSPTREE_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>

/*
  Class StSpTree models a Binary State Space Tree for the 0/1 knapsack problem.
  It takes a list of items, each with a weight and profit, and chooses a subset of these
  items that maximizes total profit, under the constraint that the total weight of the 
  items is less than a given capacity.
  This class contains a constructor that sets the number of items, capacity of the knapsack,
  and creates a vector of items (and sorts them).
  
  prettyPrint() prints the State Space Tree in a way that shows the levels of each node,
  and each node's weight, profit, and bound. Used to debug the program.
  
  bound() calculates the upperbound for a Node.
  
  optimalSolution() backtracks from the node that represents the optimal solution and creates
  the optimal subset of items for the max profit.
  
  bestFirstBandB() starts with the root node and uses a priority queue to expand the node with
  the greatest bound. it updates the max profit as it works through the tree, and keeps track
  of the node that represents the state of the optimal solution.
  
  writeToFile() organizes the output of the StateSpaceTree and formats it into an output file
 */
class StSpTree{
private:
	/*
	  Class Node models each possible state of the problem. It stores the weight, profit,
	  level, and bound for each state. Additionally, it stores pointers to two child nodes,
	  yes and no, and a parent node.
	  
	  The constructor initializes the yes and no child pointers to nullptr, and sets the other
	  variables accordingly.
	 */
	class Node{
	public:
		int level;
		double weight;
		double profit;
		double bound;
		Node *parent, *yes, *no;
		Node(Node *ptr, int lvl, double w, double p){
			parent = ptr;
			level = lvl;
			weight = w;
			profit = p;
			bound = 0;
			yes = no = nullptr;
		}
	};
	/*
	  This struct is used to order the Nodes in the priority queue such that the highest bound
	  is at the top.
	 */
	struct compareNodePtr{
		bool operator() (const Node* a, const Node* b) const{
			return a->bound < b->bound;
		}
	};
	/* 
	   Class Item stores each item, with its weight and profit.
	   
	   The constructor sets the variables accordingly.
	 */
	class Item{
	public:
		double weight;
		double profit;
		Item(double w, double p){
			weight = w;
			profit = p;
		}
	};
	void prettyPrint(Node *n, int i);
	double bound(Node* n);
	void optimalSolution(Node *n);
	Node *root;
	std::vector<Item*> items;
	std::vector<Item*> optimal;
	double capacity;
	int numItems;
	int numNodes;
	int numLeaves;
	double best;
public:
	StSpTree(int n, double c, std::vector<std::string> & list);
	void bestFirstBandB();
	void writeToFile(char *);
};

#endif
