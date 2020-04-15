COMPILE INSTRUCTIONS
--> "make" will compile the whole program.
--> "make run" will compile the whole program and run ./BestFirstSearch input.txt output.txt

DATA STRUCTURES & IMPLEMENTATION
--> Binary State Space Tree
	The Binary State Space Tree is made up of nodes which represent each possible state of the problem.
	Starting with the root node, each level of the tree is built by adding or not adding another item.
	The depth of the tree is equivalent to the index of item which will/wont be added (e.g. the root,
	at level 0, has no items added, but at level 1, the 1st item is considered).
	Each node contains a pointer to two child nodes: "yes", which represents adding the next item,
	and "no", where the next item is not added. A pointer to the parent is also tracked for convenience.
--> Priority Queue
	The STL priority queue is used to keep track of which node to expand next. A custom comparator struct,
	compareNodePtr, is passed to the priority queue to order the nodes by the value of its bound.
	The priority queue uses an vector of Node pointers as its container.
--> Vector
	The STL vector is used to store the Item pointers.
	One vector stores all items, and the other vector stores the subset of items of the optimal solution.

COMPUTATION TIME ANALYSIS
--> The State Space Tree constructor has a worst case O(nLogn), where n is the total number of items.
	The method bestFirstBandB() has a worst case of O(2^n), because each item can be either
	part of the optimal solution or not part of the optimal solution. However, it will have a average
	or expected case that is less than 2^n because the priority queue prioritizes the nodes
	which have the highest chance of expanding to the best solution, and ignores the nodes that
	are either not feasible or will not lead to a better solution. The best case for this function
	will approach nlogn, as the each node has two child nodes, meaning the tree will have a depth
	of logn, and the bound of each node must be calculated, which will take O(n).

CLASSES & INTERACTION
--> StSpTree
	This class is represents a Binary Space State Tree comprised of Nodes, with a starting "root"
	node. Private variables keep track of the capacity, the best profit,
	and the number of items, nodes, and leaves. Vectors store all items and the optimal solution.
	The constructor creates a vector of Item objects, which are built from each line of the input,
	and sorts them by the ratio of profit to weight. The bestFirstBandB() function calculates
	the max profit, and keeps track of the node that represents the optimal solution.
	The optimalSolution() function builds the optimal solution vector which stores the subset of
	items that produces the max profit by backtracking through the tree to the root.
	Lastly, the writeToFile() function organizes the information of the StSpTree class and
	writes it line by line to the output file.
--> Node
	This nested class represents each state of the problem, and stores the level, weight, profit,
	bound, and pointers to the yes child, no child, and parent node. The compareNodePtr struct is
	used to order the Node pointers by the value of their bound.
--> Item
	This nested class represents each item that can be put in the knapsack, and stores each item's
	weight and profit.
