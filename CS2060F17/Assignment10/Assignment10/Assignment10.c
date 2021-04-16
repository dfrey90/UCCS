/*
Daniel Frey
CS2060-002
Assignment10
11-21-17
This program will produce letters based on the information from 2 files

Pesudocode:
Initialize constants for: number of lines per customer data file to 8,
	array indices for product to1, name to 2, street address to 3, city to 4, state to 5, zip to 6, and order cost to 7,
	file names for customer data, confirm template/letter, ship template/letter, review template/letter
Main
	Start read files function
Function: Start read files
	Initialize variables for index to 0, customer count to 0, array of strings for customer data, file pointer to customer data
	Connect file pointer to file for reading
	If the file can be opened
		Initialize customer data array
		While not EOF, get and store customer data from file into array
			Replace newline character with a null character
			If index mod 8 is 0
				increment customer count
			Increment index
		Make letters function using customer data and customer count
		Deallocate memory for customer data
	Else
		File cannot be opened
Function: Initialize array, argument array of strings
	For each array index allocate memroy for info
Function: Free array, argument array of strings
	For each array index deallocate memory
Function: Make letters, argument array of strings, customer count
	Create array of function pointers for each letter function
	For each letter index
		Call function to make letter
Function: Confirm, Ship, Review letters, arugment array of strings, index
	Create variables for confirm/ship/review template/letter file, array to hold file line, 2 pointers for tokenizing string
	If the file can be opened
		While not EOF, get and store file line
			If file line has brackets
				If the first character in line is bracket
					Set pointers to tokenize string for brackets
					Compare and concatenate strings function
				Else
					Divide line from first left bracket
					Divide line from last right bracket
					Compare and concatenate strings function
				If line token is not null
					Write line token to file
				Else if the file line has a newline character
					Write file line to file
				Else (newline char only)
					Write file line to file
				Set both pointers used to tokenize strings back to null
			Else
				Cannot open file
			Close file pointers
Function: Compare and concatenate strings, argument compare string, concat string, array of strings
	If replacing name brackets
		If string is not empty
			Append name to line
		Else
			Copy name over line
	Else if replacing product brackets
		If string is not empty
			Append product to line
		Else
			Copy product over line
	Else if replacing street address brackets
		If string is not empty
			Append street address to line
		Else
			Copy street address over line
	Else if replacing city brackets
		If string is not empty
			Append city, state, and zip to line
		Else
			Copy city, state, and zip over line
	Else replacing order cost brackets
		If string is not empty
			Append order cost to line
		Else
			Copy order cost over line
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

//max line size
#define MAX_LINE 256

//number of customer data lines in file
const int CUST_DATA_LINES = 8;

//customer data info for array
const int PRODUCT = 1;
const int NAME = 2;
const int STREET_ADDRESS = 3;
const int CITY = 4;
const int STATE = 5;
const int ZIP = 6;
const int ORDER_COST = 7;

//file names
const char CUST_DATA_FILE[] = "C:/Dev/CustomerData.txt";
const char CONFIRM_TEMPLATE[] = "C:/Dev/ConfirmationTemplate.txt";
const char SHIP_TEMPLATE[] = "C:/Dev/ShippedTemplate.txt";
const char REVIEW_TEMPLATE[] = "C:/Dev/ReviewTemplate.txt";
const char CONFIRM_LETTER[] = "C:/Dev/ConfirmationLetter.txt";
const char SHIP_LETTER[] = "C:/Dev/ShippingLetter.txt";
const char REVIEW_LETTER[] = "C:/Dev/Review.txt";

//function prototypes
void startReadFiles();
void initializeArray(char *array[]);
void freeArray(char *array[]);
void makeLetters(const char * const customerData[], unsigned int custCount);
void confirmLetter(const char * const customerData[], int baseIndex);
void shipLetter(const char * const customerData[], int baseIndex);
void reviewLetter(const char * const customerData[], int baseIndex);
void cmpCatStrings(const char compareStr[], char concatStr[], const char * const customerData[], int baseIndex);

int main(void)
{
	startReadFiles();

	return 0;
}

void startReadFiles()
{
	size_t index = 0, customerCount = 0;
	char *customerData[MAX_LINE];
	FILE *custFilePtr = fopen(CUST_DATA_FILE, "r");

	//if file exists, else file DNE
	if (custFilePtr != NULL)
	{
		initializeArray(customerData);

		//while not EOF store lines into customer data array
		while (fgets(customerData[index], MAX_LINE, custFilePtr) != NULL)
		{
			//replace the newline character with a null character
			char *ch = strchr(customerData[index], '\n');
			if(ch !=NULL)
				*ch = '\0';

			//count number of customers, every 9th line is a new customer (8 because array starts 0)
			if (index % CUST_DATA_LINES == 0)
			{
				customerCount++;
			}

			index++;
		}

		//produce the letters using customer data
		makeLetters(customerData, customerCount);

		//deallocate mem from malloc call, end of program
		freeArray(customerData);
	}
	else
	{
		puts("Customer Data file cannot be opened or does not exist.");
	}
}

//initializes the array by allocating memory
void initializeArray(char *array[])
{
	for (size_t i = 0; i < MAX_LINE; i++)
	{
		array[i] = malloc(MAX_LINE * sizeof(char));
	}
}

void freeArray(char *array[])
{
	//deallocate each index where malloc was called
	for (size_t i = 0; i < MAX_LINE; i++)
	{
		free(array[i]);
	}
}

//determines which letter to make based on customer data file/array
void makeLetters(const char * const customerData[], unsigned int customerCount)
{
	//function pointer array to letter functions
	void(*functionPtr[])(const char * const[], int) = { confirmLetter, shipLetter, reviewLetter };

	unsigned int numLines = customerCount * CUST_DATA_LINES;

	//determine letter type, use current line to < (current line + 8)
	for (size_t funcIndex = 0, baseIndex = 0; funcIndex < customerCount && baseIndex < numLines; funcIndex++, baseIndex += CUST_DATA_LINES)
	{
		(*functionPtr[funcIndex])(customerData, baseIndex);
	}
}

//reads the confirmation template file and creates a new confirm file with cust data
void confirmLetter(const char * const customerData[], int baseIndex)
{
	FILE *cnfrmTmpFilePtr = fopen(CONFIRM_TEMPLATE, "r");
	FILE *writeCnfrmFilePtr = fopen(CONFIRM_LETTER, "w");
	char cnfrmLine[MAX_LINE], *lineToken = NULL, *lineCmp = NULL;

	if (cnfrmTmpFilePtr != NULL)
	{
		//while not at EOF, read file and store lines
		while (fgets(cnfrmLine, MAX_LINE, cnfrmTmpFilePtr) != NULL)
		{
			//if a brackets are found
			if (strchr(cnfrmLine, '[') != NULL && strchr(cnfrmLine, ']') != NULL)
			{
				//if line begins with a bracket '[' (is only [...])
				if (cnfrmLine[0] == '[' && cnfrmLine[strlen(cnfrmLine) - 2] == ']')
				{
					lineCmp = strtok(cnfrmLine, "[]");
					lineToken = strtok(cnfrmLine, "[]");

					//compare lineCmp with keyword to add to lineToken
					cmpCatStrings(lineCmp, lineToken, customerData, baseIndex);
				}
				//else if line begins with bracket word but contains more after
				else if (cnfrmLine[0] == '[')
				{
					lineCmp = strtok(cnfrmLine, "[]");
					lineToken = strtok(NULL, "\n");

					//compare lineCmp with keyword to add to lineToken
					cmpCatStrings(lineCmp, lineToken, customerData, baseIndex);
				}
				//else line begins with words and contains brackets in middle
				else
				{
					//divide the string from the first bracket, and then to the last bracket
					lineToken = strtok(cnfrmLine, "[");
					lineCmp = strtok(NULL, "]");

					//compare lineCmp with keyword to add to lineToken
					cmpCatStrings(lineCmp, lineToken, customerData, baseIndex);
				}
			}

			//write line to file
			//if changed line is not null, else if line already have newline char, else just print newline char
			if (lineToken != NULL)
				fprintf(writeCnfrmFilePtr, "%s\n", lineToken);
			else if(strtok(cnfrmLine, "\n"))
				fprintf(writeCnfrmFilePtr, "%s\n", cnfrmLine);
			else
				fprintf(writeCnfrmFilePtr, "%s", cnfrmLine);

			//erase content for next use
			lineCmp = NULL;
			lineToken = NULL;
		}
	}
	else
	{
		puts("Cannot open the Confirmation Template file.");
	}

	//close files
	fclose(cnfrmTmpFilePtr);
	fclose(writeCnfrmFilePtr);
}

//reads the shipped template file and creates a new ship file with cust data
void shipLetter(const char * const customerData[], int baseIndex)
{
	FILE *shipTmpFilePtr = fopen(SHIP_TEMPLATE, "r");
	FILE *writeShipFilePtr = fopen(SHIP_LETTER, "w");
	char shipLine[MAX_LINE], *lineToken = NULL, *lineCmp = NULL;

	if (shipTmpFilePtr != NULL)
	{
		//while not at EOF, read file and store lines
		while (fgets(shipLine, MAX_LINE, shipTmpFilePtr) != NULL)
		{
			//if a brackets are found
			if (strchr(shipLine, '[') != NULL && strchr(shipLine, ']') != NULL)
			{
				//if line begins with a bracket '[' (is only [...])
				if (shipLine[0] == '[' && shipLine[strlen(shipLine) - 2] == ']')
				{
					lineCmp = strtok(shipLine, "[]");
					lineToken = strtok(shipLine, "[]");

					//compare lineCmp with keyword to add to lineToken
					cmpCatStrings(lineCmp, lineToken, customerData, baseIndex);
				}
				//else if line begins with bracket word but contains more after
				else if (shipLine[0] == '[')
				{
					lineCmp = strtok(shipLine, "[]");
					lineToken = strtok(NULL, "\n");

					//compare lineCmp with keyword to add to lineToken
					cmpCatStrings(lineCmp, lineToken, customerData, baseIndex);
				}
				//else line begins with words and contains brackets in middle
				else
				{
					//divide the string from the first bracket, and then to the last bracket
					lineToken = strtok(shipLine, "[");
					lineCmp = strtok(NULL, "]");

					//compare lineCmp with keyword to add to lineToken
					cmpCatStrings(lineCmp, lineToken, customerData, baseIndex);
				}
			}

			//write line to file
			//if changed line is not null, else if line already have newline char, else just print newline char
			if (lineToken != NULL)
				fprintf(writeShipFilePtr, "%s\n", lineToken);
			else if (strtok(shipLine, "\n"))
				fprintf(writeShipFilePtr, "%s\n", shipLine);
			else
				fprintf(writeShipFilePtr, "%s", shipLine);

			//erase content for next use
			lineCmp = NULL;
			lineToken = NULL;
		}
	}
	else
	{
		puts("Cannot open the Confirmation Template file.");
	}

	//close files
	fclose(shipTmpFilePtr);
	fclose(writeShipFilePtr);
}

//reads the review template file and creates a new review file with cust data
void reviewLetter(const char * const customerData[], int baseIndex)
{
	FILE *reviewTmpFilePtr = fopen(REVIEW_TEMPLATE, "r");
	FILE *writeReviewFilePtr = fopen(REVIEW_LETTER, "w");
	char reviewLine[MAX_LINE], *lineToken = NULL, *lineCmp = NULL;

	if (reviewTmpFilePtr != NULL)
	{
		//while not at EOF, read file and store lines
		while (fgets(reviewLine, MAX_LINE, reviewTmpFilePtr) != NULL)
		{
			//if a brackets are found
			if (strchr(reviewLine, '[') != NULL && strchr(reviewLine, ']') != NULL)
			{
				//if line begins with a bracket '[' (is only [...])
				if (reviewLine[0] == '[' && reviewLine[strlen(reviewLine) - 2] == ']')
				{
					lineCmp = strtok(reviewLine, "[]");
					lineToken = strtok(reviewLine, "[]");

					//compare lineCmp with keyword to add to lineToken
					cmpCatStrings(lineCmp, lineToken, customerData, baseIndex);
				}
				//else if line begins with bracket word but contains more after
				else if (reviewLine[0] == '[')
				{
					lineCmp = strtok(reviewLine, "[]");
					lineToken = strtok(NULL, "\n");

					//compare lineCmp with keyword to add to lineToken
					cmpCatStrings(lineCmp, lineToken, customerData, baseIndex);
				}
				//else line begins with words and contains brackets in middle
				else
				{
					//divide the string from the first bracket, and then to the last bracket
					lineToken = strtok(reviewLine, "[");
					lineCmp = strtok(NULL, "]");

					//compare lineCmp with keyword to add to lineToken
					cmpCatStrings(lineCmp, lineToken, customerData, baseIndex);
				}
			}
			//write line to file
			//if changed line is not null, else if line already have newline char, else just print newline char
			if (lineToken != NULL)
				fprintf(writeReviewFilePtr, "%s\n", lineToken);
			else if (strtok(reviewLine, "\n"))
				fprintf(writeReviewFilePtr, "%s\n", reviewLine);
			else
				fprintf(writeReviewFilePtr, "%s", reviewLine);

			//erase content for next use
			lineCmp = NULL;
			lineToken = NULL;
		}
	}
	else
	{
		puts("Cannot open the Confirmation Template file.");
	}

	//close files
	fclose(reviewTmpFilePtr);
	fclose(writeReviewFilePtr);
}

//compares strings and concatenates one if needed
void cmpCatStrings(const char compareStr[], char concatStr[], const char * const customerData[], int baseIndex)
{
	//if compare string is == "name"
	if (strncmp(compareStr, "name", 4) == 0)
	{
		//if concatStr not NULL and not appending to beginning, else if append to beginning, else copy data into blank string
		if (strncmp(concatStr, "name", 4) != 0 && concatStr[0] != 32)
			strcat(concatStr, customerData[baseIndex + NAME]);
		else if (strncmp(concatStr, "name", 4) != 0)
		{
			char name[MAX_LINE];
			strcpy(name, customerData[baseIndex + NAME]);
			strcat(name, concatStr);
			strcpy(concatStr, name);
		}
		else
			strcpy(concatStr, customerData[baseIndex + NAME]);
	}
	//product
	else if (strncmp(compareStr, "product", 7) == 0)
	{
		//if concatStr not NULL and not appending to beginning, else if append to beginning, else copy data into blank string
		if (strncmp(concatStr, "product", 7) != 0 && concatStr[0] != 32)
			strcat(concatStr, customerData[baseIndex + PRODUCT]);
		else if (strncmp(concatStr, "product", 7) != 0)
		{
			char product[MAX_LINE];
			strcpy(product, customerData[baseIndex + PRODUCT]);
			strcat(product, concatStr);
			strcpy(concatStr, product);
		}
		else
			strcpy(concatStr, customerData[baseIndex + PRODUCT]);
	}
	//street address
	else if (strncmp(compareStr, "streetaddress", 13) == 0)
	{
		//if concatStr not NULL and not appending to beginning, else if append to beginning, else copy data into blank string
		if (strncmp(concatStr, "streetaddress", 13) != 0 && concatStr[0] != 32)
			strcat(concatStr, customerData[baseIndex + STREET_ADDRESS]);
		else if (strncmp(concatStr, "streetaddress", 13) != 0)
		{
			char streetAddress[MAX_LINE];
			strcpy(streetAddress, customerData[baseIndex + STREET_ADDRESS]);
			strcat(streetAddress, concatStr);
			strcpy(concatStr, streetAddress);
		}
		else
			strcpy(concatStr, customerData[baseIndex + STREET_ADDRESS]);
	}
	//city state zip
	else if (strncmp(compareStr, "city", 4) == 0)
	{
		//if concatStr not NULL and not appending to beginning, else if append to beginning, else copy data into blank string
		if (strncmp(concatStr, "city", 4) != 0 && concatStr[0] != 32)
		{
			//add city, space, state, space, zip
			strcat(concatStr, customerData[baseIndex + CITY]);
			strcat(concatStr, " ");
			strcat(concatStr, customerData[baseIndex + STATE]);
			strcat(concatStr, " ");
			strcat(concatStr, customerData[baseIndex + ZIP]);
		}
		else if (strncmp(concatStr, "city", 4) != 0)
		{
			char cityStateZip[MAX_LINE];
			strcpy(cityStateZip, customerData[baseIndex + CITY]);
			strcat(cityStateZip, " ");
			strcat(cityStateZip, customerData[baseIndex + STATE]);
			strcat(cityStateZip, " ");
			strcat(cityStateZip, customerData[baseIndex + ZIP]);

			strcat(cityStateZip, concatStr);
			strcpy(concatStr, cityStateZip);
		}
		else
		{
			//copy and overwrite contents into concastStr, then append remaining info
			strcpy(concatStr, customerData[baseIndex + CITY]);
			strcat(concatStr, " ");
			strcat(concatStr, customerData[baseIndex + STATE]);
			strcat(concatStr, " ");
			strcat(concatStr, customerData[baseIndex + ZIP]);
		}
	}
	//order cost
	else
	{
		//if concatStr not NULL and not appending to beginning, else if append to beginning, else copy data into blank string
		if (strncmp(concatStr, "ordercost", 9) != 0 && concatStr[0] != 32)
			strcat(concatStr, customerData[baseIndex + ORDER_COST]);
		else if (strncmp(concatStr, "ordercost", 9) != 0)
		{
			char orderCost[MAX_LINE];
			strcpy(orderCost, customerData[baseIndex + ORDER_COST]);
			strcat(orderCost, concatStr);
			strcpy(concatStr, orderCost);
		}
		else
			strcpy(concatStr, customerData[baseIndex + ORDER_COST]);
	}
	//else do nothing if not replacing brackets
}