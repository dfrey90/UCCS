// Assignment #7 Solution

#include <stdio.h>
#include <stdlib.h>

void printHeader();

int main(void) {
	// Fix issue with Eclipse
	setvbuf (stdout, NULL, _IONBF, 0);

	// Declare primitive types
	int intValue = 1234;
	double doubleValue = 367.7;
	char charValue = 'A';
	int intArray[5] = {8, 1, 7, 2, 4};

	// Declare pointer types
	int *intValuePtr = &intValue;
	double *doubleValuePtr = &doubleValue;
	char *charValuePtr = &charValue;

	// Declare pointers to pointers
	int **pointerToIntValuePtr = &intValuePtr;

	// Print the header
	printHeader();

	// Print integer value and integer value pointer
	printf ("  int     intValue             %p    %d         %d\n", &intValue, intValue, sizeof (int));
	printf ("  int*    intValuePtr          %p    %p     %d      pointer to integer value\n", &intValuePtr, intValuePtr, sizeof (int*));
	printf ("  int*    intValuePtr          %p    %p     %d      pointer to integer value\n", &intValuePtr, intValuePtr, sizeof (intValuePtr));

	// Print double value and double value pointer
	puts ("");
	printf ("  double  doubleValue          %p    %.2f       %d\n", &doubleValue, doubleValue, sizeof (double));
	printf ("  double* doubleValuePtr       %p    %p     %d      pointer to double value\n", &doubleValuePtr, doubleValuePtr, sizeof (double*));

	// Print character value and character value pointer
	puts ("");
	printf ("  char    charValue            %p    %c            %d\n", &charValue, charValue, sizeof (char));
	printf ("  char*   charValuePtr         %p    %p     %d      pointer to character value\n", &charValuePtr, charValuePtr, sizeof (char*));

	// Print integer array
	puts ("");
	printf ("  int     intArray             %p    %p     %d     array name is address of 1st element - address of array and address of 1st element are the same!\n", &intArray, intArray, sizeof (intArray));
	printf ("  int     intArray[0]          %p    %d            %d\n", &intArray[0], intArray[0], sizeof (intArray[0]));
	printf ("  int     intArray[1]          %p    %d            %d\n", &intArray[1], intArray[1], sizeof (intArray[1]));
	printf ("  int     intArray[2]          %p    %d            %d\n", &intArray[2], intArray[2], sizeof (intArray[2]));
	printf ("  int     intArray[3]          %p    %d            %d\n", &intArray[3], intArray[3], sizeof (intArray[3]));
	printf ("  int     intArray[4]          %p    %d            %d\n", &intArray[4], intArray[4], sizeof (intArray[4]));

	// Print integer array through a pointer
	puts ("");
	int *intArrayPtr = NULL;
	printf ("  int*    intArrayPtr          %p    %p     %d      pointer set to NULL - doesn't point to anything\n", &intArrayPtr, intArrayPtr, sizeof (int*));

	intArrayPtr = intArray;
	printf ("  int*    intArrayPtr          %p    %p     %d      pointer to array has its own memory address - stores address of array\n", &intArrayPtr, intArrayPtr, sizeof (int*));

	// Access array elements through a pointer
	printf ("  int*    intArrayPtr          %p    %d            %d      access intArray[0] through pointer\n", &intArrayPtr[0], *intArrayPtr, sizeof (int*));
	printf ("  int*    intArrayPtr          %p    %d            %d      access intArray[1] through pointer\n", &intArrayPtr[1], *(intArrayPtr +1), sizeof (int*));
	printf ("  int*    intArrayPtr          %p    %d            %d      access intArray[2] through pointer\n", &intArrayPtr[2], *(intArrayPtr +2), sizeof (int*));
	printf ("  int*    intArrayPtr          %p    %d            %d      access intArray[3] through pointer\n", &intArrayPtr[3], *(intArrayPtr +3), sizeof (int*));
	printf ("  int*    intArrayPtr          %p    %d            %d      access intArray[4] through pointer\n", &intArrayPtr[4], *(intArrayPtr +4), sizeof (int*));

	// Print integer value and integer value pointer and pointer to integer value pointer
	puts ("");
	printf ("  int**   pointerToIntValuePtr %p    %p     %d      pointer to integer value pointer\n", &pointerToIntValuePtr, pointerToIntValuePtr, sizeof (int**));

	// Print obtaining values through pointers
	puts ("");
	puts ("");
	puts ("Accessing value using a pointer then pointer to pointer");
	puts ("-------------------------------------------------------");
	printf ("Accessing intValue though intValuePtr = %d\n", *intValuePtr);
	printf ("Accessing intValue through a pointer to intValuePtr - 2 levels of indirection = %d\n", **pointerToIntValuePtr);


} // main

// Display the header information
void printHeader () {
	printf ("----------------------------------------------------------------------------\n");
	printf ("| Type  | Name               | Address   | Value      | Size | Comment     |\n");
	printf ("----------------------------------------------------------------------------\n");
}


