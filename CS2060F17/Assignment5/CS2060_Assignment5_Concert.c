// CS2060 Assignment #5 (Functions) Solution

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

// Function prototypes
int displayConcertMenu(void);
int getSection (int whichConcert);
int getNumberOfSeats (int whichConcert);
float ticketCost (int section, int numSeats, int sectionNumber, int row, int seat);
int generateRandomSectionNumber (int section);
int generateRandomRowNumber (void);
int generateRandomSeatNumber (void);
bool purchaseTickets(float totalCost);
void printTickets (int whichConcert, int section, int numberSeats, int sectionNumber, int row, int seat);

// Concert group menu items
const int ROLLING_STONES = 1;
const int IMAGINE_DRAGONS = 2;
const int U2 = 3;

// Sections in the concert venue
const int FLOOR = 0;
const int SECTION_100 = 1;
const int SECTION_200 = 2;
const int SECTION_300 = 3;

// Ticket cost for each group
const float FLOOR_PRICE = 400.00;
const float SECTION_100_PRICE = 325.00;
const float SECTION_200_PRICE = 250.00;
const float SECTION_300_PRICE = 100.00;

// Service Fees and time to purchase
const float SERVICE_FEE_TAXES = 23.50;
const int ALLOWED_PURCHASE_TIME = 30;

int main(void) {
	// Fix issue with Eclipse
	// Seed the random number function to get different values
	setvbuf (stdout, NULL, _IONBF, 0);
	srand(time(NULL));

	// Prompt customer for which concert they want to attend
	int whichConcert = displayConcertMenu();

	// Now need to know what section the customer wants to sit
	// The section determines the price
	int section = getSection(whichConcert);

	// Next find out how many seats customer wants
	int numSeats = getNumberOfSeats(whichConcert);

	// Generate random values for section, row and seat
	// Section number is randomly generated based on desired section
	// Row is random value between 1 and 30
	// Seat is random value between 1 and 15
	int sectionNumber = generateRandomSectionNumber(section);
	int row = generateRandomRowNumber();
	int seat = generateRandomSeatNumber();

	// Now that we know which concert, section and how many seats, the next step
	// is to calculate the total cost and display that information to the customer
	float concertCost = ticketCost (section, numSeats, sectionNumber, row, seat);

	// Finally give customer option to purchase the tickets within 30 seconds
	bool purchaseSeats = purchaseTickets(concertCost);
	if (purchaseSeats) {
		printTickets(whichConcert, section, numSeats, sectionNumber, row, seat);
	}

} // main


// Display the concert menu.
// Prompts customer for desired concert and returns selected concert.
int displayConcertMenu (void) {

	int concert;   // Which concert customer wants to attend

	// Display menu and check for valid menuOption.
	puts ("------------------------------");
	puts ("1: Rolling Stones");
	puts ("2: Imagine Dragons");
	puts ("3: U2");
	puts ("------------------------------");
	printf ("Which concert would you like to attend? ");

	scanf ("%d", &concert);
	while (concert != ROLLING_STONES && concert != IMAGINE_DRAGONS && concert != U2) {

		printf ("%d is not a valid option.  Please try again. ", concert);
		scanf ("%d", &concert);
	}

	return concert;

} // displayMenu


// Displays the sections and prices based on the concert customer wants to see.
// Prompts user for desired section and returns selected section
int getSection (int whichConcert) {

	int section;   	// Which section the user wants to sit

	// Display section menu and check for valid selection.
	puts ("");
	puts ("-------------------------------------------");
	puts ("     Section             Price");
	puts ("-------------------------------------------");
	printf ("0: Floor A - F           $%.2f\n", FLOOR_PRICE);
	printf ("1: Section 102 - 148     $%.2f\n", SECTION_100_PRICE);
	printf ("2: Section 202 - 260     $%.2f\n", SECTION_200_PRICE);
	printf ("3: Section 302 - 380     $%.2f\n", SECTION_300_PRICE);
	puts ("-------------------------------------------");

	// Display a proper message based on selected concert
	if (whichConcert == ROLLING_STONES) {
		printf ("In what section would you like to view the Rolling Stones? " );
	}
	else if (whichConcert == IMAGINE_DRAGONS) {
		printf ("In what section would you like to view Imagine Dragons? " );
	}
	else {
		printf ("In what section would you like to view U2? " );
	}

	// Read section and validate
	scanf ("%d", &section);
	while (section != FLOOR &&
		   section != SECTION_100 &&
		   section != SECTION_200 &&
		   section != SECTION_300) {

			printf ("%d is not a valid section.  Please try again. ", section);
			scanf ("%d", &section);
	}

	return section;

} // getSection


// Prompts the user for the number of seats they want for the selected concert
// The user can select between 1 and 6 seats.
int getNumberOfSeats (int whichConcert) {

	int numSeats = 0;

	// Display a proper message based on selected concert
	if (whichConcert == ROLLING_STONES) {
		printf ("How many seats for the Rolling Stones? ");
	}
	else if (whichConcert == IMAGINE_DRAGONS) {
		printf ("How many seats for the Imagine Dragons? ");
	}
	else if (whichConcert == U2) {
		printf ("How many seats for U2? ");
	}
	scanf ("%d", &numSeats);

	// Use a while loop to do seat validation
	while (numSeats < 1 || numSeats > 6) {

		printf ("%d seats is not valid.  Please select between 1 and 6 seats. ", numSeats);
		scanf ("%d", &numSeats);

	} // while

	return numSeats;

} // getNumberOfSeats


// Calculate the total cost and display this result
float ticketCost (int section, int numSeats, int sectionNumber, int row, int seat) {

	float seatCost = 0;  		// Price of one seat for a specific group
	float totalSeatCost = 0;    // Price for all seats
	float serviceFeeTaxes = 0;	// Price for service fees and taxes per ticket
	float totalCost = 0; 		// Total cost for all seats including service fee and taxes

	// Determine seat cost based on section customer selected
	if (section == FLOOR) {
		seatCost = FLOOR_PRICE;
	}
	else if (section == SECTION_100) {
		seatCost = SECTION_100_PRICE;
	}
	else if (section == SECTION_200) {
		seatCost = SECTION_200_PRICE;
	}
	else {
		seatCost = SECTION_300_PRICE;
	}

	// Compute the different costs based on number of seats customer wants
	totalSeatCost = numSeats * seatCost;
	serviceFeeTaxes = numSeats * SERVICE_FEE_TAXES;
	totalCost = totalSeatCost + serviceFeeTaxes;

	// Display total costs
	puts ("");
	printf ("Quantity\tSection\tRow\tSeats\tSeat Price\tTotal\n");
	puts ("-------------------------------------------------------------------");
	if (numSeats == 1) {
		// If the seats are on the floor, need to print out the section as a character
		if (section == FLOOR) {
			printf ("%d\t\t%c\t%d\t%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seat, seatCost, totalSeatCost);
		}
		else {
			printf ("%d\t\t%d\t%d\t%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seat, seatCost, totalSeatCost);
		}
	}
	else {
		// If more than one seat is selected, need to print out seats as a range
		// Also if the seats are on the floor, need to print out the section as a character
		if (section == FLOOR) {
			printf ("%d\t\t%c\t%d\t%d-%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seat, seat+(numSeats-1), seatCost, totalSeatCost);
		}
		else {
			printf ("%d\t\t%d\t%d\t%d-%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seat, seat+(numSeats-1), seatCost, totalSeatCost);
		}
	}
	puts ("");
	printf ("\t\t\t\tService Fee & Taxes(x%d)\t$%.2f", numSeats, serviceFeeTaxes);
	puts ("");
	puts ("-------------------------------------------------------------------");
	printf ("\t\t\t\t\t\tTotal:\t$%.2f\n", totalCost);
	puts("");

	return totalCost;

} // ticketCost


// Generate a random section number and return that random section.
int generateRandomSectionNumber (int section) {

	// Now that we know the customer desired section, generate a random section number
	// Floor section numbers range from A - F
	// Section 100 section numbers range from 102 - 148
	// Section 200 section numbers range from 202 to 260
	// Section 300 section numbers range from 302 to 380
	int sectionNumber = 0;

	// When generating section numbers, use section to determine correct values.
	if (section == FLOOR) {
		// Seats on the floor are letters A- F
		// Generate a random number between 65 and 70 to represent sections A - F
		sectionNumber = 65 + (rand() % (70-65));
	}
	else if (section == SECTION_100) {
		// Section 100 has sections between 102 and 148
		sectionNumber = 102 + (rand() % (148-102));   // Random number from 102 to 148
	}
	else if (section == SECTION_200) {
		// Section 200 has sections between 202 and 260
		sectionNumber = 202 + (rand() % (260-202));
	}
	else if (section == SECTION_300) {
		// Section 300 has sections between 302 and 380
		sectionNumber = 302 + (rand() % (380-302));
	}

	return sectionNumber;

} // generateRandomSectionNumber


// Generate a random row number and returns that row.
int generateRandomRowNumber (void) {

	// Generate a random row from 1 to 30
	return (1 + (rand() % 30));

} //generateRandomRowNumber


// Generates a random seat number and returns that seat.
int generateRandomSeatNumber (void) {

	// Generate a random seat from 1 to 15
	return (1 + (rand() % 15));

} //generateRandonSeatNumber


// Prompts customer to purchase tickets.  Starts a timer.  If customer doesn't select to purchase
// tickets within 30 seconds, then void the order, otherwise charge credit card.
bool purchaseTickets(float totalCost) {

	time_t startTime;	// Captures time when start purchase
	time_t stopTime;	// Captures time when customer decides to make purchase
	int timeDifference; // Difference between the start time and when user makes decision
	char yesNoInput;	// Customer input
	bool purchase;

	// Get the start time
	startTime = time(NULL);

	// Prompt customer asking if they would like to purchase tickets
	// Inform customer they have 30 seconds to decide
	puts ("");
	printf("You have 30 seconds to decide if you want to purchase these tickets (Y/N)? ");
	scanf (" %c", &yesNoInput);

	// Validate customer input
	while (toupper(yesNoInput) != 'Y' && toupper(yesNoInput) != 'N') {
		printf ("%c is not valid.  Please enter Y or N: ", yesNoInput);
		scanf (" %c", &yesNoInput);
	}

	// Get the stop time
	stopTime = time(NULL);

	// If customer decided to purchase tickets, ensure 30 seconds did not elapse
	if (toupper(yesNoInput) == 'Y') {

		// Calculate elapse time
		timeDifference = difftime(stopTime, startTime);
		if (timeDifference <= ALLOWED_PURCHASE_TIME) {
			printf ("Thank you for your purchase.  Your credit card will be charged $%.2f\n", totalCost);
			purchase = true;
		}
		else {
			puts ("Sorry, this order expired, please try again");
			purchase = false;
		}
	}
	else {
		puts ("Thank you!  Visit soon for all the latest concerts.");
		purchase = false;
	}

	return purchase;

} // purchaseTickets


// Print tickets for each seat purchased
void printTickets (int whichConcert, int section, int numberSeats, int sectionNumber, int row, int seat) {

	int currentSeat = seat;

	// Print section, row and seat information for each ticket
	puts ("");
	for (unsigned int i = 1; i <= numberSeats; i++) {
		if (whichConcert == ROLLING_STONES) {
			puts ("Rolling Stones");
		}
		else if (whichConcert == IMAGINE_DRAGONS) {
			puts ("Imagine Dragons");
		}
		else {
			puts ("U2");
		}

		printf ("-------------------------\n");
		printf ("Section |  Row  |  Seat |\n");
		printf ("-------------------------\n");

		// Determine if we need to print a character or number for the section number
		if (section == FLOOR) {
			printf ("%c          %d       %d\n", sectionNumber, row, currentSeat);
		}
		else {
			printf ("%d          %d       %d\n", sectionNumber, row, currentSeat);
		}
		printf ("-------------------------\n");

		currentSeat++;

		puts ("");
		puts ("");
	} // for each seat

} // printTickets

