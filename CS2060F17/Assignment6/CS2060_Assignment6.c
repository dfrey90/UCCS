// Solution for Assignment6.c  -- Note solution is not 100% complete


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

// Need to use #define if want to use these as cases in a switch
#define FLOOR 0
#define SECTION_102 1
#define SECTION_202 2
#define SECTION_302 3

// Assignment #5 function prototypes
int displayConcertMenu(void);
int getSection (int whichConcert);
int getNumberOfSeats (int whichConcert);
float ticketCost (int numSeats, int section, int row, int seat);
bool purchaseTickets(float totalCost);
void printTickets (int whichConcert, int section, int numberSeats, int sectionNumber, int row, int seat);

// Assignment #6 function prototypes
// I provided the students with the nextLine function
void nextLine (FILE *filePtr, char line[]);
void populateSectionWithFileData (FILE *filePtr, int section[5][10]);
bool findAvailableSeats (int numSeats, int section[5][10], int availableSeat[1]);
void updateSectionSeats (int numSeats, int section[5][10], int availableSeat[]);
void totalVenueSales (int whichConcert, int floor[5][10], int section102[5][10], int section202[5][10], int section302[5][10]);

const int MAX_LINE = 27;
const int ROWS_PER_SECTION = 5;
const int SEATS_PER_ROW = 10;

// Concert group menu items
const int ROLLING_STONES = 1;
const int IMAGINE_DRAGONS = 2;
const int U2 = 3;

// Ticket cost for each group
const float FLOOR_PRICE = 400.00;
const float SECTION_102_PRICE = 325.00;
const float SECTION_202_PRICE = 250.00;
const float SECTION_302_PRICE = 100.00;

// Service Fees and time to purchase
const float SERVICE_FEE_TAXES = 23.50;
const int ALLOWED_PURCHASE_TIME = 30;


int main(void) {
	// Fix issue with Eclipse
	// Seed the random number function so get different values
	setvbuf (stdout, NULL, _IONBF, 0);
	srand(time(NULL));

	// Create a pointer to the file
	FILE *filePtr;

	// Create 2-dimensional arrays for the different seating sections
	// Each seating section has 5 rows and then 10 seats in the row
	int floor[5][10];
	int section102[5][10];
	int section202[5][10];
	int section302[5][10];

	// Need this variable to compute the total sales for the entire concert
	int whichConcert;

	// Open the file
	// Make files with notepad not wordpad!
	if ((filePtr = fopen("C:/concert.txt", "r")) != NULL) {

		// Read the data from the file and fill in each seating section
		populateSectionWithFileData (filePtr, floor);
		populateSectionWithFileData (filePtr, section102);
		populateSectionWithFileData (filePtr, section202);
		populateSectionWithFileData (filePtr, section302);

		fclose(filePtr);

		// Prompt customer for which concert they want to attend
		whichConcert = displayConcertMenu();

		// Start processing the customer until customer is done buying tickets
		bool processCustomer = true;
		while (processCustomer) {

			// Prompt customer for what section the they want to sit in - section determines price
			int customerSelectedSection = getSection(whichConcert);

			// Next find out how many seats customer wants
			int numSeats = getNumberOfSeats(whichConcert);

			// Determine if any available seats in customer selected section
			int availableRowNumber = 0;
			int availableSeatNumber = 0;
			int availableSeat[2] = {0, 0};
			bool availableSeats = false;

			switch (customerSelectedSection) {
				case FLOOR:
					availableSeats = findAvailableSeats(numSeats, floor, availableSeat);
					break;
			   case SECTION_102:
				   availableSeats = findAvailableSeats(numSeats, section102, availableSeat);
				   break;
			   case SECTION_202:
				   availableSeats = findAvailableSeats(numSeats, section202, availableSeat);
				   break;
			   case SECTION_302:
				   availableSeats = findAvailableSeats(numSeats, section302, availableSeat);
				   break;
			} // switch to find seats in desired section

			// If there are seats available, show customer the seats and the cost
			if (availableSeats) {

				// Get the row and seat numbers out of availableSeat array
				// We know the seats are next to each other so this is the starting seat
				availableRowNumber = availableSeat[0];
				availableSeatNumber = availableSeat[1];

				// Now that we know which concert, section and how many seats, the next step
				// is to calculate the total cost and display that information to the customer
				float concertCost = ticketCost (numSeats, customerSelectedSection, availableRowNumber, availableSeatNumber);

				// Finally give customer option to purchase the tickets within 30 seconds
				bool purchaseSeats = purchaseTickets(concertCost);
				if (purchaseSeats) {

					//printTickets(whichConcert, customerSelectedSection, numSeats, customerSelectedSection, availableRowNumber, availableSeatNumber);

					switch (customerSelectedSection) {
						case FLOOR:
							updateSectionSeats (numSeats, floor, availableSeat);
							break;
					   case SECTION_102:
						   updateSectionSeats (numSeats, section102, availableSeat);
						   break;
					   case SECTION_202:
						   updateSectionSeats (numSeats, section202, availableSeat);
						   break;
					   case SECTION_302:
						   updateSectionSeats (numSeats, section302, availableSeat);
						   break;
					} // switch to find seats in desired section

				} // if customer wants to purchase seats

				// Prompt customer to determine if they would like to purchase more tickets
				char yesNoInput;
				puts ("");
				printf ("Would you like to purchase more concert tickets? ");
				scanf (" %c", &yesNoInput);

				// Validate input
				while (toupper(yesNoInput) != 'Y' && toupper(yesNoInput) != 'N') {
					printf ("Invalid input, purchase more concert tickets? (Y/N) ");
					scanf (" %c", &yesNoInput);
				}

				// Check if we need to end loop
				if (toupper(yesNoInput) == 'N') {
					processCustomer = false;
				}

			} // if seats are available
			else {
				printf ("There are no tickets available that meet your request. Please select a different quantity or choose another price range.\n");

			} // no seats available

		} // while processing a customer

	} // file opened successfully

	else {
		puts ("File could not be opened");
	}

	// Display the total sales for this venue
	totalVenueSales (whichConcert, floor, section102, section202, section302);

	return EXIT_SUCCESS;

} // main


// Populate the incoming section with the seating data in the file
void populateSectionWithFileData (FILE *filePtr, int section[5][10]) {

	// Setup array to hold the line we will be reading from file
	// Each line in the file contains: section number, row number, seats (0 = available, 1 = sold)
	char line[50];			// One line from the file

	// There will be 5 lines in the file for each section
	for (unsigned int lineNumber = 1; lineNumber <= 5; lineNumber ++) {

		// Get the line to process from the file
		nextLine (filePtr, line);

		// Process the data in the line we read from the file
		// Note: no need to process the section number in the line since the
		// "array for a specific section" is sent to this method.  However, we
		// do need the row - it tells us which row in the array these seats are for!
		// Extract the row number and then the seats from the line

		int sectionNumber;
		int rowNumber;
		sscanf(line, "%d%d", &sectionNumber, &rowNumber);

		// The seats start at location 6 in the array
		// Move the seats into the sent in section
		// Make sure to subtract one from the row since file data is not zero-based
		int seatIndex = 6;
		for (int i = 0; i < SEATS_PER_ROW; i++) {
			section [rowNumber-1][i] = line[seatIndex] - '0';
			seatIndex++;
		}

	} // for each line in the file for this section

} //fillSectionWithFileData


// ***************** PROVIDED BY ME ***************************
// Reads the next line in the file.
// Returns false when the end of the file is reached, otherwise returns true.
void nextLine (FILE *filePtr, char line[]) {

	// Read one line from the file
	fgets(line, MAX_LINE, filePtr);

} // nextLine


// Determines if there are any seats available in the section
bool findAvailableSeats (int numSeats, int section[5][10], int availableSeat[]) {

	bool foundSeats = false;
	bool keepLooking = true;
	int row = 0;

	while (keepLooking && row < ROWS_PER_SECTION) {

		// Check each seat in current row for availability
		// NEED TO ADD CODE TO MAKE SURE NOT GOING PAST ARRAY BOUNDS - DID NOT complete!
		int seat = 0;
		while (keepLooking && seat < SEATS_PER_ROW) {

			if (section[row][seat] == 0) {

				switch (numSeats) {
				   case 1:
					   foundSeats = true;
					   break;
				   case 2:
					   if (section[row][seat+1] == 0) {
						   foundSeats = true;
					   }
					   break;
				   case 3:
					   if (section[row][seat+1] == 0 && section[row][seat+2] == 0) {
						   foundSeats = true;
					   }
					   break;
				   case 4:
					   if (section[row][seat+1] == 0 && section[row][seat+2] == 0 && section[row][seat+3] == 0) {
							  foundSeats = true;
					   }
					   break;
				   case 5:
					   if (section[row][seat+1] == 0 && section[row][seat+2] == 0 &&
						   section[row][seat+3] == 0 && section[row][seat+4] == 0) {
							  foundSeats = true;
					   }
					   break;
				   case 6:
					   if (section[row][seat+1] == 0 && section[row][seat+2] == 0 &&
						   section[row][seat+3] == 0 && section[row][seat+4] == 0 &&
						   section[row][seat+5] == 0) {
							  foundSeats = true;
					   }
					   break;
				} // switch

			} // if found an open seat

			if (foundSeats) {
				// If we found seats we can stop searching the array
				// Need to place the available row and available seat into an array to return to called
				// Increment row and seat by one so not zero based and ready to show in order
				keepLooking = false;
				availableSeat[0] = row+1;
				availableSeat[1] = seat+1;
			}

			seat++;

		} // for each seat

		row++;

	} // while no seats have been found that are available and entire section has not been checked

	return foundSeats;

} // findAvailableSeats


// If the customer purchases the seats then update the seats in the section so no longer available
void updateSectionSeats (int numSeats, int section[5][10], int availableSeat[]) {

	// availableSeat array contains the values for row and seat but they need to be reduced
	// by one to work with the section arrays
	int row = availableSeat[0] - 1;
	int firstSeat = availableSeat[1] - 1;

	// Update the seats starting at the firstSeat counting forward in row
	// until numSeats are updated
	for (unsigned int seats = firstSeat; seats < firstSeat + numSeats; seats++) {
		section[row][seats] = 1;
	}

} // updateSectionSeats


// Runs through all sections calculates the total sales
void totalVenueSales (int whichConcert,
			          int floor[5][10], int section102[5][10],
		              int section202[5][10], int section302[5][10]) {

	float totalConcertSales = 0;

	// Calculate the sales for the sections
	for (size_t row = 0; row < ROWS_PER_SECTION; row++) {
		for (size_t seat = 0; seat < SEATS_PER_ROW; seat++) {
			if (floor[row][seat] == 1) {
				totalConcertSales = totalConcertSales + FLOOR_PRICE;
			}
			if (section102[row][seat] == 1) {
				totalConcertSales = totalConcertSales + SECTION_102_PRICE;
			}
			if (section202[row][seat] == 1) {
				totalConcertSales = totalConcertSales + SECTION_202_PRICE;
			}
			if (section302[row][seat] == 1) {
				totalConcertSales = totalConcertSales + SECTION_302_PRICE;
			}
		}
	}

	char band[100] = " ";

	puts ("");
	if (whichConcert == ROLLING_STONES) {
		strcpy (band, "Rolling Stones");
	}
	else if (whichConcert == IMAGINE_DRAGONS) {
		strcpy (band, "Imagine Dragons");
	}
	else {
		strcpy (band, "U2");
	}

	printf ("The total sales for the %s is $%.2f\n", band, totalConcertSales);

} // totalVenueSales



/************ FROM ASSIGNMENT 5 **************/


// Displays the concert menu.
// Prompts customer for desired concert and returns selected concert (group).
int displayConcertMenu (void) {

	int concert;   // Which concert customer wants to attend

	// Display menu and check for valid menuOption.
	// Here I use a while loop.  I display the menu before the loop and get user
	// input before loop.  Loop only executes if the user's input is not valid.
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


// Displays the sections and prices based on the group customer wants to see.
// Prompts user for desired section and returns selected section
int getSection (int whichConcert) {

	int section;   	// Which section the user wants to sit

	// Display section menu and check for valid selection.
	// Again,  I use a while loop.  I display the menu before the loop and get user
	// input before loop.  Loop only executes if the user's input is not valid.
	puts ("");
	puts ("-------------------------------------------");
	puts ("     Section             Price");
	puts ("-------------------------------------------");
	printf ("0: Floor           $%.2f\n", FLOOR_PRICE);
	printf ("1: Section 102     $%.2f\n", SECTION_102_PRICE);
	printf ("2: Section 202     $%.2f\n", SECTION_202_PRICE);
	printf ("3: Section 302     $%.2f\n", SECTION_302_PRICE);
	puts ("-------------------------------------------");

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
		   section != SECTION_102 &&
		   section != SECTION_202 &&
		   section != SECTION_302) {

			printf ("%d is not a valid section.  Please try again. ", section);
			scanf ("%d", &section);
	}

	return section;

} // getSection


// Prompts the user for the number of seats they want for the selected concert
// The user can select between 1 and 6 seats.
int getNumberOfSeats (int whichConcert) {

	// Example of using a while loop. Although a do while works, a while
	// loop also fits the pattern needed.  This time I don't display
	// anything before the loop executes because I don't want to repeat code.
	// Instead I setup the loop condition to ensure it will enter at least once by
	// initializing numSeats to ensure loop entry.
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
float ticketCost (int numSeats, int section, int row, int seat) {

	int sectionNumber;			// Stores section constant converted into section number
	float seatCost = 0;  		// Price of one seat for a specific group
	float totalSeatCost = 0;    // Price for all seats
	float serviceFeeTaxes = 0;	// Price for service fees and taxes per ticket
	float totalCost = 0; 		// Total cost for all seats including service fee and taxes

	// Determine section number and seat cost based on section customer selected
	if (section == FLOOR) {
		seatCost = FLOOR_PRICE;
	}
	else if (section == SECTION_102) {
		seatCost = SECTION_102_PRICE;
		sectionNumber = 102;
	}
	else if (section == SECTION_202) {
		seatCost = SECTION_202_PRICE;
		sectionNumber = 202;
	}
	else {
		seatCost = SECTION_302_PRICE;
		sectionNumber = 302;
	}

	// Compute the different costs
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
			printf ("%d\t\tFloor\t%d\t%d\t$%.2f\t\t$%.2f\n", numSeats, row, seat, seatCost, totalSeatCost);
		}
		else {
			printf ("%d\t\t%d\t%d\t%d\t$%.2f\t\t$%.2f\n", numSeats, sectionNumber, row, seat, seatCost, totalSeatCost);
		}
	}
	else {
		// If more than one seat is selected, need to print out seats as a range
		// Also if the seats are on the floor, need to print out the section as a character
		if (section == FLOOR) {
			printf ("%d\t\tFloor\t%d\t%d-%d\t$%.2f\t\t$%.2f\n", numSeats, row, seat, seat+(numSeats-1), seatCost, totalSeatCost);
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


// Prompts customer to purchase tickets.  Starts a timer.  If customer doesn't select to purchase
// tickets within 30 seconds, then void the order, otherwise charge credit card.
bool purchaseTickets(float totalCost) {

	time_t startTime;	// Captures time when starts purchase
	time_t stopTime;	// Captures time when customer decides
	int timeDifference; // Difference between the start time and when user makes decision
	char yesNoInput;
	bool purchase;

	// Start the timer
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

	// If customer decided to purchase tickets, stop the timer and ensure 30 seconds did not elapse
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

	// Print this information for each ticket
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

