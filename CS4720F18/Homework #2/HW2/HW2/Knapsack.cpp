#include <iostream>		//cout
#include <fstream>		//ofstream
#include <bitset>		//bitset
#include <vector>		//vector
#include <chrono>		//chrono

using namespace std;

#define MAX_ITEMS 15;	//max number of items possible
const int CURRENT_MAX = 15;	//max items for current run
const int MAX_WEIGHT = 10000;	//max weight/capacity of knapsack

//output file for results
ofstream outFileExhaustive("Results - Exhaustive.txt");
ofstream outFileGreedy("Results - Greedy.txt");

void exhaustiveKS(const int maxWeight, const vector <int> values, const vector <int> weights);
void greedyKS(const int maxWeight, const vector <int> values, const vector <int> weights);
int nextHighestIndx(const vector <int> values, int highVal);

//Exhaustive (Brute Force) Algorithm for Knapsack Problem
//Picks best value and weight combination of items
void exhaustiveKS(const int maxWeight, const vector <int> values, const vector <int> weights)
{
	int bestVal = 0, bestWeight = 0, bestComb = 0;	//best value, best combination
	int numItems = values.size();	//number of items
	int maxPermutations = (int)pow(2, numItems);	//number of possible permutations
	int currVal, currWeight;	//variables for testing specific combination
	vector<int> knapsack;		//knapsack of indices of included items

	//iterate through each possible permutation
	for (int permutation = 0; permutation < maxPermutations - 1; permutation++)
	{
		currVal = 0;
		currWeight = 0;
		
		//iterate through specific permutation
		for (int item = 0; item < numItems - 1; item++)
		{
			//shift "i" by "j" and isolate bit to see if included
			if (((permutation >> item) & 1) != 1)
			{
				continue;
			}

			currVal = currVal + values.at(item);
			currWeight = currWeight + weights.at(item);
		}

		//if current permutation is better and satisfies weight constraint
		if ((currVal > bestVal) && (currWeight <= maxWeight))
		{
			bestVal = currVal;
			bestWeight = currWeight;
			bestComb = permutation;
		}
	}

	for (int i = 0; i<CURRENT_MAX; i++)
	{
		//if item included, then mark its index
		if (((bestComb >> i) & 1) == 1)
			knapsack.push_back(i);
	}
	
	//turn permutation number to bits (0's and 1's of items)
	bitset<15> include(bestComb);

	//display results
	cout << "Exhaustive: n = " << CURRENT_MAX << endl;
	outFileExhaustive << CURRENT_MAX << endl;

	cout << "The indices of items included are: ";// << include << endl;
	for (unsigned int i = 0; i < knapsack.size(); i++)
	{
		cout << knapsack.at(i) << " " << endl;
		outFileExhaustive << knapsack.at(i) << " ";
	}
	cout << "\nThe value is: " << bestVal << "." << endl;
	cout << "The weight is: " << bestWeight << "." << endl;

	
	//outFileExhaustive << include << endl;
	outFileExhaustive << endl << bestVal << endl;
	outFileExhaustive << bestWeight << endl;


}

//Greedy Algorithm for Knapsack Problem
//Picks next highest valued item until capacity reached
void greedyKS(const int maxWeight, const vector <int> values, const vector <int> weights)
{
	int currVal = 0, currWeight = 0;	//variables for keeping running count
	int numVisited = 0;		//keep track of how many items
	int highIndx = 0;	//index of highest value, start at first element
	int highVal = -1;	//current highest value 
	int numItems = values.size();
	int tempWeight;		//hold possible weight to add
	vector<int> knapsack;	//knapsack containing indices of items picked

	//keep adding items if capacity not reached and still have items to add
	while ((currWeight < maxWeight) && (numVisited < numItems))
	{
		//get next highest value based on current highest value
		highIndx = nextHighestIndx(values, highVal);
		highVal = values.at(highIndx);

		//temporarily add the weight to the current weight
		tempWeight = currWeight + weights.at(highIndx);

		//if adding the item doesn't exceed capacity, add it to knapsack
		if (tempWeight <= maxWeight)
		{
			currVal = currVal + values.at(highIndx);
			currWeight = currWeight + weights.at(highIndx);

			knapsack.push_back(highIndx);
		}

		//increment number of items processed
		numVisited = numVisited + 1;
	}

	//dispaly results
	cout << "Greedy: n = " << CURRENT_MAX << endl;
	outFileGreedy << CURRENT_MAX << endl;

	cout << "The items included are: ";
	for (unsigned int i = 0; i < knapsack.size(); i++)
	{
		cout << knapsack.at(i);
		outFileGreedy << knapsack.at(i) << " ";
	}

	cout << "\nThe value is: " << currVal << "." << endl;
	outFileGreedy << endl << currVal << endl;

	cout << "The weight is: " << currWeight << "." << endl;
	outFileGreedy << currWeight << endl;
}

//Returns the index of the next highest value in an array
int nextHighestIndx(const vector <int> values, int highVal)
{
	//set current highest to first elem
	int currHigh = highVal, prevHigh = highVal;		//values used for testing next highest
	int nextHighIndx = 0;	//index of next highest value

	//iterate through all values and find next highest
	if (prevHigh == -1)
	{
		//find highest overall
		currHigh = values.at(0);
		for (unsigned int j = 0; j < values.size(); j++)
		{
			if (values.at(j) > currHigh)
			{
				currHigh = values.at(j);
				nextHighIndx = j;
			}
		}
	}
	else
	{
		unsigned int i = 0;
		bool match = false;

		while (prevHigh > 0 && !match)
		{
			while (prevHigh - 1 != values.at(i) && i < CURRENT_MAX - 1 && !match)
			{
				i = i + 1;
			}
			if (prevHigh - 1 == values.at(i))
			{
				nextHighIndx = i;
				match = true;
			}

			if (!match)
			{
				prevHigh = prevHigh - 1;
				i = 0;
			}
		}
	}

	return nextHighIndx;
}


//Runs exhaustive knapsack algorithm
void runExhaustiveKS(vector<int> values, vector<int> weights)
{
	//start time for how long it takes
	chrono::steady_clock::time_point begin = chrono::steady_clock::now();

	//run exhaustive
	exhaustiveKS(MAX_WEIGHT, values, weights);

	//stop time 
	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	//give completion time in approximate seconds
	cout << "Time  (sec) = " << (chrono::duration_cast<chrono::microseconds>(end - begin).count()) / 1000000.0 << endl << endl;

	outFileExhaustive << (chrono::duration_cast<chrono::microseconds>(end - begin).count()) / 1000000.0 << endl << endl;
}

//Runs greedy knapsack algorithm
void runGreedyKS(vector<int> values, vector<int> weights)
{
	//start time for how long it takes
	chrono::steady_clock::time_point begin = chrono::steady_clock::now();

	//run exhaustive
	greedyKS(MAX_WEIGHT, values, weights);

	//stop time 
	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	//give completion time in approximate seconds
	cout << "Time  (sec) = " << (chrono::duration_cast<chrono::microseconds>(end - begin).count()) / 1000000.0 << endl << endl;

	outFileGreedy << (chrono::duration_cast<chrono::microseconds>(end - begin).count()) / 1000000.0 << endl << endl;
}

//Makes inputs of value for running algorithm
vector<int> makeValueArrray()
{
	vector<int> values;

	for (int i = 0; i < CURRENT_MAX; i++)
	{
		//create random value between 1 and 100
		int randVal = rand() % 100 + 1;
		values.push_back(randVal);
	}

	return values;
}

//Makes inputs of weight for running algorithm
vector<int> makeWeightArray()
{
	vector<int> weights;

	for (int i = 0; i < CURRENT_MAX; i++)
	{
		//create random weight between 1 and 10,000
		int randVal = rand() % 10000 + 1;
		weights.push_back(randVal);
	}

	return weights;
}

int main()
{
	vector<vector<int>> values;
	vector<vector<int>> weights;

	for (int i = 0; i < 5; i++)
	{
		values.push_back(makeValueArrray());
		weights.push_back(makeWeightArray());
	}

	for (int i = 0; i < 5; i++)
	{
		//runExhaustiveKS(values.at(i), weights.at(i));
		runGreedyKS(values.at(i), weights.at(i));
	}

	outFileExhaustive.close();
	outFileGreedy.close();

	return 0;
}