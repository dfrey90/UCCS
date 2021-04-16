/*
Daniel Frey
CS2060_002
Assignment 4
9-28-17
This program simulates a simple self-serve gas station with one pump

Pseudocode:
srand(time(NULL));
rand() % 30 + 1;

initialize variables for regular/diesel fuel prices
Setup mode
	Initialize variables for Y/N input to '0', and proceeding to false
	
	while not continuing
		Prompt for input for fuel prices
		Validate input for fuel prices
		while not a valid input
			Prompt for input for correctness of prices
			if invalid, say invalid
		if yes then continue
		else if no, change input to invalid to keep loop going
Customer mode
	Initialize varialbes for proceed to true, Y/N input to 'n', one or two input, car wash, car wash code, gallons,
	fuel type, zip code, total customer cost, total pump gallons to 0, total pump fuel costs to 0
	
	while continuing
		while invalid input and not quitting
			prompt for discount card
			if not quitting
				if invalid, say invalid and loop
			else assume quitting
				no longer continuing main loop
				while shutdown pin is incorrect
					prompt for shutdown pin
		if not quitting
			if discount card present, reduce fuel by 0.03/gal
			reset input for later use
			while invalid input
				prompt for car wash
				if invalid, say invalid
			if yes, prompt for type
				while invalid input
					prompt for car wash option
					if invalid, say invalid
				if selection one
					basic info
				else
					premium info
			reset inputs for later use
			assume credit card swiped and prompt for zip code
			while invalid input
				prompt for fuel grade
				if invalid, say invalid
			if input selection one
				regular fuel
			else
				diesel fuel
			reset input
			generate random number to pump gas gallons
				for 0 to random generated gallons
					display running total price and gallons
			while invalid input
				prompt for receipt
				if invalid, say invalid
			if yes, print:
				number of gallons, price/gal, fuel cost, car wash cost and code if included, total cost
			reset input
			update total gallons
			update total fuel cost
	correct pin entered - display total number gallons/fuel costs for all customers
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

const double DISCOUNT = 0.03, CARWSH_BASIC = 2.00, CARWSH_PREM = 5.00;
const int MAX_GAL = 30;

void setupMode(double* regFuel, double* dieselFuel);
void customerMode(double regFuel, double dieselFuel);
bool validYesNoInput(char yesNoInput);
bool validOneTwoInput(int oneTwoInput);

int main(void)
{
	double regFuel, dieselFuel;

	setupMode(&regFuel, &dieselFuel);
	customerMode(regFuel, dieselFuel);

	return 0;
}

//Simulates store setting price for reg/diesel fuel, prompts for input on each
void setupMode(double* regFuel, double* dieselFuel)
{
	char yesNoInput = '0';
	bool yesNoContinue = false;

	puts("**************************************");
	puts("        Gas Station Pump Setup");
	puts("**************************************");

	while (!yesNoContinue)
	{
		//prompt for input for fuel prices
		printf("%s", "Enter price of regular fuel: ");
		scanf_s("%lf", regFuel);

		printf("%s", "Enter price of diesel fuel:  ");
		scanf_s("%lf", dieselFuel);

		//validate fuel prices

		//repeat until valid yes no input
		while (!validYesNoInput(yesNoInput))
		{
			printf("%s", "Are these the correct prices? (Y/N): ");
			scanf_s(" %c", &yesNoInput, sizeof(yesNoInput));

			if (!validYesNoInput(yesNoInput))
				puts("Invalid input.");
		}

		//if yes then continue, if no then make input invalid to keep looping
		if (yesNoInput == 'Y' || yesNoInput == 'y')
			yesNoContinue = true;
		else if (yesNoInput == 'N' || yesNoInput == 'n')
			yesNoInput = '0';
	} 
}

//Asks customer questions about the services they want
void customerMode(double regFuel, double dieselFuel)
{
	bool proceed = true;
	char yesNoInput = '0';
	int zipCode, oneTwoInput = 0, gallons, pumpTotalGal = 0, galCount, carWashCode, shutdownPin = 0;
	double rFuel = regFuel, dFuel = dieselFuel, fuelType, carWash = 0, totalCustCost = 0, pumpTotalFuelCost = 0;

	//use srand later for random gallons
	srand((unsigned int)time(NULL));

	while (proceed)
	{
		puts("\n\n**************************************");
		puts("        Welcome to Gas Station");
		puts("**************************************");

		//repeat until valid input for discount card
		while (!validYesNoInput(yesNoInput) && yesNoInput != 'Q' && yesNoInput != 'q')
		{
			printf("%s", "Do you have a discount card? (Y/N): ");
			scanf_s(" %c", &yesNoInput, sizeof(yesNoInput));

			//if not shutting down pump
			if (yesNoInput != 'Q' && yesNoInput != 'q')
			{
				if (!validYesNoInput(yesNoInput))
					puts("Invalid input.");
			}
			else
			{
				//shutting down pump, do not continue
				proceed = false;

				//loop until correct pin entered
				while (shutdownPin != 2445)
				{
					printf("%s", "To shut down pump and get report, enter pin: ");
					scanf_s("%d", &shutdownPin);

					if (shutdownPin != 2445)
						puts("Invalid pin.");
				}
			}
		}

		//if quitting, then skip the customer parts
		if (yesNoInput != 'Q' && yesNoInput != 'q')
		{
			//if yes to discount card apply discount
			if (yesNoInput == 'Y' || yesNoInput == 'y')
			{
				puts("Scanning discount card...");
				puts("$0.03/gallon discount");
				rFuel = regFuel - DISCOUNT;
				dFuel = dieselFuel - DISCOUNT;
			}
			else
			{
				rFuel = regFuel;
				dFuel = dieselFuel;
			}

			//reset y/n input
			yesNoInput = '0';

			//repeat until valid input for car wash
			while (!validYesNoInput(yesNoInput))
			{
				printf("%s", "\nCar wash? (Y/N): ");
				scanf_s(" %c", &yesNoInput, sizeof(yesNoInput));

				if (!validYesNoInput(yesNoInput))
					puts("Invalid input.");
			}

			//if yes to car wash, select type and validate input
			if (yesNoInput == 'Y' || yesNoInput == 'y')
			{
				puts("1 Basic wash\t$2.00\n2 Premium wash\t$5.00");

				while (!validOneTwoInput(oneTwoInput))
				{
					printf("%s", "Enter car wash type: ");
					scanf_s("%d", &oneTwoInput);

					if (!validOneTwoInput(oneTwoInput))
						puts("Invalid input.");
				}

				//valid input, either 1 or 2
				if (oneTwoInput == 1)
				{
					carWash = CARWSH_BASIC;
					carWashCode = 1750;
				}
				else
				{
					carWash = CARWSH_PREM;
					carWashCode = 1751;
				}
			}

			//reset y/n input
			yesNoInput = '0';

			//reset 1/2 input
			oneTwoInput = 0;

			//pretend credit card scanned, ask for zip code
			puts("\nInsert credit card");
			printf("%s", "Enter zip code: ");
			scanf_s("%d", &zipCode);
			puts("Please wait... Authorizing... ... ...");

			//repeat until valid input for fuel grade
			while (!validOneTwoInput(oneTwoInput))
			{
				printf("%s", "\nLift nozzle and select fuel grade - Regular (1) or Diesel (2): ");
				scanf_s("%d", &oneTwoInput);

				if (!validOneTwoInput(oneTwoInput))
					puts("Invalid input.");
			}

			//valid input, either 1 or 2
			if (oneTwoInput == 1)
				fuelType = rFuel;
			else
				fuelType = dFuel;


			//reset 1/2 input
			oneTwoInput = 0;

			//pump fuel, generate random number of gallons from 1 to 30
			gallons = rand() % MAX_GAL + 1;

			//loop until number of random gallons reached, display price and gallons
			puts("\n********** Fueling **********");
			for (galCount = 0; galCount < gallons; galCount++)
			{
				printf("** $%.2lf\t%d Gallons **\n", fuelType * (galCount + 1), galCount + 1);
			}
			puts("*****************************");

			//repeat until valid input for receipt
			while (!validYesNoInput(yesNoInput))
			{
				printf("%s", "\nReceipt? (Y/N): ");
				scanf_s(" %c", &yesNoInput, sizeof(yesNoInput));

				if (!validYesNoInput(yesNoInput))
					puts("Invalid input.");
			}

			//print receipt if yes
			if (yesNoInput == 'Y' || yesNoInput == 'y')
			{
				puts("\n************* Receipt *************");
				printf("\nGallons:\t%d", galCount);
				printf("\nPrice/gallon:\t$%.2lf", fuelType);
				printf("\nFuel cost:\t$%.2lf", fuelType * galCount);

				if (carWash > 0)
				{
					printf("\nCar wash cost:\t$%.2lf (code: %d)", carWash, carWashCode);
					totalCustCost = (fuelType * galCount) + carWash;
				}
				else
					totalCustCost = fuelType * galCount;

				puts("\n-----------------------------------");
				printf("Total cost:\t$%.2lf", totalCustCost);
				//car wash cost/code
				puts("\n***********************************");
			}

			//reset y/n input
			yesNoInput = '0';

			//reset car wash cost
			carWash = 0;

			//reset individual customer cost
			totalCustCost = 0;

			puts("\nThank you for your business!");

			//update fuel cost and gallons
			pumpTotalGal = pumpTotalGal + galCount;
			pumpTotalFuelCost = pumpTotalFuelCost + (fuelType * galCount);
		}
	}

	//exit to here from quit
	//display total gallons and sales
	puts("Shutting down...");
	puts("Pump has been successfully shut down.");


	printf("Total gallons:\t%d", pumpTotalGal);
	printf("\nTotal sales:\t%.2lf", pumpTotalFuelCost);

}

//validate input for Y/N question
bool validYesNoInput(char yesNoInput)
{
	if (yesNoInput == 'Y' || yesNoInput == 'y' || yesNoInput == 'N' || yesNoInput == 'n')
		return true;
	else
		return false;
}

//validate input for 1 or 2
bool validOneTwoInput(int oneTwoInput)
{
	if (oneTwoInput == 1 || oneTwoInput == 2)
		return true;
	else
		return false;
}