/*
Daniel Frey
CS2060_002
Assignment 5
10/5/17
This program sells concert tickets for a venue hosting 3 groups

Pseudocode:
	Declare constants for section prices and fees/tax, group names, sections, rows, seats, and how long for purchase
	Declare variables for concert, seciton, number of seats, seciton number, row, seat, and total cost
	Set variables to function calls
	Display concert menu
		Initialize variable for input to 0
		Get input, repeat if invalid
		Return selection
	Display sections
		Initialize variable for input to -1
		Get input, repeat if invalid
		Return selection
	Get number of seats
		Initialize variable for input to 0
		Get input, repeat if invalid or out of bounds
		Return number of seats
	Get section number based on section selection
		Initialize variable for section number
		Seed random
		Randomly generate section number in bounds
		Return section number
	Generate and return random row number in bounds
	Generate and return random seat number in bounds
	Calculate ticket cost
		Initialize variables for cost, seat price, total, and seat bounds
		Use section to determine seat price
		Total is seat price times the number of seats
		Cost is total plus number of seats times fees/taxes
		Display ticket information
			If alpha row and multiple seats, display character and seat range
				Display smaller seat range first
			If alpha row and single seat, dispaly character and no seat range
			If number row and multiple seats, display seat range
			If number row and single seat, display no seat range
	Purchase tickets
		Initialize variable for purchase T/F, user input, start and end times
		Start time is current time at beginning
		Prompt for purchase yes no
			Repeat if invalid input
		Get end time after question
		If purchasing and within time limit
			Say total cost
			Purchase is true
		Else
			Purchase is false
		Return purchase
	If purchasing tickets
		Initialize variables for loop
		Beginning is lower end of seat range, end is high end of seat range
		Loop through each ticket displaying ticket info
*/

#include<stdio.h>
#include<stdbool.h>
#include<time.h>
#include<stdlib.h>

const float SECTION0_PRICE = 400.00, SECTION1_PRICE = 325.00, SECTION2_PRICE = 250.00, 
		SECTION3_PRICE = 100.00, SERVICE_TAX = 23.50;
const char* CONCERT_GROUP[] = { "Rolling Stones", "Imagine Dragons", "U2" };
const int SEC0_MIN = 65, SEC0_MAX = 70, SEC1_MIN = 102, SEC1_MAX = 148,
		SEC2_MIN = 202, SEC2_MAX = 260, SEC3_MIN = 302, SEC3_MAX = 380,
		ROW_MAX = 30, SEAT_MAX = 15;
const double PURCHASE_TIMEOUT = 30;

int displayConcertMenu(void);
int getSection(int concert);
int getNumberOfSeats(int concert);
int generateRandomSectionNumber(int section);
int generateRandomRowNumber(void);
int generateRandomSeatNumber(void);
float ticketCost(int section, int numSeats, int sectionNumber, int row, int seat);
bool purchaseTickets(float totalCost);
void printTickets(int concert, int section, int numSeats, int sectionNumber, int row, int seat);
const char* getConcertGroup(int concert);
float getSeatPrice(section);
int getSeatRange(int numSeats, int seat);
bool validYesNoInput(char yesNoInput);

int main(void)
{
	int concert, section, numSeats, sectionNumber, row, seat;
	float totalCost;

	concert = displayConcertMenu();
	section = getSection(concert);
	numSeats = getNumberOfSeats(concert);
	sectionNumber = generateRandomSectionNumber(section);
	row = generateRandomRowNumber();
	seat = generateRandomSeatNumber();
	totalCost = ticketCost(section, numSeats, sectionNumber, row, seat);
	if (purchaseTickets(totalCost))
		printTickets(concert, section, numSeats, sectionNumber, row, seat);

	return 0;
}

// Displays the concert menu.
// Prompts customer for desired concert and returns selected concert (group).
int displayConcertMenu(void)
{
	int concert = 0;

	puts("-------------------");
	puts("1: Rolling Stones");
	puts("2: Imagine Dragons");
	puts("3: U2");
	puts("-------------------");
	
	//repat until valid input
	while (concert < 1 || concert > 3)
	{
		printf("%s", "Which concert would you like to attend? ");
		scanf_s("%d", &concert);

		if (concert < 1 || concert > 3)
			printf("%s", "Invalid selection. ");
	}

	return concert;
}

// Displays the sections and prices associated with those sections.
// Prompts customer for desired section for selected concert (group).  
// Returns selected section.
int getSection(int concert)
{
	int section = -1;

	puts("\n--------------------------------");
	puts("   Section\t\tPrice");
	puts("--------------------------------");
	printf("0: Floor A-F\t\t$%.2f\n", SECTION0_PRICE);
	printf("1: Section 102 - 148\t$%.2f\n", SECTION1_PRICE);
	printf("2: Section 202 - 260\t$%.2f\n", SECTION2_PRICE);
	printf("3: Section 302 - 380\t$%.2f\n", SECTION3_PRICE);


	//repeat until a valid input
	while (section < 0 || section > 3)
	{
		printf("Which section would you like to view %s? ", getConcertGroup(concert));
		scanf_s("%d", &section);

		if (section < 0 || section > 3)
			printf("%s", "Invalid selection. ");
	}

	return section;
}


// Prompts customer for the number of seats they want for the selected concert.
// The customer can purchase between 1 and 6 seats.
int getNumberOfSeats(int concert)
{
	int numSeats = 0;

	//repeat until valid input
	while (numSeats < 1 || numSeats > 6)
	{
		printf("How many seats would you like for %s? ", getConcertGroup(concert));
		scanf_s("%d", &numSeats);

		if(numSeats < 1 || numSeats > 6)
			printf("%s", "Invalid input. ");
	}

	return numSeats;
}

// Generate a random section number and returns that section number.
int generateRandomSectionNumber(int section)
{
	int sectionNumber;

	srand((unsigned int)time(NULL));

	//generate random number between min and max for section
	switch (section)
	{
		case 0:
			sectionNumber = rand() % (SEC0_MAX - SEC0_MIN + 1) + SEC0_MIN;
			break;
		case 1:
			sectionNumber = rand() % (SEC1_MAX - SEC1_MIN + 1) + SEC1_MIN;
			break;
		case 2:
			sectionNumber = rand() % (SEC2_MAX - SEC2_MIN + 1) + SEC2_MIN;
			break;
		default:
			sectionNumber = rand() % (SEC3_MAX - SEC3_MIN + 1) + SEC3_MIN;
			break;
	}

	return sectionNumber;
}

// Generate a random row number and returns that row.
int generateRandomRowNumber(void)
{
	return rand() % ROW_MAX + 1;
}

// Generates a random seat number and returns that seat.
int generateRandomSeatNumber(void)
{
	return rand() % SEAT_MAX + 1;
}

// Calculate the total cost and display order information.
float ticketCost(int section, int numSeats, int sectionNumber, int row, int seat)
{
	float ticketCost, seatPrice, total;
	int seatRange = getSeatRange(numSeats, seat);

	//calculate each price
	seatPrice = getSeatPrice(section);
	total = seatPrice * numSeats;
	ticketCost = total + (numSeats * SERVICE_TAX);


	puts("\nQuantity\tSection\tRow\tSeats\tSeat Price\tTotal");
	puts("-----------------------------------------------------------------");

	//if section 0 print A-F and multiple seats or single seat, 
	//else if section 100-300 and multiple seats or single seat
	if (sectionNumber >= SEC0_MIN && sectionNumber <= SEC0_MAX && numSeats > 1)
	{
		//if seat number is higher than the beginning of seat range, put seat first
		if (seat > seatRange)
			printf("%d\t\t%c\t%d\t%d-%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seatRange, seat, seatPrice, total);
		else
			printf("%d\t\t%c\t%d\t%d-%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seat, seatRange, seatPrice, total);
	}
	else if (sectionNumber >= SEC0_MIN && sectionNumber <= SEC0_MAX && numSeats < 2)
	{
		printf("%d\t\t%c\t%d\t%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seat, seatPrice, total);
	}
	else if (numSeats > 1)
	{
		//if seat number is higher than the beginning of seat range, put seat first
		if (seat > seatRange)
			printf("%d\t\t%d\t%d\t%d-%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seatRange, seat, seatPrice, total);
		else
			printf("%d\t\t%d\t%d\t%d-%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seat, seatRange, seatPrice, total);
	}
	else
	{
		printf("%d\t\t%d\t%d\t%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seat, seatPrice, total);
	}
	
	printf("\t\t\tService Fee & Taxes(x%d):\t$%.2f\n", numSeats, numSeats * SERVICE_TAX);
	puts("-----------------------------------------------------------------");
	printf("\t\t\t\t\t\t Total: $%.2f\n", ticketCost);

	return ticketCost;
}

// Prompts customer to purchase tickets.  Starts a timer.  If customer doesn't purchase 
// tickets within 30 seconds, then void the order, otherwise charge credit card.
bool purchaseTickets(float totalCost)
{
	bool purchase;
	char yesNoInput = 0;
	//start time to current time in seconds
	time_t startTime = time(NULL), endTime;

	printf("%s", "You have 30 seconds to decide if you want to purchase these tickets (Y/N): ");
	//repeat until valid input
	while (!validYesNoInput(yesNoInput))
	{
		scanf_s(" %c", &yesNoInput, sizeof(yesNoInput));

		if (!validYesNoInput(yesNoInput))
			printf("%s", "Invalid input. Do you want to purchase these tickets(Y/N)?: ");
	}

	//get end time after question asked
	endTime = time(NULL);

	if (yesNoInput == 'Y' || yesNoInput == 'y')
	{
		if (difftime(endTime, startTime) < PURCHASE_TIMEOUT)
		{
			printf("Thank you for your purchase. Your credit card will be $%.2f.\n\n", totalCost);
			purchase = true;
		}
		else
		{
			puts("Sorry, this order expired, please try again.");
			purchase = false;
		}
	}
	else
	{
		puts("Thank you for considering. Goodbye.");
		purchase = false;
	}

	return purchase;
}

// Print tickets for each seat purchased
void printTickets(int concert, int section, int numSeats, int sectionNumber, int row, int seat)
{
	unsigned int start, stop;
	int seatRange = getSeatRange(numSeats, seat);

	//determine start and stop for loop
	if (seat > seatRange)
	{
		start = seatRange;
		stop = seat;
	}
	else
	{
		start = seat;
		stop = seatRange;
	}

	//for each ticket print ticket info
	for (start; start <= stop; start++)
	{
		printf("%s\n", getConcertGroup(concert));
		puts("-------------------------");
		puts("Section |  Row  |  Seat |");
		puts("-------------------------");

		if (sectionNumber >= SEC0_MIN && sectionNumber <= SEC0_MAX)
			printf("%c          %d       %d\n", sectionNumber, row, start);
		else
			printf("%d         %d      %d\n", sectionNumber, row, start);
		puts("-------------------------\n\n");
	}
}

// Returns concert group name given concert selection
const char* getConcertGroup(int concert)
{
	const char* concertGroup;

	switch (concert)
	{
		case 1:
			concertGroup = CONCERT_GROUP[0];
			break;
		case 2:
			concertGroup = CONCERT_GROUP[1];
			break;
		default:
			concertGroup = CONCERT_GROUP[2];
			break;
	}

	return concertGroup;
}

// Returns the seat price for a specific section
float getSeatPrice(int section)
{
	float seatPrice;

	switch (section)
	{
		case 0:
			seatPrice = SECTION0_PRICE;
			break;
		case 1:
			seatPrice = SECTION1_PRICE;
			break;
		case 2:
			seatPrice = SECTION2_PRICE;
			break;
		default:
			seatPrice = SECTION3_PRICE;
			break;
	}

	return seatPrice;
}

// Returns the number of seats together
int getSeatRange(int numSeats, int seat)
{
	int seatRange, adjustedNumSeats;

	//adjust number of seats for correct amount in range
	adjustedNumSeats = numSeats - 1;

	//seats can't exceed 15 or less than 1
	if ((adjustedNumSeats + seat) > 15)
	{
		seatRange = seat - adjustedNumSeats;
	}
	else
	{
		seatRange = seat + adjustedNumSeats;
	}

	return seatRange;
}

//validate input for Y/N question
bool validYesNoInput(char yesNoInput)
{
	if (yesNoInput == 'Y' || yesNoInput == 'y' || yesNoInput == 'N' || yesNoInput == 'n')
		return true;
	else
		return false;
}