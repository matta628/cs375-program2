#include "StSpTree.h"

using namespace std;

StSpTree::StSpTree(int n, int c, vector<string> & list){
	numItems = n;
	numNodes = 0;
	numLeaves = 0;
	capacity = c;

	vector<Item*> temp;
	for (int i = 0; i < n; i++){
		int comma = list[i].find(",");
		int weight = stoi(list[i].substr(0, comma));
		int profit = stoi(list[i].substr(comma+1, -1));
		Item *item = new Item(weight, profit);
		temp.push_back(item);
	}
	sort(temp.begin(), temp.end(),
		 [](Item* a, Item* b){
			 return (a->profit/a->weight) > (b->profit/b->weight);
		 });
	items.push_back(new Item(0, 0));
	for (int i = 0; i < (int)temp.size(); i++){
		items.push_back(temp[i]);
	}

	root = new Node(nullptr, 0, 0, 0);
}

double StSpTree::bound(Node *node){
	double bound = node->profit;
	double weightSoFar = node->weight;
	if (weightSoFar > capacity) return -1;
	int i = node->level + 1;
	vector<double> x (numItems + 1);
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

void StSpTree::bestFirstBandB(){
	priority_queue<Node*, vector<Node*>, compareNodePtr> pq;
	Node * start = root;
	pq.push(start);
	numNodes++;
	best = 0;
	Node * bestNode = nullptr;
	while (!pq.empty()){
		Node* v = pq.top();
		pq.pop();
		v->bound = bound(v);
		int uLevel = v->level + 1;
		if (v->bound > best && uLevel <= numItems && v->weight < capacity){
			//yes chile...
			double uWeight = v->weight + items[uLevel]->weight;
			double uProfit = v->profit + items[uLevel]->profit;
			Node * u = new Node(v, uLevel, uWeight, uProfit);
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
			//no chile...
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

void StSpTree::optimalSolution(Node *node){
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

void StSpTree::writeToFile(char * out){
	ofstream output(out, ios::out | ios::trunc);
	output << numItems << "," << best << "," << optimal.size() << endl;
	output << numNodes << "," << numLeaves << endl;
	for (Item* i : optimal){
		output << i->weight << "," << i->profit << endl;
	}
	output.close();
}

void StSpTree::prettyPrint(Node *node, int i){
	if (node->no != nullptr)
		prettyPrint(node->no, i+1);
	for (int j = 0; j < i; j++)
		cout << "            ";
	cout << node->profit << "," << node->weight << "," << node->bound << endl;
	if (node->yes != nullptr)
		prettyPrint(node->yes, i+1);
}
