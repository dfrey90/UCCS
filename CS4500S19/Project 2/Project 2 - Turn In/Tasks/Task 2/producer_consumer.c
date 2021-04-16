#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024
#define BUFF_SIZE 10
const char* FILE_NAME = "message.txt";


int buffPos = 0;  //keeps track of buffer position
char *msg, buffer[BUFF_SIZE];
FILE *fp;

pthread_mutex_t buffPos_mutex;
pthread_cond_t prod_cond, cons_cond;	//condition variables for producer and consumer

/*//Read file to get message from it
int readf(FILE *fp)
{
	if((fp = fopen(FILE_NAME, "r")) == NULL)
	{
		printf("ERROR: can't open %s!\n", FILE_NAME);
		return 0;
	}

	msg = (char *)malloc(sizeof(char)*MAX);

	if(msg == NULL)
	{
		printf("ERROR: Out of memory!\n");
		return -1;
	}

	//read message from the file
	msg = fgets(msg, MAX, fp);

	msg_sz = strlen(msg);  //length of message
}*/

//Produces items from message to buffer 
void *producer(void *ptr)
{	
	int i = 0;
	
	while (!feof((FILE *)ptr))
	{
		pthread_mutex_lock(&buffPos_mutex);
		
		//while buffer is full wait
		while (buffPos == BUFF_SIZE)
		{
			pthread_cond_wait(&prod_cond, &buffPos_mutex);
		}
		
		if (i == BUFF_SIZE)
		{
			i = 0;
		}
		
		//add char to buffer
		buffer[i++] = fgetc((FILE *)ptr);
		
		pthread_cond_signal(&cons_cond);
		pthread_mutex_unlock(&buffPos_mutex);
		
		//increment buffPos since char added
		buffPos++;
	}
	
	pthread_exit(NULL);
}

//Consumes items from buffer and prints
void *consumer(void *ptr)
{	
	FILE *output = (FILE *)ptr;
	
	int i = 0;
	
	while (!feof(fp) || buffPos)
	{
		pthread_mutex_lock(&buffPos_mutex);
		
		if (i == BUFF_SIZE)
		{
			i = 0;
		}
		
		//while buffer is empty
		while (buffPos == 0)
		{
			pthread_cond_wait(&cons_cond, &buffPos_mutex);
		}
		
		//check to not attempt to print out eof char
		if (buffer[i] != -1)
		{
			fprintf(output, "%c", buffer[i++]);
		}
		
		buffPos--;
		
		pthread_cond_signal(&prod_cond);
		pthread_mutex_unlock(&buffPos_mutex);
	}
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	//thread vars for producer and consumer
	pthread_t prod, cons;
	
	//open file for reading
	if((fp = fopen(FILE_NAME, "r")) == NULL)
	{
		printf("ERROR: can't open %s!\n", FILE_NAME);
		return 0;
	}
	
	//initialize condition vars and mutex
	pthread_mutex_init(&buffPos_mutex, NULL);
	pthread_cond_init(&prod_cond, NULL);
	pthread_cond_init(&cons_cond, NULL);
	
	//create prod and cons threads
	pthread_create(&prod, NULL, producer, (void *)fp);
	pthread_create(&cons, NULL, consumer, (void *)stdout);
	
	//wait for threads to complete
	pthread_join(prod, NULL);
	pthread_join(cons, NULL);
	
	pthread_mutex_destroy(&buffPos_mutex);
	pthread_cond_destroy(&prod_cond);
	pthread_cond_destroy(&cons_cond);
	
	pthread_exit(NULL);
}
