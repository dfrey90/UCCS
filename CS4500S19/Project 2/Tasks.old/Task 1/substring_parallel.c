#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

//Constants for file line buffer, max num threads, and file name
#define MAX 1024
#define NUM_THREADS 4
const char* FILE_NAME = "strings.txt";

//Global vars for matches, string info, and file pointer
int total = 0;
int n1, n2;
char *s1, *s2;
FILE *fp;

//Struct for thread info of string on thread num and current count
typedef struct threadData
{
	char *str1;	//partitioned string to search
	char *str2;	//string to find in str1
} thdData;

//Reads strings from file
int readf(FILE *fp)
{
	char *str_ptr;	//ptr to facilitate removal of newline char at end of strings from file
	
	if((fp = fopen(FILE_NAME, "r")) == NULL)
	{
		printf("ERROR: can't open %s!\n", FILE_NAME);
		return 0;
	}

	s1 = (char *)malloc(sizeof(char)*MAX);

	if(s1 == NULL)
	{
		printf("ERROR: Out of memory!\n");
		return -1;
	}

	s2 = (char *)malloc(sizeof(char)*MAX);

	if(s2 == NULL)
	{
		printf("ERROR: Out of memory\n");
		return -1;
	}

	/*read s1 s2 from the file and stores in global vars*/
	s1 = fgets(s1, MAX, fp);
	s2 = fgets(s2, MAX, fp);

	//calculate lengths of strings and store in global vars
	n1 = strlen(s1)-1; /*length of s1 excluding newline char*/
	n2 = strlen(s2)-1; /*length of s2 excluding newline char*/
	
	//get rid of newline char at end
	str_ptr = s1;
	str_ptr[n1] = 0;
	
	str_ptr = s2;
	str_ptr[n2] = 0;
	
	printf("Strings from file:\n\ts1: %s, s2: %s\n", s1, s2);
	
	/*when error exit*/
	if(s1 == NULL || s2 == NULL || n1 < n2)
		return -1;
}

//Function to find the number of substrings given s1 and s2
void *num_substring_parallel(void *ptr)
{
	int i, j, k, count, str_sz1, str_sz2;
	char *str_data1, *str_data2;
	thdData *thd_data;
	
	//get info from data attached to thread
	thd_data  = (thdData *)ptr;
	str_data1 = thd_data->str1;
	str_data2 = thd_data->str2;
	str_sz1 = strlen(str_data1);
	str_sz2 = strlen(str_data2);
	
	//printf("sd1: %s, sd2: %s\n", str_data1, str_data2);
	//printf("nd1: %d, nd2: %d\n", str_sz1, str_sz2);

	//loop through string of current thread checking for matches
	for (i = 0; i <= (str_sz1 - str_sz2); i++)
	{   
		count = 0;

		 /*search for the next string of size of n2*/ 
		for(j = i, k = 0; k < str_sz2; j++, k++)
		{ 
			if (*(str_data1 + j) != *(str_data2 + k))
			{
				break;
			}
			else
				count++;

			if(count == str_sz2)
			{
				total++;  /*find a substring in this step*/
			}
		}
	}
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	//vars to work with threads and strings
	int i, rc, offset, repeat, currChar = 0;
	char *str;

	//vars for threads and thread data for each thread
	pthread_t thds[NUM_THREADS];
	thdData thd_data[NUM_THREADS];
	
	//read file and get s1 and s2
	readf(fp);
	
	offset = (n1/NUM_THREADS) + n2;

	//assign thread numbers to thread infos and create threads
	for (i = 0; i < NUM_THREADS; i++)
	{
		//split s1 to make substring for thread
		str = strndup(s1 + currChar, offset);
		
		//assign strings to thread data; string to search and search string
		thd_data[i].str1 = str;
		thd_data[i].str2 = s2;

		//create threads
		rc = pthread_create(&thds[i], NULL, num_substring_parallel, (void *) &thd_data[i]);
	
		if (rc)
		{
			printf("ERROR: return code from pthread_create() is %d.\n", rc);
			exit(-1);
		}
		
		printf("String being searched in thread[%d]:\n\tstr1: %s, str2: %s\n", i, str, s2);
		
		//determine num chars to repeat to address partition problem
		repeat = n2 - 1;

		//update current character position
		currChar += offset - repeat;
	}

	//wait for all threads to finish
	for (i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(thds[i], NULL);
	}

	//display total matches
 	printf("The number of substrings is: %d\n", total);

	pthread_exit(NULL);
}
