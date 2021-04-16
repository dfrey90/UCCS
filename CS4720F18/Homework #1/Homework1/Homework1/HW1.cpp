#include <iostream>		//cout
#include <vector>		//vector
#include <random>		//rand_engine, uni dist
#include <algorithm>	//sort
#include <chrono>		//chrono
#include <fstream>		//oftream

using namespace std;

const int MAX_LENGTH = 100000;	//constant for max array size
const int STEP_SIZE = 5000;		//constant for step size for array

vector<vector <int>> createRandArr();
void sortArrays(vector<vector <int>>& arrArrs);
void binSearchArray(vector<vector <int>> arrArrs, ofstream& outFile);
void ternSearchArray(vector<vector <int>> arrArrs, ofstream& outFile);
int binarySearch(vector<int> arr, int key, int lower, int upper);
int ternarySearch(vector<int>, int key, int lower, int upper);

//Creates an array of arrays using vectors filled with random numbers
vector<vector <int>> createRandArr()
{
	vector<vector <int>> arrArrs((MAX_LENGTH / STEP_SIZE) + 1);		//array of arrays, +1 to get total num arrays
	int arrNum = 0;		//array number, 0 - 20 (max length / step = 20)
	default_random_engine rng(random_device{}());		//set random seed for generating numbers
	uniform_int_distribution<int> gen(0, MAX_LENGTH);	//generate numbers from 0 to max length

	//create arrays from 0 to 100,000 in steps of 5,000
	for (int i = 0; i < MAX_LENGTH + 1; i = i + STEP_SIZE)
	{
		//create varying lengths of arrays within the array
		arrArrs[arrNum].resize(i);
		
		//for each array within the array, make elements random variables
		for (int j = 0; j < i; j++)
		{
			//fill array with numbers between 0 and 100,000
			arrArrs[arrNum][j] = gen(rng);
		}

		//increment to next array spot
		arrNum++;
	}

	return arrArrs;
}

//Sorts the arrays in the array
void sortArrays(vector<vector <int>>& arrArrs)
{
	//for each array in the array of arrays, sort
	for (vector<int>& arr : arrArrs)
	{
		sort(arr.begin(), arr.end());
	}
}

//Calls Binary Search to search array for randomly generated number and calculate time
void binSearchArray(vector<vector <int>> arrArrs, ofstream& outFile)
{
	default_random_engine rng(random_device{}());		//set random seed for generating numbers
	uniform_int_distribution<int> gen(0, MAX_LENGTH);	//generate numbers from 0 to max length

	//output data to file
	outFile << "Binary Search" << endl;

	//iterate through all arrays counting time to binary search
	for (int i = 0; i < (MAX_LENGTH / STEP_SIZE) + 1; i++)
	{
		int lower = 0, upper = arrArrs[i].size();

		//start time for how long it takes to find a random element
		chrono::steady_clock::time_point begin = chrono::steady_clock::now();

		//display the results of binary search
		cout << binarySearch(arrArrs[i], gen(rng), lower, upper) << endl;

		//stop time 
		chrono::steady_clock::time_point end = chrono::steady_clock::now();

		//give completion time in approximate seconds
		cout << "Time difference (sec) = " << (chrono::duration_cast<chrono::microseconds>(end - begin).count()) / 1000000.0;
		cout << " for Binary Search of array size " << arrArrs[i].size() << endl << endl;

		//output data to file
		outFile << (chrono::duration_cast<chrono::microseconds>(end - begin).count()) / 1000000.0 << endl;
	}
}

//Calls Ternary Search to search array for randomly generated number and calculate time
void ternSearchArray(vector<vector <int>> arrArrs, ofstream& outFile)
{
	default_random_engine rng(random_device{}());		//set random seed for generating numbers
	uniform_int_distribution<int> gen(0, MAX_LENGTH);	//generate numbers from 0 to max length

	//output data to file
	outFile << "Ternary Search" << endl;

	//iterate through all arrays counting time to binary search
	for (int i = 0; i < (MAX_LENGTH / STEP_SIZE) + 1; i++)
	{
		int lower = 0, upper = arrArrs[i].size();

		//start time for how long it takes to find a random element
		chrono::steady_clock::time_point begin = chrono::steady_clock::now();

		//display the results of ternary search
		cout << ternarySearch(arrArrs[i], gen(rng), lower, upper) << endl;

		//stop time 
		chrono::steady_clock::time_point end = chrono::steady_clock::now();

		//give completion time in approximate seconds
		cout << "Time difference (sec) = " << (chrono::duration_cast<chrono::microseconds>(end - begin).count()) / 1000000.0;
		cout << " for Ternary Search of array size " << arrArrs[i].size() << endl << endl;

		//output data to file
		outFile << (chrono::duration_cast<chrono::microseconds>(end - begin).count()) / 1000000.0 << endl;
	}
}

//Binary search function to search an array for a key, returns index of key or -1 if not found
int binarySearch(vector<int> arr, int key, int lower, int upper)
{
	int mid;	//middle variable

	//if array is acceptable size to search
	if ((upper >= lower) && (arr.size() > 0))
	{
		//find middle of array
		mid = lower + (upper - lower) / 2;

		//if key is at mid, return index
		if (key == arr[mid])
			return mid;
		//if key is less than mid, search lower half
		else if (key < arr[mid])
			return binarySearch(arr, key, lower, mid - 1);
		//else search upper half, key > arr[mid]
		else
			return binarySearch(arr, key, mid + 1, upper);
	}
	else
		//key is not found in array
		return -1;
}

//Ternary search function to search an array for a key, returns index of key or -1 if not found
int ternarySearch(vector<int> arr, int key, int lower, int upper)
{
	int lMid, uMid;		//middle variables

	//if array is acceptable size to search
	if ((upper >= lower) && (arr.size() > 0))
	{
		//find middles of upper and lower halves
		lMid = lower + (upper - lower) / 3;
		uMid = lMid + (upper - lower) / 3;

		//if key is at either middle, return index
		if (key == arr[lMid])
			return lMid;
		else if (key == arr[uMid])
			return uMid;
		//else if key is in lower third, search lower third
		else if (key < arr[lMid])
			return ternarySearch(arr, key, lower, lMid - 1);
		//else if key is in upper third, search upper third
		else if (key > arr[uMid])
			return ternarySearch(arr, key, uMid + 1, upper);
		//else key must be in middle third, search middle third
		else
			return ternarySearch(arr, key, lMid + 1, uMid - 1);
	}
	else
		//key not found in array
		return -1;
}

int main()
{	
	ofstream outFile;	//file for collecting results

	//create array of arrays to pass around to functions
	vector <vector <int>> arrArrs = createRandArr();

	outFile.open("Search Times.txt");

	//run functions
	sortArrays(arrArrs);
	binSearchArray(arrArrs, outFile);
	ternSearchArray(arrArrs, outFile);

	outFile.close();

	return 0;
}