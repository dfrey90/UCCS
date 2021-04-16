// Assignment #10 Solution

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 200

void processTemplateFileLine(char line[], char newLine[], char *customer[]);
void processConfirmationLetter (char * customer[]);
void processShippingLetter (char * customer[]);
void processReviewLetter (char * customer[]);

int main(void) {

	// Fix issue with Eclipse
	setvbuf (stdout, NULL, _IONBF, 0);

	// Create a pointer to the file
	FILE *filePtr = NULL;

	// Array of pointers to the different functions that can be called
	// There is a number in the customer data file that will be used as
	// an index into this array to call the proper function
	void (*functionPtr[3]) (char * customer[]) = {processConfirmationLetter,
											      processShippingLetter,
												  processReviewLetter};

	// Open the customer data file
	if ((filePtr = fopen("C:/Dev/Assignment10-CustomerData.txt", "r")) != NULL) {

		char line[100];
		int functionToCall;
		char product[50];
		char customerName[50];
		char customerStreetAddress[50];
		char customerCity[50];
		char customerState[50];
		char customerZip[50];
		char customerOrderCost[50];

		char *customer[7];

		// Process each customer in the file
		// The file structure for each customer is the following:
		//    Type of letter
		//    Product name
		//    Customer name
		//    Customer street address
		//    Customer city
		//    Customer state
		//    Customer zip code
		//    Order cost
		while (!feof(filePtr)) {

			// Read type of letter from the file - use as index into array of functions to call
			// correct letter processing function
			fgets(line, 50, filePtr);
			sscanf(line, "%d", &functionToCall);

			// Read product name from the file
			// Here customer[0] is a pointer, so need to place an address into it
			// Cannot say fgets(customer[0], 50, filePtr) because customer is an array
			// of pointers to strings but no memory has been allocated to store the string.
			// Instead, place value from file into array of allocated memory, then place address
			// of that array in customer (array of strings - which is array of pointers to strings)
			// Chapter 12, presents ability to allocate memory dynamically which is another way to do this..

			// Product is an array with allocated memory and the address of the first element
			// Assign that address to the first element in customer (which is an array of pointers)
			fgets(product, 50, filePtr);
			customer[0] = product;

			// Read customer name from the file
			fgets(customerName, 50, filePtr);
			customer[1] = customerName;

			// Read customer street address from the file
			fgets(customerStreetAddress, 50, filePtr);
			customer[2] = customerStreetAddress;

			// Read customer city from the file
			fgets(customerCity, 50, filePtr);
			customer[3] = customerCity;

			// Read customer state from the file
			fgets(customerState, 50, filePtr);
			customer[4] = customerState;

			// Read customer zip code from the file
			fgets(customerZip, 50, filePtr);
			customer[5] = customerZip;

			// Read customer order cost from the file
			fgets(customerOrderCost, 50, filePtr);
			customer[6] = customerOrderCost;

			/* FOR DEBUGGING
			for (int i = 0; i < 7; i ++) {
				printf ("Customer[%d] = %s", i, customer[i]);
			}
			puts (" ");
			*/

			// Call the function to create the desired letter
			functionPtr[functionToCall](customer);

		} // while not eof

		fclose(filePtr);
	}
	else {
		puts ("Could not open customer data file - no letters can be created");
	}

} // main


// Process one line of text from the file.
// Determine where the customer data needs to be inserted.
void processTemplateFileLine(char line[], char newLine[], char *customer[]) {

	////puts (" ");
	////printf ("Processing line => %s\n", line);

	// Get a token from the line
	char *tokenPtr = strtok(line, " ");

	// While this token is not null determine if the line contains any items that need to
	// be filled in with customer data
	while (tokenPtr != NULL) {

		// Determine if the token is a value that needs to be replaced with customer data
		if (strncmp(tokenPtr, "[product]", 9) == 0) {
			strcat(newLine, customer[0]);
		}
		else if (strncmp(tokenPtr, "[name]", 6) == 0) {
			strcat(newLine, customer[1]);
		}
		else if (strncmp(tokenPtr, "[streetaddress]", 15) == 0) {
			strcat(newLine, customer[2]);
		}
		else if (strncmp(tokenPtr, "[city]", 6) == 0) {
			strcat(newLine, customer[3]);
		}
		else if (strncmp(tokenPtr, "[state]", 7) == 0) {
			strcat(newLine, customer[4]);
		}
		else if (strncmp(tokenPtr, "[zip]", 5) == 0) {
			strcat(newLine, customer[5]);
		}
		else if (strncmp(tokenPtr, "[ordercost]", 11) == 0) {
			strcat(newLine, customer[6]);
		}
		else {
			strcat(newLine, tokenPtr);
		}

		strcat(newLine, " ");
		tokenPtr = strtok(NULL, " ");

	} // while not done tokenizing the line

} // processTemplateFileLine


// Create a confirmation letter based on the shipping template and the customer data
void processConfirmationLetter (char *customer[]) {

	FILE *filePtr;
	FILE *outputFilePtr = NULL;

	// Open the file with letter template and the file to write output letter
	if ( (filePtr = fopen("C:/Dev/Assignment10-ConfirmationTemplate.txt", "r")) != NULL &&
		 //(outputFilePtr = fopen(outputFileName, "w")) != NULL) {
		 (outputFilePtr = fopen("C:/Dev/Assignment10-Letter1.txt", "w")) != NULL) {

		puts ("Producing confirmation letter\n");
		char line[MAX_LINE_LENGTH];

		while (!feof(filePtr)) {
			// Read menu option from the file
			fgets(line, MAX_LINE_LENGTH, filePtr);

			// Process the line
			char newLine[MAX_LINE_LENGTH] = {0};
			processTemplateFileLine(line, newLine, customer);

			// Write processed line to a new file
			fprintf (outputFilePtr, "%s", newLine);
		}

		fclose(filePtr);
		fclose(outputFilePtr);
	}
	else {
		puts ("Files to create confirmation letter could not be opened");
	}

} // processConfirmationLetter


// Create a shipping letter based on the shipping template and the customer data
void processShippingLetter (char * customer[]) {

	FILE *filePtr;
	FILE *outputFilePtr = NULL;

	// Open the file with letter template and the file to write output letter
	if ( (filePtr = fopen("C:/Dev/Assignment10-ShippedTemplate.txt", "r")) != NULL &&
		 (outputFilePtr = fopen("C:/Dev/Assignment10-Letter2.txt", "w")) != NULL) {

		puts ("Producing shipping letter\n");
		char line[MAX_LINE_LENGTH];

		while (!feof(filePtr)) {
			// Read menu option from the file
			fgets(line, MAX_LINE_LENGTH, filePtr);

			// Process the line
			char newLine[MAX_LINE_LENGTH] = {0};
			processTemplateFileLine(line, newLine, customer);

			// Write processed line to a new file
			fprintf (outputFilePtr, "%s", newLine);
		}

		fclose(filePtr);
		fclose(outputFilePtr);
	}
	else {
		puts ("Files to create shipping letter could not be opened");
	}

} // processShippingLetter


// Create a review letter based on the review template and the customer data
void processReviewLetter (char * customer[]) {

	FILE *filePtr;
	FILE *outputFilePtr = NULL;

	// Open the file with letter template and the file to write output letter
	if ( (filePtr = fopen("C:/Dev/Assignment10-ReviewTemplate.txt", "r")) != NULL &&
		 (outputFilePtr = fopen("C:/Dev/Assignment10-Letter3.txt", "w")) != NULL) {

		puts ("Producing review letter\n");
		char line[MAX_LINE_LENGTH];

		while (!feof(filePtr)) {
			// Read menu option from the file
			fgets(line, MAX_LINE_LENGTH, filePtr);

			// Process the line
			char newLine[MAX_LINE_LENGTH] = {0};
			processTemplateFileLine(line, newLine, customer);

			// Write processed line to a new file
			fprintf (outputFilePtr, "%s", newLine);
		}

		fclose(filePtr);
		fclose(outputFilePtr);
	}
	else {
		puts ("Files to create review letter could not be opened");
	}

} // processReviewLetter
