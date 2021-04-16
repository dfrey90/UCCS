/*
Daniel Frey
CS2060_002
Assignment 6
10/19/17
This program sells concert tickets for a venue hosting 3 groups

Pseudocode:
Declare constants for section prices and fees/tax, group names, sections, rows, seats, and how long for purchase
Declare variables for concert, seciton, number of seats, seciton number, row, seat, and total cost

Set variables concert, section, number seats, section number, row, seat, array for seat info, total cost for function calls
Create a pointer for file operation

If the file exists
	Populate seat info
		While not EOF
			Loop through the file adding info into seating array
	Display concert menu
		Initialize variable for input to 0
		Display 3 group options
		Prompt for input, repeat if invalid
		Return selection
	Do
		Do
			Display sections
				Initialize variable for input to -1
				Display 4 different sections
				Prompt for input, repeat if invalid
				Return selection
			Get number of seats
				Initialize variable for input to 0
				Ask for number of seats between 1 and 6
				Get input, repeat if invalid or out of bounds
				Return number of seats
			Row and Seat
				Initialize variables for found seats, start and stop in for loop, seat count
				If number of seats is more than 1
					Add one to count
				Else
					Count is 0
				Loop through seat array
					If more than 1 seat
						If current seat and next seat vacant
							Add one to count
						Else
							Start over from 1
						If all seats found
							Add one to row then mod by max row
							If row is 0
								Row is 5
							Else
								Row is row
							Seat is seat position - number of seats + 3
							Seat range is seat position + 2
							Found seats is true
						Else
							Found seats is false
					Else
						If position is empty/0
							One seat, mark row and seat
							Found seats is true
						Else
							Found seats is false
				Return seats found or not
		While no seats are found
		Calculate ticket cost
			Initialize variables for cost, seat price, total, and seat bounds
			Use section to determine seat price
			Total is seat price times the number of seats
			Cost is total plus number of seats times fees/taxes
		Display ticket information: quantity, section, row, seat(s), seat price, total
			If number of seats is one
				If section floor
					Print ticket info with flor for section
				Else
					Print ticket info with number for section
			Else must have multiple tickets
				If section floor
					Print ticket info with floor for section
				Else
					Print ticket info with number for section
			Print service fee/tax using number of seats multiplied by fee/tax
			Print total cost
		Purchase tickets
			Initialize variable for purchase T/F, user input, start and end times
			Start time is current time at beginning
			Prompt for purchase yes no
				Repeat if invalid input
			Get end time after prompt
			If purchasing
				If within time limit
					Say total cost
					Purchase is true
				Else
					Purchase expired
					Purchase is false
			Else said no
				Purchase is false
			Return purchase
		If purchasing tickets
			Initialize variables for loop using seat range
			If the original seat is higher than range
				Start is the range (lower number)
				Stop is the original seat (higher number)
			Else
				Start is the orignal seat (higher number)
				Stop is the range (lower number)
			Loop through each ticket displaying ticket info
				If section number is floor
					Print floor for section number
				Else
					Print number for section number
		Update seat array with purchased ticket info
	While purchasing more tickets
	Calculate total cost
		Loop through seat array tallying seats
		Multiply num seats for each section by seat price
		Add all section prices
	Close pointer for file
Else file cannot be opened

*/

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

//array constant size for seating
#define ROW 20
#define COL 10

#pragma warning(disable:4996)

//function prototypes
void nextLine(FILE *filePtr, char line[]);
void populateSeatInfo(FILE *filePtr, int seatArray[ROW][COL]);
int displayConcertMenu(void);
int getSection(int concert);
int getSectionNumber(int section);
int getNumberOfSeats(int concert);
bool findRowAndSeatNumbers(int searArray[ROW][COL], int section, int numSeats, int *rowPtr, int *seatPtr, int *seatRangePtr);
float ticketCost(int section, int numSeats, int sectionNumber, int row, int seat, int seatRange);
bool purchaseTickets(float totalCost);
void printTickets(int concert, int section, int numSeats, int sectionNumber, int row, int seat, int seatRange);
const char* getConcertGroup(int concert);
float getSeatPrice(int section);
void updateSeatInfo(int seatArray[ROW][COL], int section, int numSeats, int row, int seat, int seatRange);
float calculateTotalSales(int seatArray[ROW][COL]);
int fpeek(FILE *filePtr);
int getBeginSearch(int section);
void determineStartStop(int numSeats, int seat, int seatRange, int *startPtr, int *stopPtr);

//constants
//ticket costs and fees/taxes
const float SECTION0_PRICE = 400.00;
const float SECTION1_PRICE = 325.00;
const float SECTION2_PRICE = 250.00;
const float SECTION3_PRICE = 100.00;
const float SERVICE_FEE_TAXES = 23.50;

//group names and numbers
const char * const GROUP_NAMES[] = { "Rolling Stones", "Imagine Dragons", "U2" };
const int GROUP1 = 1;
const int GROUP2 = 2;
const int GROUP3 = 3;

//sections
const int FLOOR = 0;
const int SECTION1 = 1;
const int SECTION2 = 2;
const int SECTION3 = 3;
const int MAX_ROW = 5;
const int MAX_SEAT = 10;
const int MIN_PURCHASE_SEATS = 1;
const int MAX_PURCHASE_SEATS = 6;

//max line size in file
const int MAX_LINE = 27;

//purchase time
const double PURCHASE_TIMEOUT = 30;


int main(void)
{
	int concert, section, numSeats, sectionNumber, row = 0, seat = 0, seatRange = 0;
	int seatArray[ROW][COL] = { 0 };
	float totalCost;
	char yesNoInput = 0;
	bool foundSeats = false;
	// Create a pointer to the file
	FILE *filePtr;
	errno_t err;

	// Open the file
	//if file opens, then continue, else quit
	if ((err = fopen_s(&filePtr, "C:/concert.txt", "r")) == 0)
	{
		//populate arrays with seating info
		populateSeatInfo(filePtr, seatArray);

		//get concert selection
		concert = displayConcertMenu();

		//repeat while wanting to purchase more tickets
		do
		{
			//do while no seats are found and customer wants to search again
			do
			{
			//get section
			section = getSection(concert);

			//get section number
			sectionNumber = getSectionNumber(section);

			//get number of seats
			numSeats = getNumberOfSeats(concert);

			//set row and seat(s) based on availability
			foundSeats = findRowAndSeatNumbers(seatArray, section, numSeats, &row, &seat, &seatRange);
			
			//if no seats found, ask if want to search again
			if (!foundSeats)
			{
				puts("There are no tickets available that meet your request.");
				puts("Please select a different section or different quantity.");
			}
			} while (!foundSeats);

			//reset yesNoInput
			yesNoInput = 0;
			
			//calculate total cost
			totalCost = ticketCost(section, numSeats, sectionNumber, row, seat, seatRange);

			//print ticket info if purchased and update seat array
			if (purchaseTickets(totalCost))
			{
				printTickets(concert, section, numSeats, sectionNumber, row, seat, seatRange);
				updateSeatInfo(seatArray, section, numSeats, row, seat, seatRange);
			}

			while (toupper(yesNoInput) != 'Y' && toupper(yesNoInput) != 'N')
			{
				printf("Do you want to purchase more concert tickets for %s? (Y/N): ", getConcertGroup(concert));
				scanf_s(" %c", &yesNoInput, sizeof(yesNoInput));

				if (toupper(yesNoInput) != 'Y' && toupper(yesNoInput) != 'N')
					printf("%s", "Invalid input. ");
			}
		} while (toupper(yesNoInput) == 'Y');

		if (toupper(yesNoInput) == 'N')
		{
			puts("You have chosen not to purchase more tickets.");
		}

		//calculate and dispaly total sales
		printf_s("The total sales (excluding fees and taxes) for the %s concert is: $%.2f.\n", getConcertGroup(concert), calculateTotalSales(seatArray));

		// Close the file 
		fclose(filePtr);
	}
	else
	{
		puts("File could not be opened.");
	}

	puts("Goodbye!");

	return 0;
}

// Reads the next line in the file and places the data into line array
void nextLine(FILE *filePtr, char line[])
{
	fgets(line, MAX_LINE, filePtr);
}

// Populate the array with the seating info from the file
void populateSeatInfo(FILE *filePtr, int seatArray[ROW][COL])
{
	//arrays to hold info from file
	char line[27], seats[COL + 1];

	//variables to get unused info in file
	int section, row;

	size_t i = 0;

	//repeat until end of file, adding info
	while (fpeek(filePtr) != -1)
	{
		//get line of file
		nextLine(filePtr, line);

		//separate line to store seat info
		sscanf(line, "%d %d %s", &section, &row, seats);

		//assign values from file to seat array
		for (size_t j = 0; j < COL; j++)
		{
			seatArray[i][j] = seats[j] - '0';
		}

		//increment row for seat array
		i++;
	}
}

// Displays the concert menu.
// Prompts customer for desired concert and returns selected concert (group).
int displayConcertMenu(void)
{
	int concert = 0;

	puts("-------------------");
	printf("1: %s\n", GROUP_NAMES[0]);
	printf("2: %s\n", GROUP_NAMES[1]);
	printf("3: %s\n", GROUP_NAMES[2]);
	puts("-------------------");
	
	//repat until valid input
	while (concert < GROUP1 || concert > GROUP3)
	{
		printf("%s", "Which concert would you like to attend? ");
		scanf_s("%d", &concert);

		if (concert < GROUP1 || concert > GROUP3)
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
	printf("0: Floor\t\t$%.2f\n", SECTION0_PRICE);
	printf("1: Section 102\t\t$%.2f\n", SECTION1_PRICE);
	printf("2: Section 202\t\t$%.2f\n", SECTION2_PRICE);
	printf("3: Section 302\t\t$%.2f\n", SECTION3_PRICE);


	//repeat until a valid input
	while (section < FLOOR || section > SECTION3)
	{
		printf("In which section would you like to view %s? ", getConcertGroup(concert));
		scanf_s("%d", &section);

		if (section < FLOOR || section > SECTION3)
			printf("%s", "Invalid selection. ");
	}

	return section;
}

// Get the section number from section selection
int getSectionNumber(int section)
{
	int sectionNumber;

	switch (section)
	{
		case 0:
			sectionNumber = 0;
			break;
		case 1:
			sectionNumber = 102;
			break;
		case 2:
			sectionNumber = 202;
			break;
		default:
			sectionNumber = 303;
			break;
	}
	
	return sectionNumber;
}


// Prompts customer for the number of seats they want for the selected concert.
// The customer can purchase between 1 and 6 seats.
int getNumberOfSeats(int concert)
{
	int numSeats = 0;

	//repeat until valid input
	while (numSeats < MIN_PURCHASE_SEATS || numSeats > MAX_PURCHASE_SEATS)
	{
		printf("How many seats would you like for %s? ", getConcertGroup(concert));
		scanf_s("%d", &numSeats);

		if(numSeats < MIN_PURCHASE_SEATS || numSeats > MAX_PURCHASE_SEATS)
			printf("%s", "Invalid input. ");
	}

	return numSeats;
}

// Search the contents of concert seating file to find seats together in a row
bool findRowAndSeatNumbers(int seatArray[ROW][COL], int section, int numSeats, int *rowPtr, int *seatPtr, int *seatRangePtr)
{
	bool foundSeats = false;
	unsigned int rowStart, rowStop;
	int seatCount;
	size_t row, seat;

	//find start and stop search positions
	rowStart = getBeginSearch(section);
	rowStop = rowStart + (MAX_ROW - 1);

	//determine starting seat count
	if (numSeats > 1)
	{
		//set seat count to 1, since comparing current vacant seat against next seat
		seatCount = 1;
	}
	else
	{
		seatCount = 0;
	}

	//search specific rows that correspond to sections
	for (row = rowStart; row <= rowStop && numSeats != seatCount; row++)
	{
		//search the column/seat
		for (seat = 0; seat < MAX_SEAT && numSeats != seatCount; seat++)
		{
			//if more than one seat
			if (numSeats > 1)
			{
				//since searching current and next, don't want to go out of bounds for last element
				if (seat < MAX_SEAT - 1)
				{
					//test current position and next position
					if (seatArray[row][seat] == 0 && seatArray[row][seat + 1] == 0)
					{
						//keep track of seats together, add one for another seat matched
						seatCount = seatCount + 1;
					}
					else
					{
						//start over back at 1
						seatCount = 1;
					}
				}//if

				//assign findings to row, seat, and seat range when correct number of seats found together
				if (numSeats == seatCount)
				{
					row = (row + 1) % MAX_ROW;

					if (row == 0)
					{
						*rowPtr = MAX_ROW;
					}
					else
					{
						*rowPtr = row;
					}

					//add one to seat to get last matched position, subtract numSeats-1, then add one to array index to get seat
					// [(position+1)-(#-1)]+1 => position-#+3
					*seatPtr = seat - numSeats + 3;

					//add one to seat to get last matched position, then add 1 again to array index to get seat range
					// (seat+1)+1 => seat+2
					*seatRangePtr = seat + 2;

					foundSeats = true;
				}
				else
				{
					foundSeats = false;
				}
			}//if
			//else numSeats is 1
			else
			{
				if (seatArray[row][seat] == 0)
				{
					//set seatCount to 1 since only matching 1 seat
					seatCount = 1;

					//assign findings to row and seat
					//add one to row and mod MAX_ROW (multiples of 5's) to get number between 0-4, where 0 is row 5
					row = (row + 1) % MAX_ROW;

					if (row == 0)
					{
						*rowPtr = MAX_ROW;
					}
					else
					{
						*rowPtr = row;
					}

					*seatPtr = seat + 1;

					foundSeats = true;
				}
				else
				{
					foundSeats = false;
				}
			}//else
		}//column
	}//row

	//foundSeats is true if found seats
	//foundSeats is false if not found seats
	return foundSeats;
}

// Calculate the total cost and display order information.
float ticketCost(int section, int numSeats, int sectionNumber, int row, int seat, int seatRange)
{
	float ticketCost, seatPrice, total;

	//calculate each price
	seatPrice = getSeatPrice(section);
	total = seatPrice * numSeats;
	ticketCost = total + (numSeats * SERVICE_FEE_TAXES);


	puts("\nQuantity\tSection\tRow\tSeats\tSeat Price\tTotal");
	puts("-----------------------------------------------------------------");

	//if one seat, else more than one seat
	if (numSeats == 1)
	{
		//if floor section print letter, else print number
		if (sectionNumber == FLOOR)
			printf("%d\t\t%s\t%d\t%d\t$%.2f\t\t$%.2f\n", numSeats, "Floor", row, seat, seatPrice, total);
		else
			printf("%d\t\t%d\t%d\t%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seat, seatPrice, total);
	}
	else
	{
		//if floor print letter, else print number
		if (sectionNumber == FLOOR)
			printf("%d\t\t%s\t%d\t%d-%d\t$%.2f\t\t$%.2f\n", numSeats, "Floor", row, seat, seatRange, seatPrice, total);
		else
			printf("%d\t\t%d\t%d\t%d-%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seat, seatRange, seatPrice, total);
	}
	
	printf("\t\t\tService Fee & Taxes(x%d):\t$%.2f\n", numSeats, numSeats * SERVICE_FEE_TAXES);
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
	while (toupper(yesNoInput) != 'Y' && toupper(yesNoInput) != 'N')
	{
		scanf_s(" %c", &yesNoInput, sizeof(yesNoInput));

		//if not valid input
		if (toupper(yesNoInput) != 'Y' && toupper(yesNoInput) != 'N')
			printf("%s", "Invalid input. Do you want to purchase these tickets(Y/N)?: ");
	}

	//get end time after question asked in seconds
	endTime = time(NULL);

	if (toupper(yesNoInput) == 'Y')
	{
		if (difftime(endTime, startTime) <= PURCHASE_TIMEOUT)
		{
			printf("Thank you for your purchase. Your credit card will be charged $%.2f.\n\n", totalCost);
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
		puts("Thank you for considering.");
		purchase = false;
	}

	return purchase;
}

// Print tickets for each seat purchased
void printTickets(int concert, int section, int numSeats, int sectionNumber, int row, int seat, int seatRange)
{
	unsigned int start, stop;

	determineStartStop(numSeats, seat, seatRange, &start, &stop);

	//for each ticket print ticket info
	for (start; start <= stop; start++)
	{
		printf("%s\n", getConcertGroup(concert));
		puts("-------------------------");
		puts("Section |  Row  |  Seat |");
		puts("-------------------------");

		if (sectionNumber == FLOOR)
			printf("%s      %d       %d\n", "Floor", row, start);
		else
			printf("%d        %d       %d\n", sectionNumber, row, start);
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
			concertGroup = GROUP_NAMES[0];
			break;
		case 2:
			concertGroup = GROUP_NAMES[1];
			break;
		default:
			concertGroup = GROUP_NAMES[2];
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

// Update seating array for tickets purchased
void updateSeatInfo(int seatArray[ROW][COL], int section, int numSeats, int row, int seat, int seatRange)
{
	int seatArrayRow;
	unsigned int start, stop;

	//determine array row for section row
	switch (section)
	{
		case 0:
			seatArrayRow = row;
			break;
		case 1:
			seatArrayRow = row + 4;
			break;
		case 2:
			seatArrayRow = row + 9;
			break;
		default:
			seatArrayRow = row + 14;
			break;
	}

	//determine which columns to update in seat array
	determineStartStop(numSeats, seat - 1, seatRange - 1, &start, &stop);

	//update seats for row
	for (start; start <= stop; start++)
	{
		seatArray[seatArrayRow][start] = 1;
	}
}

// Calculate the total sales for a concert
float calculateTotalSales(int seatArray[ROW][COL])
{
	size_t arrayRow, arrayCol;
	int sec0Tickets = 0, sec1Tickets = 0, sec2Tickets = 0, sec3Tickets = 0;
	float sec0Sales = 0, sec1Sales = 0, sec2Sales = 0, sec3Sales = 0, totalSales = 0;

	//loop through array tallying tickets sold for each section
	for (arrayRow = 0; arrayRow < 20; arrayRow++)
	{
		for (arrayCol = 0; arrayCol < 10; arrayCol++)
		{
			if (arrayRow >= 0 && arrayRow <= 4)
			{
				if (seatArray[arrayRow][arrayCol] == 1)
				{
					sec0Tickets = sec0Tickets + 1;
				}
			}
			else if (arrayRow >= 5 && arrayRow <= 9)
			{
				if (seatArray[arrayRow][arrayCol] == 1)
				{
					sec1Tickets = sec1Tickets + 1;
				}
			}
			else if (arrayRow >= 10 && arrayRow <= 14)
			{
				if (seatArray[arrayRow][arrayCol] == 1)
				{
					sec2Tickets = sec2Tickets + 1;
				}
			}
			else //array row 15-19
			{
				if (seatArray[arrayRow][arrayCol] == 1)
				{
					sec3Tickets = sec3Tickets + 1;
				}
			}
		}
	}

	//calulate individual section sales
	sec0Sales = sec0Tickets * SECTION0_PRICE;
	sec1Sales = sec1Tickets * SECTION1_PRICE;
	sec2Sales = sec2Tickets * SECTION2_PRICE;
	sec3Sales = sec3Tickets * SECTION3_PRICE;

	//sum up total sales
	totalSales = sec0Sales + sec1Sales + sec2Sales + sec3Sales;

	return totalSales;
}

// Peeks next char in file
// Used to test EOF since using user defined nextLine() instead
//-1 if EOF
int fpeek(FILE *filePtr)
{
	int c;

	c = fgetc(filePtr);
	ungetc(c, filePtr);

	return c;
}

// Gets the beginning range to search specific section of seat array
int getBeginSearch(int section)
{
	int rowStart;

	//determine start and stop for searching seat array
	switch (section)
	{
	case 0:
		rowStart = 0;
		break;
	case 1:
		rowStart = 5;
		break;
	case 2:
		rowStart = 10;
		break;
	default:
		rowStart = 15;
		break;
	}

	return rowStart;
}

// Determines the start and stop conditions in for loop
void determineStartStop(int numSeats, int seat, int seatRange, int *startPtr, int *stopPtr)
{
	//determine start and stop for loop
	if (numSeats == 1)
	{
		*startPtr = seat;
		*stopPtr = seat;
	}
	else if (seat > seatRange)
	{
		*startPtr = seatRange;
		*stopPtr = seat;
	}
	else
	{
		*startPtr = seat;
		*stopPtr = seatRange;
	}
}