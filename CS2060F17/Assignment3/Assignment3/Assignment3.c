/*
Daniel Frey
CS2060_02
Assignment 3
9-14-17
This program implements a simple scheme for encrypting and decoding a 4-digit integer

Pseudocode:
Initialize a variable for menu option and set to 0 (empty input)
while not exiting
	Dispaly menu and prompt for user input
	validate input for encrypt, decrypt, or exit
	if Encrypt
		initialize counter and variable for input
		read in the integer value
		add 7 and then mod 10
		remainder after division = new digit
		swap digits 1&3, 2&4
		display result -- (1234-->3412)
	else if Decrypt
		initialize counter and variable for input
		read in the integer value
		add 10 if less than 7
		subtract 7
		swap digits 3&1, 4&2
		display result -- (3412-->1234)
	else if exit, exit
	else
		must be invalid menu option
*/

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

void startEncryptDecrypt();
void encrypt();
void decrypt();
void swapDigits(int inputIntegers[]);
void printDigits(int inputIntegers[]);

int main(void)
{

	startEncryptDecrypt();

	return 0;
}

//this function starts the program, prompts for user input, and performs tasks based on input
void startEncryptDecrypt()
{
	int menuOptn = 0;

	//loop to keep performing unspecified number of tasks until exit
	while(menuOptn!=3)
	{
		//diplay menu and get menu selection
		puts("Encryption and Decryption Program");
		puts("----------------------------------");
		puts("1.) Encryption \n2.) Decryption \n3.) Exit");
		printf_s("%s", "Which option would you like? Select 1, 2 or 3: ");
		scanf_s("%d", &menuOptn);

		//validate user input and decide path based on selection
		if (1 <= menuOptn && menuOptn <= 3 && menuOptn == 1)
		{
			encrypt();
		}
		else if (1 <= menuOptn && menuOptn <= 3 && menuOptn == 2)
		{
			decrypt();
		}
		else if (1 <= menuOptn && menuOptn <= 3 && menuOptn == 3);//exit, end program
		else
			printf_s("%d is an invalid menu option. Please enter 1, 2 or 3.\n\n", menuOptn);
	}
}

//this function encrypts the 4 digit input
void encrypt()
{
	int count = 0, inputIntegers[4];

	printf_s("%s", "Enter a 4-digit number to encrypt: ");
	while (count < 4)
	{
		//scan one digit at a time
		scanf_s("%1d", &inputIntegers[count]);

		//encrypt digit
		inputIntegers[count] = (inputIntegers[count] + 7) % 10;

		count++;
	}

	swapDigits(inputIntegers);

	printf_s("The encrypted digits are: ");
	printDigits(inputIntegers);
}

//this functioin decrypts the 4 digit input
void decrypt()
{
	int count = 0, inputIntegers[4];

	printf_s("%s", "Enter a 4-digit number to decrypt: ");
	while (count < 4)
	{
		//read digit one at a time
		scanf_s("%1d", &inputIntegers[count]);

		//decrypt digit
		if (inputIntegers[count] < 7)
			inputIntegers[count] += 10;
		inputIntegers[count] -= 7;

		count++;
	}

	swapDigits(inputIntegers);

	printf_s("The decrypted digits are: ");
	printDigits(inputIntegers);
}

//this function swaps digits 1&3 and 2&4
void swapDigits(int inputIntegers[])
{
	int temp;

	//temp holds digit so not lost in swap, then swap digits

	//swap first and third
	temp = inputIntegers[0];
	inputIntegers[0] = inputIntegers[2];
	inputIntegers[2] = temp;

	//swap second and fourth
	temp = inputIntegers[1];
	inputIntegers[1] = inputIntegers[3];
	inputIntegers[3] = temp;
}

//this function prints out the 4 digits
void printDigits(int inputIntegers[])
{
	int count = 0;

	//iterate until all digits are displayed
	while (count < 4)
	{
		printf_s("%d", inputIntegers[count]);
		count++;
	}

	//for easy reading purposes only
	puts("\n");
}