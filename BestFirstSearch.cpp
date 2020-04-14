/*
Input:
Line 1: n,c // n is the size of the problem (the total number of items) and C is the total weight limit 
Lines 2 to n+1: wi,pi // wi is the weight of item i and pi is the profit of item i

Output:
Line 1: the size of the problem (i.e., the total number of items),the optimal value (i.e., the maximum 
profit),the size (k) of the first optimal solution found (i.e., the size of the first optimal subset 
of items found to produce the maximum profit)
Line 2: the total number of nodes visited (by the algorithm from the start to the end of the search
including the root and the last node visited in the binary search space tree),the total number of 
leaf nodes visited
Lines 3 to 3+(k-1): each line contains the weight and profit of an item selected in order for the 
first optimal solution found.

In this assignment, you need to implement the best-first-search branch and bound algorithm for the 
0-1 knapsack problem introduced in the class. The algorithm needs to find one optimal solution as 
quickly as possible. Note: the input problem instance may not contain items in decreasing order of 
profit/weight, so your program needs to sort the items first before the search.
 */
#include "StSpTree.h"

using namespace std;

int main(int argc, char** argv){
	if (argc < 3) return -1;
	
	char * in = argv[1];
	ifstream input(in);
	double n;
	double c;
	vector<string> items;

	string line;
	int i = 0;
	while (input >> line){
		if (i == 0){
			string nStr = line.substr(0, line.find(","));
			n = stoi(nStr);
			string cStr = line.substr(line.find(",") + 1,-1);
			c = stoi(cStr);
		}
		else{
			items.push_back(line);
		}
		i++;
	}

	StSpTree *tree = new StSpTree(n, c, items);
	tree->bestFirstBandB();
	tree->writeToFile(argv[2]);
	//ofstream output(argv[2], ios::out | ios::trunc);
	//output.close();
}
