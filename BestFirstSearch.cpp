#include "StSpTree.h"

using namespace std;

/*main function parses the input text file and uses its information to build a state space tree
  that will determine the max profit that can be made by selecting a subset of items, subject
  to the constraint that the total weight of the items must be less than a certain capacity.
  int argc: number of command arguments. Must be exactly 3: ./BestFirstSearch, argv[1], argv[2]
  char* argv[1]: the title of the input file, which must contain the structure delineated in the
                 program assignment overview
  char* argv[2]: the title of the output file, which may or may not already exist. if it already
                 exists, it will be overwritten. Otherwise a file named argv[2] will be created 
				 and written to.
 */
int main(int argc, char** argv){
	if (argc != 3) return -1;
	
	char * in = argv[1]; //the input text file's name
	ifstream input(in); //an ifstream that reads from the file named "in"
	double n; //number of items
	double c; //capacity of the knapsack
	vector<string> items; //vector of strings that represent each item

	string line; //each line of the input text file
	int i = 0; //counter variable
	while (input >> line){
		if (i == 0){
			//Substring of the first line, containing the number of items
			string nStr = line.substr(0, line.find(","));
			n = stoi(nStr);
			//Substring of the first line, containing the capacity of the knapsack
			string cStr = line.substr(line.find(",") + 1,-1);
			c = stoi(cStr);
		}
		else{
			items.push_back(line);
		}
		i++;
	}

	//StateSpaceTree object that will create model this problem and output an answer.
	StSpTree *tree = new StSpTree(n, c, items);
	tree->bestFirstBandB();
	tree->writeToFile(argv[2]);
}
