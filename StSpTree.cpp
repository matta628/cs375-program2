#include "StSpTree.h"

using namespace std;

/*
  Constructor for StSpTree Class. 
  n is the number of items
  c is the capacity of the knapsack 
  list is a vector, size n, of strings that contains the weight,profit of each item. 
  The constructor creates an Item object from each line, and stores them in the vector
  which then gets sorted based on the profit/weight ratio.
 */
StSpTree::StSpTree(int n, double c, vector<string> & list){
	numItems = n;
	numNodes = 0;
	numLeaves = 0;
	capacity = c;

	vector<Item*> temp; //temporary vector holding the items as they're parsed from the input file
	for (int i = 0; i < n; i++){
		int comma = list[i].find(","); //index of the comma in each line
		int weight = stoi(list[i].substr(0, comma)); //the weight of the item
		int profit = stoi(list[i].substr(comma+1, -1)); //the profit of the item
		Item *item = new Item(weight, profit); //the item object
		temp.push_back(item);
	}
	sort(temp.begin(), temp.end(),
		 [](Item* a, Item* b){
			 return (a->profit/a->weight) > (b->profit/b->weight);
		 });//lambda expression to sort the temp vector of items
	items.push_back(new Item(0, 0));
	for (int i = 0; i < (int)temp.size(); i++){
		items.push_back(temp[i]);
	}

	root = new Node(nullptr, 0, 0, 0);
}


/*
  node is a node of the state space tree that has been initialized with a profit, weight, and level
  this function calculates the upperbound, or the maximum amount of possible profit for each node.
  if the node's current weight exceeds the capacity, it returns -1
 */
double StSpTree::bound(Node *node){
	double bound = node->profit; // the bound initialized to the current node's profit
	double weightSoFar = node->weight; // keeps track of the weight so far
	if (weightSoFar > capacity) return -1;
	//index for the items, starting at i+1 because 1 thru i have already been considered
	int i = node->level + 1;
	vector<double> x (numItems + 1); //vector to keep track of items selected 
	for (int j = i; j <= numItems; j++)
		x[j] = 0;
	while (weightSoFar < capacity && i <= numItems){
		if (weightSoFar + items[i]->weight <= capacity){
			x[i] = 1;
			weightSoFar += items[i]->weight;
			bound += items[i]->profit;
		}
		else{
			x[i] = (capacity - weightSoFar) / items[i]->weight;
			weightSoFar = capacity;
			bound += items[i]->profit * x[i];
		}
		i++;
	}
	return bound;
}
/*
  This function initializes a priority queue that uses compareNodePtr, a comparator struct that
  orders Node pointers by their bounds. The top Node* will have the greatest bound in the PQ.
  Starting with the root node, this function works on each node pointer until the PQ is empty.
  if the top node of the PQ has a higher bound then the current best, then it is expanded and
  two child nodes are created, one which adds the next item (yes) and one that doesn't (no).
  if the bound of either child node is greater than the current best, then it is added to the 
  priority queue. In addition to keeping track of the max profit (best), the node which
  represents the optimal solution state is tracked too (bestNode). lastly, this function passes
  the bestNode to the optimalSolution() function.
 */
void StSpTree::bestFirstBandB(){
	//priority queue to hold nodes, ordered by their bounds
	priority_queue<Node*, vector<Node*>, compareNodePtr> pq; 
	pq.push(root);
	numNodes++;
	best = 0;
	Node * bestNode = nullptr; //keeps track of the node that represents the optimal solution
	while (!pq.empty()){
		Node* v = pq.top(); //the node w the greatest bound in the priority queue
		pq.pop();
		v->bound = bound(v);
		int uLevel = v->level + 1; //the level of the v's child nodes
		if (v->bound > best && uLevel <= numItems && v->weight < capacity){
			double uWeight = v->weight + items[uLevel]->weight; //the weight of v's yes child node
			double uProfit = v->profit + items[uLevel]->profit; //the profit of v's yes child node
			Node * u = new Node(v, uLevel, uWeight, uProfit); // v's yes child node
			u->bound = bound(u);
			v->yes = u;
			if (u->weight < capacity && u->profit > best){
				best = u->profit;
				bestNode = u;
			}
			if(u->bound > best){
				pq.push(u);
			}
			else{
				numLeaves++;
			}
			u = new Node(v, uLevel, v->weight, v->profit);
			u->bound = bound(u);
			v->no = u;
			if (u->bound > best){
				pq.push(u);
			}
			else{
				numLeaves++;
			}
			numNodes += 2;
		}
		else{
			numLeaves++;
		}
	}
	optimalSolution(bestNode);
}

/*
  node is the node pointer that represents the optimal solution state in the state space tree
  this function creates a vector that stores the subset of items that represents the optimal solution
  it starts at the end node, and backtracks through the tree, adding any item whos index is equal to
  the depth of the yes child nodes. to put them in the correct order, the vector is reversed.
 */
void StSpTree::optimalSolution(Node *node){
	//temporary vector to hold the items belonging to the subset of optimal solutions, which are
	//added in reverse order
	vector<Item*> temp; 
	while (node->parent != nullptr){
		if (node->parent->yes == node){
			temp.push_back(items[node->level]);
		}
		node = node->parent;
	}
	reverse(temp.begin(), temp.end());
	optimal = temp;
}

/*
  out is the name of the output text file which will either be created if it doesn't exist, or
  overwritten if it does
  The first line of the file is the total number of items, followed by the number of items in the
  optimal subset 
  The second line is the total number of nodes visited, followed by the number of leaves visited
  the last lines are items in the optimal subset in the order theyre selected, in weight,profit form
 */
void StSpTree::writeToFile(char * out){
	//ofstream will write to a file named by the variable out
	ofstream output(out, ios::out | ios::trunc); 
	output << numItems << "," << best << "," << optimal.size() << endl;
	output << numNodes << "," << numLeaves << endl;
	for (Item* i : optimal){
		output << i->weight << "," << i->profit << endl;
	}
	output.close();
}

/*
  node is a valid node in the state space tree, and i is the level/depth of the node
  this function is used for debugging purposes, in order to illustrate the nodes of the
  state space tree. it traverses through the tree "in order" and illustrates depth
  by adding increasingly large whitespace.
 */
void StSpTree::prettyPrint(Node *node, int i){
	if (node->no != nullptr)
		prettyPrint(node->no, i+1);
	for (int j = 0; j < i; j++) 
		cout << "            ";
	cout << node->profit << "," << node->weight << "," << node->bound << endl;
	if (node->yes != nullptr)
		prettyPrint(node->yes, i+1);
}
