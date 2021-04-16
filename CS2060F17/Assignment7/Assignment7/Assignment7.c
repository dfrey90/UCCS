/*
Daniel Frey
CS2060_002
Assignment 7
10/26/17
This program prints a memory map of variables and pointers
*/

#include <stdio.h>

int main(void)
{
	//declare and initialize variables
	int intValue = 13;
	double doubleValue = 13.87;
	char charValue = 'G';
	int intArray[] = { 4,8,6,1,0 };

	//declare and initialize pointers to variables
	int *intValuePtr = &intValue;
	double *doubleValuePtr = &doubleValue;
	char *charValuePtr = &charValue;
	int *intArrayPtr = NULL;

	//declare and initialize pointer to int pointer
	int **pointerToIntValuePtr = &intValuePtr;

	//print variable and pointer info
	//type name address value size comment
	puts("---------------------------------------------------------------------------------------------------");
	puts("| Type    | Name                 | Address  | Value    | Size | Comment                           |");
	puts("---------------------------------------------------------------------------------------------------");
	
	//int and intPtr
	printf_s("  int       intValue               %p   %d         %zu\n", &intValue, intValue, sizeof(intValue));
	printf_s("  int*      intValuePtr            %p   %p   %zu      Pointer to integer value\n", &intValuePtr, intValuePtr, sizeof(intValuePtr));
	puts("");
	
	//double and doublePtr
	printf_s("  double    doubleValue            %p   %.2lf      %zu\n", &doubleValue, doubleValue, sizeof(doubleValue));
	printf_s("  double*   doubleValuePtr         %p   %p   %zu      Pointer to double value\n", &doubleValuePtr, doubleValuePtr, sizeof(doubleValuePtr));
	puts("");
	
	//char and charPtr
	printf_s("  char      charValue              %p   %c          %zu\n", &charValue, charValue, sizeof(charValue));
	printf_s("  char*     charValuePtr           %p   %p   %zu      Pointer to char value\n", &charValuePtr, charValuePtr, sizeof(charValuePtr));
	puts("");
	
	//array
	printf_s("  int       intArray               %p   %p   %zu     Array name address of 1st element\n", &intArray, intArray, sizeof(intArray));
	for (size_t i = 0; i < 5; i++)
	{
		printf_s("  int       intArray[%d]            %p   %d          %zu\n", i, &intArray[i], intArray[i], sizeof(intArray[i]));
	}
	puts("");

	//arrayPtr
	printf_s("  int*      intArrayPtr            %p   %p   %zu      Pointer set to null\n", &intArrayPtr, intArrayPtr, sizeof(*intArray));
	//assign array to arrayPtr so it's not NULL
	intArrayPtr = intArray;
	printf_s("  int*      intArrayPtr            %p   %p   %zu      Pointer to array\n", &intArrayPtr, intArrayPtr, sizeof(*intArray));
	for (size_t i = 0; i < 5; i++)
	{
		printf_s("  int*      intArrayPtr[%d]         %p   %d          %zu      Access intArray[%d] through pointer\n", i, &(intArrayPtr + i), *(intArrayPtr + i), sizeof((intArray + i)), i);
	}
	puts("");

	//pointer to pointer
	printf_s("  int**     pointerToIntValuePtr   %p   %p   %zu      Pointer to integer value pointer\n", &pointerToIntValuePtr, pointerToIntValuePtr, sizeof(**pointerToIntValuePtr));

	//access value using pointer then pointer to pointer
	puts("\n\nAccessing value using a pointer then pointer to pointer");
	puts("----------------------------------------------------------------------------------");
	printf_s("Accessing intValue though intValuePtr = %d\n", *intValuePtr);
	printf_s("Accessing intValue though a pointer to intValuePtr - 2 levels of indirection = %d\n", **pointerToIntValuePtr);


	return 0;
}