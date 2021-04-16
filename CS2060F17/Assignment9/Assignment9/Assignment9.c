/*
Daniel Frey
CS2060_002
Assignment 9
11-9-17
This program will compare entries from 3 files and write common entries to a new file

Pseudocode:
Define constants for max file lines, max file line size, and file paths/names
Main
	Start processessing the three files
Function: Start processessing files
	Declare variables for the three file pointers, array for each file, and counter for elements for each
	Open the files
	If files can be opened
		Number of elements for each equals function: Populate arrays with file info
		Sort the arrays
		Compare the entries
		Write repeat array to file
		Close files
	Else
		Files cannot be opened, quit
Function: Populate array with file info
	Declare variables for array counter, and file line
	While not EOF
		Array at index equals customer ID from file
		Increment index counter
	Return index count
Function: Sort array
	Quick sort the arrays
Function: Compare entries
	Declare variables for each array count
	While current count is less than total number of array elements
		Check for duplicates
		Find largest number
		If all entries are the same
			Write to repeat array
			Increment counters for each array
	Number of repeat elements equals repeat count
Function: Check for duplicates
	If count is more than 0 and not reached max elements and current element is equal to previous element
		Increment the index count
Function: Find largest
	If element is higher than the other two elements
		Increment the smaller elements
	If two elements equal and higher than the third element
		Increment the third element
Function: Increment smaller element
	If element 1 is greater than element 2 and element 2 count is less than max number element 2
		Increment the smaller element
Function: Write repeat customer list to file
	Declare variable for repeat file
	For each repeat array line
		Write repeat array element to file
	Close file
	

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//max number of lines in a fille
#define MAX_LINES 100

//max size of individual line
#define MAX_LINE_SIZE 10

#pragma warning(disable:4996)

void startProcessesingFiles();
int populateArrayFileData(FILE *filePtr, int * const dayArray);
int cmpfunc(const void * val1, const void * val2);
void sortArray(int * const dayArray, int numElements);
void compareEntries(const int * const day1Array, int day1NumElmnts,
	const int * const day2Array, int day2NumElmnts,
	const int * const day3Array, int day3NumElmnts,
	int * const repeatArray, int * const repeatNumElmnts);
void checkDuplicates(const int * const dayArray, int numElmnts, size_t * const count);
void findLargest(const int * const day1Array, int day1NumElmnts, int * const day1Count,
	const int * const day2Array, int day2NumElmnts, int * const day2Count,
	const int * const day3Array, int day3NumElmnts, int * const day3Count);
void incrementSmaller(const int * const dayArray1, int * const dayCount1,
	const int * const dayArray2, int dayNumElmnts2, int * const dayCount2);
void writeRepeatList(const int * const returnArray, const int returnSize);

//file names
const char DAY1_FILE[] = "C:/Nov22.txt";
const char DAY2_FILE[] = "C:/Nov23.txt";
const char DAY3_FILE[] = "C:/Nov24.txt";
const char REPEAT_CUSTOMER_FILE[] = "RepeatCustomers.txt";

int main(void)
{
	startProcessesingFiles();

	return 0;
}

//starts the processing of daily log files and calls other functions to complete
void startProcessesingFiles()
{
	FILE *fileDay1Ptr, *fileDay2Ptr, *fileDay3Ptr;
	int day1Array[MAX_LINES] = { 0 }, day2Array[MAX_LINES] = { 0 }, day3Array[MAX_LINES] = { 0 }, repeatArray[MAX_LINES] = { 0 },
		day1NumElmnts = 0, day2NumElmnts = 0, day3NumElmnts = 0, repeatNumElmnts = 0;

	//link files to pointers
	fileDay1Ptr = fopen(DAY1_FILE, "r");
	fileDay2Ptr = fopen(DAY2_FILE, "r");
	fileDay3Ptr = fopen(DAY3_FILE, "r");

	//if files exit, else files DNE
	if ((fileDay1Ptr != NULL) && (fileDay2Ptr != NULL) && (fileDay3Ptr != NULL))
	{
		//fill arrays with info from files
		day1NumElmnts = populateArrayFileData(fileDay1Ptr, day1Array);
		day2NumElmnts = populateArrayFileData(fileDay2Ptr, day2Array);
		day3NumElmnts = populateArrayFileData(fileDay3Ptr, day3Array);

		//sort the arrays to make matching and dealing with duplicates easier
		sortArray(day1Array, day1NumElmnts);
		sortArray(day2Array, day2NumElmnts);
		sortArray(day3Array, day3NumElmnts);

		//compare the elements of arrays
		compareEntries(day1Array, day1NumElmnts, 
			day2Array, day2NumElmnts, 
			day3Array, day3NumElmnts, 
			repeatArray, &repeatNumElmnts);

		writeRepeatList(repeatArray, repeatNumElmnts);

		// Close the files
		fclose(fileDay1Ptr);
		fclose(fileDay2Ptr);
		fclose(fileDay3Ptr);
	}
	else
	{
		puts("At least one file could not be opened. Cannot continue.");
	}
}

//populate day array with info from the day file
//returns the number of elements added to array
int populateArrayFileData(FILE *filePtr, int * const dayArray)
{
	size_t count = 0;
	char customerIDStr[MAX_LINE_SIZE];

	//loop through the file until end, adding custID to array each iteration
	while (fgets(customerIDStr, MAX_LINE_SIZE, filePtr) != NULL)
	{
		//convert string to integer and add that to specific day array of custID's
		dayArray[count] = atoi(customerIDStr);

		//increment index for array
		count++;
	}

	return count;
}

//comparator function for use with qsort function
//returns negative if val2 > val1 and positive if val1 > val2
int cmpfunc(const void * val1, const void * val2)
{
	return (*(int*)val1 - *(int*)val2);
}

//sort the array in ascending order to make comparing and dealing with duplicates easier
void sortArray(int * const dayArray, int numElements)
{
	//use quick sort to sort the array using the comparator function
	qsort(dayArray, numElements, sizeof(int), cmpfunc);
}

//compares 3 entries from each file
void compareEntries(const int * const day1Array, int day1NumElmnts,
	const int * const day2Array, int day2NumElmnts,
	const int * const day3Array, int day3NumElmnts,
	int * const repeatArray, int * const repeatNumElmnts)
{
	size_t repeatCount = 0, day1Count = 0, day2Count = 0, day3Count = 0;

	//find largest number
	//if one > others, get next others
	//if adding to repeat array, use it's own counter

	//while not out of bounds for each array
	while (day1Count < (size_t)day1NumElmnts && day2Count < (size_t)day2NumElmnts && day3Count < (size_t)day3NumElmnts)
	{
		//check for duplicate entries
		checkDuplicates(day1Array, day1NumElmnts, &day1Count);
		checkDuplicates(day2Array, day2NumElmnts, &day2Count);
		checkDuplicates(day3Array, day3NumElmnts, &day3Count);

		//find largest and increment count of others while lower
		findLargest(day1Array, day1NumElmnts, &day1Count, day2Array, day2NumElmnts, &day2Count, day3Array, day3NumElmnts, &day3Count);

		//if all 3 are the same, write to array and keep count
		if ((day1Array[day1Count] == day2Array[day2Count]) && (day1Array[day1Count] == day3Array[day3Count]))
		{
			//add match to repeat customer array
			repeatArray[repeatCount] = day1Array[day1Count];

			//increment repeat customer array index
			repeatCount++;
			day1Count++;
			day2Count++;
			day3Count++;
		}
	}
	
	*repeatNumElmnts = repeatCount;
}

//checks for duplicates and increments index for array
void checkDuplicates(const int * const dayArray, int numElmnts, size_t * const count)
{
	//while count is less than max elements-1 (-1 so next one not out of bounds), and current == prev
	while ((*count > 0) && (*count < (size_t)numElmnts - 1) && (dayArray[*count] == dayArray[*count - 1]))
	{
		//advance to next position, increment count
		(*count)++;
	}
}

//finds the largest of 3 numbers and calls increment smaller 
void findLargest(const int * const day1Array, int day1NumElmnts, int * const day1Count,
	const int * const day2Array, int day2NumElmnts, int * const day2Count,
	const int * const day3Array, int day3NumElmnts, int * const day3Count)
{
	//if day1 has the largest value
	if (day1Array[*day1Count] > day2Array[*day2Count] && day1Array[*day1Count] > day3Array[*day3Count])
	{
		//while val1>val2
		//increment index of val2
		//while val1>val3
		//increment index of val3
		incrementSmaller(day1Array, day1Count, day2Array, day2NumElmnts, day2Count);
		incrementSmaller(day1Array, day1Count, day3Array, day3NumElmnts, day3Count);
	}
	//else if day2 has the largest value
	else if (day2Array[*day2Count] > day1Array[*day1Count] && day2Array[*day2Count] > day3Array[*day3Count])
	{
		//while val2>val1
		//increment index of val1
		//while val2>val3
		//increment index of val3
		incrementSmaller(day2Array, day2Count, day1Array, day1NumElmnts, day1Count);
		incrementSmaller(day2Array, day2Count, day3Array, day3NumElmnts, day3Count);
	}
	//else if day3 must have the largest value
	else if (day3Array[*day3Count] > day1Array[*day1Count] && day3Array[*day3Count] > day2Array[*day2Count])
	{
		//while val3>val1
		//increment index of val1
		//while val3>val2
		//increment index of val2
		incrementSmaller(day3Array, day3Count, day1Array, day1NumElmnts, day1Count);
		incrementSmaller(day3Array, day3Count, day2Array, day2NumElmnts, day2Count);
	}
	//else if day1=day2, increment day3
	else if (day1Array[*day1Count] == day2Array[*day2Count] && day1Array[*day1Count] > day3Array[*day3Count])
	{
		incrementSmaller(day1Array, day1Count, day3Array, day3NumElmnts, day3Count);
	}
	//else if day1=day3, increment day2
	else if (day1Array[*day1Count] == day3Array[*day3Count] && day1Array[*day1Count] > day2Array[*day2Count])
	{
		incrementSmaller(day1Array, day1Count, day2Array, day2NumElmnts, day2Count);
	}
	//else must be day2=day3, increment day1
	else
	{
		incrementSmaller(day2Array, day2Count, day1Array, day1NumElmnts, day1Count);
	}
}

//increments the smaller value index until largest after finding the largest
void incrementSmaller(const int * const dayArray1, int * const dayCount1, 
	const int * const dayArray2, int dayNumElmnts2, int * const dayCount2)
{
	//while val1>val2
	//increment index of val2
	while ((dayArray1[*dayCount1] > dayArray2[*dayCount2]) && (*dayCount2 < dayNumElmnts2))
	{
		(*dayCount2)++;
	}
}

//writes the returning customerID's to file
void writeRepeatList(const int * const repeatArray, const int repeatNumElmnts)
{
	FILE *fileRepeatCustPtr = fopen(REPEAT_CUSTOMER_FILE, "w");

	//loop through array writing each customer ID to file
	for (size_t i = 0; i < (unsigned)repeatNumElmnts; i++)
	{
		fprintf(fileRepeatCustPtr, "%d\n", *(repeatArray + i));
	}

	printf("Repeat customers has been written to %s in ascending order.\n", REPEAT_CUSTOMER_FILE);

	fclose(fileRepeatCustPtr);
}