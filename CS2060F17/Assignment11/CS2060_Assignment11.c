// Assignment #11 Solution

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CONSTS AND DATA TYPES
#define NUM_RACES 3
#define MAX_NAME_LEN 50

// Structure that represents one racer
struct Racer {
	char name[MAX_NAME_LEN];
	int results[NUM_RACES];
	int overall;
};

// Structure that represents one race
struct Race {
	char raceName[MAX_NAME_LEN];
	int numRacers;;
	struct Racer racerList[4];
};

// Structure used to create a linked list
struct RacerNode {
	char name[MAX_NAME_LEN];
	int results[NUM_RACES];
	int overall;
	struct RacerNode *nextPtr;
};

// Function prototypes
void computeOverall (struct Racer *racer);
void printRaceArray (struct Race raceSeries[]);
void insertNode (struct RacerNode **headPtr, struct Racer racer);
void printList (struct RacerNode *listPtr);


int main(void) {

	// Setup the racers, the races and array of races (race series)
	// Manually do the setup with initialization lists based on data in assignment sheet
	struct Racer sue = {"Sue",   {25, 7, 4}};
	struct Racer stacy = {"Stacy", {81, 3, 9}};
	struct Racer betty = {"Betty", {71, 82, 45}};
	struct Racer joe = {"Joe",   {29, 4, 2}};

	struct Race race1 = {"Pikes Peak Ascent", 4, {sue, stacy, betty, joe}};
	struct Race race2 = {"Garden of Gods 10-Mile", 4, {sue, stacy, betty, joe}};
	struct Race race3 = {"Boulder Boulder", 4, {sue, stacy, betty, joe}};

	struct Race raceSeries[NUM_RACES] = {race1, race2, race3};


	// *************************************************
	// QUESTION #1:
	// Compute overall score for each racer
	// Requires passing structure by reference since default is value
	computeOverall (&sue);
	computeOverall (&stacy);
	computeOverall (&betty);
	computeOverall (&joe);

	//********************************************
	// QUESTION #2:
	// Print out the array
	printRaceArray(raceSeries);

	//****************************************************************************
	// QUESTION #3:
	// Find which friend had the best finish at the Pikes Peak Ascent (raceSeries[0])
	// Start out by setting an index and the best finish to the first racer's finish
	int bestRacerIndex = 0;
	int bestFinish = raceSeries[0].racerList[0].results[0];

	// Figuring out for ONLY the 1st race (raceSeries[0])
	// Look at each racers finish and determine the best
	for (int i = 0; i < raceSeries[0].numRacers; i++) {

		if (raceSeries[0].racerList[i].results[0] < bestFinish) {
			bestFinish = raceSeries[0].racerList[i].results[0];
			bestRacerIndex = i;
		}
	}

	printf ("Best finisher at %s => %s finishing of %d\n",
			raceSeries[0].raceName,
			raceSeries[0].racerList[bestRacerIndex].name,
			bestFinish);


	//*****************************************************************************
	// QUESTION #4
	// Find which friend had the worse finish at Pikes Peak Ascent (raceSeries[0])
	// Start out by setting an index and the worse finish to the first racer's finish
	int worseRacerIndex = 0;
	int worseFinish = raceSeries[0].racerList[0].results[0];

	// Figuring out for ONLY the 1st race (raceSeries[0])
	// Look at each racers finish and determine the worse
	for (int j = 0; j < raceSeries[0].numRacers; j++) {

		if (raceSeries[0].racerList[j].results[0] > worseFinish) {
			worseFinish = raceSeries[0].racerList[j].results[0];
			worseRacerIndex = j;
		}
	}
	printf ("Worst finisher at %s => %s finishing %d\n\n",
			raceSeries[0].raceName,
			raceSeries[0].racerList[worseRacerIndex].name,
			worseFinish);

	//***********************************************************
	// QUESTION #5
	// Find which friend had the best finish out of all 3 races
	// Start out by setting an index for the race where best finish occurred
	// Also set a best racer index and best finish to the first racer's finish
	int raceIndex = 0;
	bestRacerIndex = 0;
	bestFinish = raceSeries[0].racerList[0].results[0];

	// For each race
	for (int race = 0; race < NUM_RACES; race++ ) {

		// Figure out best finish for the CURRENT race
		// Go through finishing places for each racer for the CURRENT race
		for (int racer = 0; racer < raceSeries[race].numRacers; racer++) {

			if (raceSeries[race].racerList[racer].results[race] < bestFinish) {
				bestFinish = raceSeries[race].racerList[racer].results[race];
				bestRacerIndex = racer;
				raceIndex = race;
			}

		} // for each racer
	} // for each race


	printf ("Best finish for the 3 races occurred at %s => %s finishing %d\n",
			raceSeries[raceIndex].raceName,
			raceSeries[raceIndex].racerList[bestRacerIndex].name,
			bestFinish);

	//***********************************************************
	// QUESTION #6
	// Find which friend had the worse finish out of all 3 races
	// Start out by setting an index for the race where worse finish occurred
	// Also set a worse racer index and worse finish to the first racer's finish
	raceIndex = 0;
	worseRacerIndex = 0;
	worseFinish = raceSeries[0].racerList[0].results[0];

	// For each race
	for (int race = 0; race < NUM_RACES; race++ ) {

		// Figure out worse finish for the CURRENT race
		// Go through finishing places for each racer for the CURRENT race
		for (int racer = 0; racer < raceSeries[race].numRacers; racer++) {

			if (raceSeries[race].racerList[racer].results[race] > worseFinish) {
				worseFinish = raceSeries[race].racerList[racer].results[race];
				worseRacerIndex = racer;
				raceIndex = race;
			}

		} // for each racer
	} // for each race

	printf ("Worst finish for the 3 races occurred at %s => %s finishing %d\n\n",
			raceSeries[raceIndex].raceName,
			raceSeries[raceIndex].racerList[worseRacerIndex].name,
			worseFinish);


	//***************************************************************************************
	// QUESTION #7
	// Create a linked list of the friends
	// The friends are placed into the linked list.
	// The nodes created will be placed into based on their overall placing for the 3 races
	// The friends must be placed into the linked list in this order: Sue, Stacy, Betty, then Joe

	struct RacerNode *headPtr = NULL;

    insertNode(&headPtr, sue);
	insertNode(&headPtr, stacy);
	insertNode(&headPtr, betty);
	insertNode(&headPtr, joe);

	//********************************************
	// QUESTION #8
	// Print the linked list
	// Should show friends in the order from the lowest overall to the highest overall
	printList(headPtr);

} //main


// Print details for each race in the race series array.
void printRaceArray (struct Race raceSeries[]) {

	// For each race
	for (int i = 0; i < NUM_RACES; i++) {

		// Print out the information for this race
		printf ("%s\n", raceSeries[i].raceName);
		printf ("Racer\tFinish\n");
		puts ("----------------------");

		// Next display each racer and their result for the race
		for (int j = 0; j < raceSeries[i].numRacers; j++) {
			printf ("%s\t%d\n",raceSeries[i].racerList[j].name, raceSeries[i].racerList[j].results[i]);
		}
		puts ("");
	}
} // printRaceArray


// Good method since have to pass structure by reference!
// Computes the overall for each racer.  Overall is the sum of all race results.
void computeOverall (struct Racer *racer) {

	// For each race
	int overall = 0;
	for (int i = 0; i < NUM_RACES; i++) {
		overall = overall + racer->results[i];
	}

	// Set the racer's overall value
	racer->overall = overall;

} // computeOverall


// Insert a node into the linked list
void insertNode (struct RacerNode **headPtr, struct Racer racer) {

	// Allocate memory for node in the heap and setup node
	struct RacerNode *nodePtr = malloc(sizeof(struct RacerNode));
	if (nodePtr != NULL) {
		strcpy (nodePtr->name, racer.name);
		nodePtr->results[0] = racer.results[0];
		nodePtr->results[1] = racer.results[1];
		nodePtr->results[2] = racer.results[2];
		nodePtr->overall = racer.overall;
		nodePtr->nextPtr = NULL;
	}

	// Figure out where the new node belongs in linked list, create two pointers to help
	struct RacerNode *previousPtr = NULL;
	struct RacerNode *currentPtr = *headPtr;

	// While we haven't reached end of list and current node's overall <= new node's overall
	while (currentPtr != NULL && currentPtr->overall <= racer.overall ) {
		previousPtr = currentPtr;
		currentPtr = currentPtr->nextPtr;
	}

	// previousPtr is pointing to node BEFORE insertion point - if NULL, inserting as 1st node!
	// currentPtr is pointing to node AFTTER insertion point
	// Connect new node into list
	if (previousPtr == NULL) {
		*headPtr = nodePtr;
	}
	else {
		previousPtr->nextPtr = nodePtr;
	}

	nodePtr->nextPtr = currentPtr;

} // insertNode


// Walk down list printing the value in each node
void printList (struct RacerNode *listPtr) {

	// Walk list and print values
	if (listPtr != NULL) {

		puts ("The list in order from lowest to highest overall");
		struct RacerNode *currentPtr = listPtr;
		while (currentPtr != NULL) {
			printf ("%s ", currentPtr->name);
			printf ("{%d %d %d} --> ", currentPtr->results[0], currentPtr->results[1], currentPtr->results[2]);
			currentPtr = currentPtr->nextPtr;
		}
		puts ("NULL");
	}
	else {
		puts ("List is empty");
	} // linked list is empty

} // printList


