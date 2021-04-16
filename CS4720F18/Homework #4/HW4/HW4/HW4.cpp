/*
Daniel Frey
CS 4720-001
HW#4
11/15/18
This program implements the Edit Distance algorithm, as well as Greedy/DP algorithms for Coin Change
*/

#include <iostream>		//cout
#include <string>		//cout string
#include <fstream>		//ofstream
#include <algorithm>	//min
#include <vector>		//vector

using namespace std;

int editDistanceDP(string str1, string str2);
int coinChangeDP(const vector<int>& denom, int n);
int coinChangeGreedy(const vector<int>& denom, int n);
void runDPGreedyComp(string fileName, const vector<int>& denom);

//Edit distance problem using a dynamic programming algorithm
//Determines the number of letters that must be changed, inserted, or deleted to make two strings match
int editDistanceDP(string str1, string str2)
{
	int m = str1.size(), n = str2.size();	//sizes of strings
	vector<vector<int>> result(m + 1, vector<int>(n + 1, 0));		//2d array to calculate result

	//loop through size of str1
	for (int i = 0; i <= m; i++)
	{
		//loop through size of str2
		for (int j = 0; j <= n; j++)
		{
			//if first string empty, use all chars in second
			if (i == 0)
			{
				result[i][j] = j;
			}
			//if second string empty, use all chars in first
			else if (j == 0)
			{
				result[i][j] = i;
			}
			//if previous chars are same, just continue from last point
			else if (str1[i - 1] == str2[j - 1])
			{
				result[i][j] = result[i - 1][j - 1];
			}
			//if prev chars different, then find min of change, insert, delete
			else
			{
				result[i][j] = 1 + min({ result[i - 1][j - 1], result[i][j - 1], result[i - 1][j] });
			}
		}
	}

	return result[m][n];
}

//Change making problem using a dynamic programming algorithm
//Makes change for given amount(n) using given denominations(denom)
int coinChangeDP(const vector<int>& denom, int n)
{
	int denomSize = denom.size();	//loop control
	vector<int> result(n + 1);	//"array" for calculating result, initialized to all 0's
	int infinity = INT_MAX - 1;	//value for computations
	int temp;	//variable for holding current calculation
	int j;		//variable for loop control

	//initialize first value to 0
	//result.at(0) = 0;

	//loop through values to make change for
	for (int i = 1; i <= n; i++)
	{
		temp = infinity;
		j = 0;
		
		//while still denominations and still able to use current denomination
		while ((j < denomSize) && (i >= denom.at(j)))
		{
			temp = min(result[i - denom.at(j)], temp);
			j = j + 1;
		}

		//add min number coins with current coin that is being added
		result.at(i) = temp + 1;
	}

	return result.back();
}

//Change making problem using a greedy algorithm
//Makes change for amount(n) using given denominations(denom)
int coinChangeGreedy(const vector<int>& denom, int n)
{
	int numCoins = 0;	//minimum number of coins
	int m = denom.size();	//number of denominations

	//loop through denominations to make change for
	for (int i = m - 1; i >= 0; i--)
	{
		//if denomination can be used to make change
		if (n / denom.at(i) >= 1)
		{
			//denom.at(i): coin value of specific denomination
			//n / denom.at(i): how many times denomination can be used to make change for value n
			//(n/denom.at(i)) * denom.at(i): value of number of coins used

			//update number of coins, previous count + number of coins used for current denomination
			numCoins = numCoins + (n / denom.at(i));

			//update value to make change for
			n = n - ((n / denom.at(i)) * denom.at(i));

		}

	}

	return numCoins;
}

//Runs DP and Greedy for coin change and outputs dp/g to file
void runDPGreedyComp(string fileName, const vector<int>& denom)
{
	ofstream outFile(fileName);

	//loop through different values to make change for
	for (int i = 1; i <= 1000; i++)
	{
		//calculate competitive ratio
		float dpG = (float)coinChangeDP(denom, i) / coinChangeGreedy(denom, i);

		outFile << dpG << endl;
	}

	cout << '\n' << fileName << " output to file." << endl;

	outFile.close();
}

int main()
{
	//run edit distance 
	cout << "Compute, Commute: " << editDistanceDP("Compute", "Commute") << endl;
	cout << "Saturday, Sunday: " << editDistanceDP("Saturday", "Sunday") << endl;
	cout << "Cat, Hat: " << editDistanceDP("Cat", "Hat") << endl;
	cout << "Dark, Moon: " << editDistanceDP("Dark", "Moon") << endl;
	cout << "Greedy, Dynamic: " << editDistanceDP("Greedy", "Dynamic") << endl << endl;
	
	//run dp and greedy coin change comparison
	runDPGreedyComp("DP vs. Greedy [1 5 6].txt", vector<int> {1, 5, 6});
	runDPGreedyComp("DP vs. Greedy [1 10 15].txt", vector<int> {1, 10, 15});
	runDPGreedyComp("DP vs. Greedy [1 30 50].txt", vector<int> {1, 30, 50});	
	
	return 0;
}