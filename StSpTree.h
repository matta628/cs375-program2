#ifndef STSPTREE_H
#define STSPTREE_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>

class StSpTree{
private:
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
	struct compareNodePtr{
		bool operator() (const Node* a, const Node* b) const{
			return a->bound < b->bound;
		}
	};
	class Item{
	public:
		double weight;
		double profit;
		Item(double w, double p){
			weight = w;
			profit = p;
		}
	};
	void buildTree(Node* n, int i);
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
	StSpTree(int n, int c, std::vector<std::string> & list);
	void bestFirstBandB();
	void writeToFile(char *);
};

#endif
