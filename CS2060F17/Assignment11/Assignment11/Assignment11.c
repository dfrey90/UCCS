/*
Daniel Frey
CS 2060-002
Assignment 11
12-5-17
This program will use an array of structures and linked list to answer questions about the structures
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable : 4996)

//number constants for racer info
#define NUM_RACE_RESULTS 3
#define NUM_RACERS 4
const int PIKES_PEAK = 0;
const int GOTG = 1;
const int BOULDER = 2;

//racer struct
typedef struct Racer
{
	//name, results(array of 3), overall (function)
	char name[20];
	int results[NUM_RACE_RESULTS];
	int overall;
}Racer;

//race struct
typedef struct Race
{
	//name, number racers, list racers(array of 4)
	char name[30];
	int numRacers;
	Racer racers[NUM_RACERS];
}Race;

//racer node struct
typedef struct RacerNode
{
	//name, list results, overall, next racer
	char name[20];
	int results[NUM_RACE_RESULTS];
	int overall;
	struct RacerNode * nextRacerPtr;
}RacerNode;

//function prototypes
void raceInfo();
int computeOverall(int value1, int value2, int value3);
void printArray(Race *races);
void pikesPeakAscentFinish(Race race);
void bestWorstAllRaces(Race *races);
RacerNode *createRacerNode(Racer racer);
void addRacerNode(RacerNode **headPtr, RacerNode *node);
void printLinkedList(RacerNode *headPtr);

int main(void)
{
	raceInfo();

	return 0;
}

//starting point for the race info, calls other functions to complete
void raceInfo()
{
	//create 4 racer structures
	Racer racer1 = { "Sue", { 25, 7, 4 }, computeOverall(racer1.results[PIKES_PEAK], racer1.results[GOTG], racer1.results[BOULDER]) };
	Racer racer2 = { "Stacy", { 81, 3, 9 }, computeOverall(racer2.results[PIKES_PEAK], racer2.results[GOTG], racer2.results[BOULDER]) };
	Racer racer3 = { "Betty", { 71, 82, 45 }, computeOverall(racer3.results[PIKES_PEAK], racer3.results[GOTG], racer3.results[BOULDER]) };
	Racer racer4 = { "Joe", { 29, 4, 2 }, computeOverall(racer4.results[PIKES_PEAK], racer4.results[GOTG], racer4.results[BOULDER]) };

	//create 3 races structures
	Race race1 = { "Pikes Peak Ascent", NUM_RACERS, { racer1, racer2, racer3, racer4 } };
	Race race2 = { "Garden of the Gods 10-mile", NUM_RACERS, { racer1, racer2, racer3, racer4 } };
	Race race3 = { "Boulder Boulder", NUM_RACERS, { racer1, racer2, racer3, racer4 } };

	//array of races
	Race races[] = { race1, race2, race3 };

	//headPtr for linked list
	RacerNode *headPtr = NULL;

	//create nodes for racers
	RacerNode *racer1Node = createRacerNode(racer1);
	RacerNode *racer2Node = createRacerNode(racer2);
	RacerNode *racer3Node = createRacerNode(racer3);
	RacerNode *racer4Node = createRacerNode(racer4);

	//add racer nodes to linked list in specified order: Sue, Stacy, Betty, Joe
	addRacerNode(&headPtr, racer1Node);
	addRacerNode(&headPtr, racer2Node);
	addRacerNode(&headPtr, racer3Node);
	addRacerNode(&headPtr, racer4Node);

	//print races array
	printArray(races);

	//best worst finish pikes peak
	pikesPeakAscentFinish(race1);

	//best worst in all 3 races
	bestWorstAllRaces(races);

	//print linked list of racer nodes
	printLinkedList(headPtr);
}

//computes the overall value for each racer
int computeOverall(int value1, int value2, int value3)
{
	return value1 + value2 + value3;
}

void printArray(Race *races)
{
	//for each race
	for (size_t race = 0; race < (size_t)NUM_RACE_RESULTS; race++)
	{
		//print race name
		printf("%s\n", races[race].name);
		printf("%s\t%s\n", "Racer", "Finish");
		puts("---------------------------");

		//for each racer per race
		for (size_t racer = 0; racer < (size_t)NUM_RACERS; racer++)
		{
			//print racer name and result
			printf("%s\t%d\n", races[race].racers[racer].name, races[race].racers[racer].results[race]);
		}
		
		puts("");
	}
}

//displays the best and worst finishers for Pikes Peak Ascent race
void pikesPeakAscentFinish(Race race)
{
	Racer bestFinisher, worstFinisher;

	//default to first racer
	bestFinisher = race.racers[PIKES_PEAK];
	worstFinisher = race.racers[PIKES_PEAK];

	//******************************************************************************************
	for (size_t racer = 1; racer < (size_t)NUM_RACERS; racer++)
	{
		//if best is higher than next, then next is best; else if worst is lower than next, then worst is next
		if (bestFinisher.results[PIKES_PEAK] > race.racers[racer].results[PIKES_PEAK])
		{
			bestFinisher = race.racers[racer];
		}
		else if (worstFinisher.results[PIKES_PEAK] < race.racers[racer].results[PIKES_PEAK])
		{
			worstFinisher = race.racers[racer];
		}
	}

	//display best and worst finishers for Pikes Peak Ascent
	printf("Best finisher at %s = > %s finishing %d\n", race.name, bestFinisher.name, bestFinisher.results[PIKES_PEAK]);
	printf("Worst finisher at %s = > %s finishing %d\n\n", race.name, worstFinisher.name, worstFinisher.results[PIKES_PEAK]);
}

//finds the best and worst finish out of the 3 different races
void bestWorstAllRaces(Race *races)
{
	int bestRace = 0, bestRacer = 0, bestResult = 0, worstRace = 0, worstRacer = 0, worstResult = 0;

	//loop through each remaining racer
	for (size_t racer = 0; racer < (size_t)NUM_RACERS; racer++)
	{
		//test each result for the racer, race and result both increment
		for (size_t race = 0, result = 0; race < (size_t)NUM_RACE_RESULTS; race++, result++)
		{
			//if best is higher than next, then next is best; else if worst is lower than next, then worst is next; else nothing
			if (races[bestRace].racers[bestRacer].results[bestResult] > races[race].racers[racer].results[result])
			{
				//assign best finish overall and the indices for specific race/racer/result
				bestRace = race;
				bestRacer = racer;
				bestResult = result;
			}
			else if (races[worstRace].racers[worstRacer].results[worstResult] < races[race].racers[racer].results[result])
			{
				//assign worst finish overall and the indices for specific race/racer/result
				worstRace = race;
				worstRacer = racer;
				worstResult = result;
			}
		}
	}

	printf("Best finisher for the 3 races occured at %s = > %s finishing %d\n", races[bestRace].name, races[bestRace].racers[bestRacer].name, races[bestRace].racers[bestRacer].results[bestResult]);
	printf("Worst finisher for the 3 races occured at %s = > %s finishing %d\n\n", races[worstRace].name, races[worstRace].racers[worstRacer].name, races[worstRace].racers[worstRacer].results[worstResult]);
}

//creates a node for a racer
RacerNode *createRacerNode(Racer racer)
{
	//allocate memory for node
	RacerNode *newNodePtr = (RacerNode*)malloc(sizeof(RacerNode));

	//assign values from racer to racer node
	strcpy(newNodePtr->name, racer.name);
	memcpy(newNodePtr->results, racer.results, NUM_RACE_RESULTS * sizeof(int));
	newNodePtr->overall = racer.overall;
	newNodePtr->nextRacerPtr = NULL;

	return newNodePtr;
}

//adds a racer node to a linked list of racers, sorted by overall in ascending order
void addRacerNode(RacerNode **headPtr, RacerNode *nodeToAdd)
{
	RacerNode *previousPtr = NULL, *currentPtr = *headPtr;

	//find location to insert node into
	//while not end of list and current overall is <= node to add overall
	while (currentPtr != NULL && currentPtr->overall <= nodeToAdd->overall)
	{
		previousPtr = currentPtr;
		currentPtr = currentPtr->nextRacerPtr;
	}

	//previous points to node before -- if NULL, insert as 1st node
	//current points to node after
	if (previousPtr == NULL)
	{
		*headPtr = nodeToAdd;
	}
	else
	{
		previousPtr->nextRacerPtr = nodeToAdd;
	}

	nodeToAdd->nextRacerPtr = currentPtr;
}

//prints the linked list
void printLinkedList(RacerNode *headPtr)
{
	RacerNode *currentPtr = headPtr;

	//if a list exists, else DNE
	if (headPtr != NULL)
	{
		puts("The list in order from lowest to highest overall is:");

		//while not end of list
		while (currentPtr != NULL)
		{
			//name {result1, result2, result3} -->
			printf("%s {%d, %d, %d} --> ", currentPtr->name, currentPtr->results[PIKES_PEAK], currentPtr->results[GOTG], currentPtr->results[BOULDER]);

			//advance to next item
			currentPtr = currentPtr->nextRacerPtr;
		}

		//if reached end of list print NULL
		if (currentPtr == NULL)
		{
			printf("%s", "NULL\n");
		}
	}
	else
	{
		puts("The list is empty.");
	}
}