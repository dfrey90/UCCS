// Assignment #9 Solution - Using 3 daily log files and pointers
// Final version assigned to class Fall 2017

#include <stdio.h>
#include <stdbool.h>

#define MAX_NUMBER_CUSTOMERS 100

bool findCustomerId (const int customerKey, const int * const customerList);
int length (const int * const customerList);

void populateCustomerList (const char *fileName, int customerList[]);
void findRepeatCustomers (const int * const customerList1, const int * const customerList2, int repeatCustomerList[]);
void writeCustomerList (const char *fileName, const int * const customerList);

// FOR DEBUGGING ONLY
void printCustomerList (int * const customerList);

int main(void) {

	// Fix issue with Eclipse
	setvbuf (stdout, NULL, _IONBF, 0);

	int customerList1[MAX_NUMBER_CUSTOMERS] = {0};
	int customerList2[MAX_NUMBER_CUSTOMERS] = {0};
	int customerList3[MAX_NUMBER_CUSTOMERS] = {0};
	int twoDayRepeatCustomerList[MAX_NUMBER_CUSTOMERS] = {0};
	int threeDayRepeatCustomerList[MAX_NUMBER_CUSTOMERS] = {0};

	// Populate the arrays with the data stored in the 3 files
	populateCustomerList ("C:/Dev/Nov22.txt", customerList1);
	populateCustomerList ("C:/Dev/Nov23.txt", customerList2);
	populateCustomerList ("C:/Dev/Nov24.txt", customerList3);

	// Search the two arrays for repeat customers and place into the repeatCustomerList
	findRepeatCustomers (customerList1, customerList2, twoDayRepeatCustomerList);
	findRepeatCustomers (customerList3, twoDayRepeatCustomerList, threeDayRepeatCustomerList);

	// Write the repeat customers to a file
	writeCustomerList ("C:/Dev/RepeatCustomers.txt", threeDayRepeatCustomerList);

} // main


// Determine how many elements are in an array
// Arrays were initialized to zero so when a zero is reached we've reached end of array
int length (const int * const customerList) {

	int numElements = 0;		// number of elements in the array

	while (customerList[numElements] != 0 && numElements < MAX_NUMBER_CUSTOMERS) {
		numElements++;
	}

	return numElements;

} //length


// Search the customer list array for a customer id
bool findCustomerId (const int customerKey, const int * const customerList) {

	// Determine the length of the customer array
	int numCustomers = length(customerList);

	int index = 0;
	bool found = false;

	// Look for key (customer id) until it is found or until the end of the array is reached
	while (!found && index < numCustomers ) {
		if (customerKey == customerList[index]) {
			found = true;
		}
		index++;
	}

	// Return true or false based on if key was found in the customer array
	if (found) {
		return true;
	}
	else {
		return false;

	}

} // findCustomerId


// Fill a customer array with data from a file.
// Make files with notepad not wordpad!
void populateCustomerList (const char *fileName, int customerList[]) {

	// Create a pointer to the file
	FILE *filePtr;

	// Open the file for reading
	if ((filePtr = fopen(fileName, "r")) != NULL) {

		int customerId;
		int numCustomers = 0;

		// Fill the customer array with the customer IDs from the file
		while (!feof(filePtr)) {

			// Read customer Id from file
			fscanf(filePtr, "%d", &customerId);

			// Don't add duplicates to the array so check if this customer is already in the array
			bool duplicate = findCustomerId (customerId, customerList);
			if (!duplicate) {

				// Add this customer since customer is not yet in the array
				customerList[numCustomers] = customerId;
				numCustomers++;

			}
		} // while

		// DEBUGGING - REMOVE: Print array of customer IDs
		//printCustomerList (customerList);

		// Close the input customer file
		fclose(filePtr);

	} // file opened successfully

	else {
		puts ("File could not be opened");
	}

}  // populateCustomerList


// Examine two arrays and determine which customers are in both.
// Place the repeat customers in the array repeatCustomer
void findRepeatCustomers (const int * const customerList1, const int * const customerList2, int repeatCustomerList[]) {

	// Get the number of customers in the 1st customer array
	int numCustomers = length(customerList1);
	int repeatIndex = 0;


	for (size_t i = 0; i < numCustomers; i++) {
		int customerId = customerList1[i];

		// Determine if customer is a repeat customer
		bool found = findCustomerId (customerId, customerList2);

		// If the customer is a repeat, add to the repeat customer array
		if (found) {
			repeatCustomerList[repeatIndex] = customerId;
			repeatIndex++;
		}

	} // for each customer in first customer array

} // findDuplicates


// Write the customer IDs in a customerList array to a file
void writeCustomerList (const char *fileName, const int * const customerList) {

	// Determine number of customers in the customer array
	int numCustomers = length(customerList);

	// Make sure we have one or more customers to write to the file
	if (numCustomers != 0) {

		// Create a pointer to the file
		FILE *filePtr;

		// Open the file for writing
		if ((filePtr = fopen(fileName, "w")) != NULL) {

			for (size_t i = 0; i < numCustomers; i++) {
				fprintf (filePtr, "%d\n", customerList[i]);
				printf ("Writing %d to file\n", customerList[i]);
			}
		}
		else {
				puts ("File could not be opened");
		}

	} // at least one customer in array
	else {
		puts ("No repeat customers");
	}

} // writeCustomerList


// FOR DEBUGGING
// Print array of customer IDs
void printCustomerList (int * const customerList) {

	puts ("");
	int numCustomers = length(customerList);
	for (int i = 0; i < numCustomers; i++) {
		printf ("Customer Id #%d: %d\n", i, customerList[i]);
	}
	puts ("");
}
