#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024
#define BUFF_SIZE 10
const char* FILE_NAME = "message.txt";


int buffPos = 0, msgPos = 0;
int msg_sz;
char *msg, buffer[BUFF_SIZE];
FILE *fp;

pthread_mutex_t buffPos_mutex;
pthread_cond_t prod_cond, cons_cond;	//condition variables for producer and consumer

//Read file to get message from it
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

	/*read message from the file*/
	msg = fgets(msg, MAX, fp);

	msg_sz = strlen(msg);  /*length of message*/
}

//Produces items from message to buffer 
void *producer(void *ptr)
{
	int i;	//for loop counter
	
	//produce as long as there are items left in string
	for (i = 0; i <= msg_sz; i++)
	{
		//keep buffer position from changing while producing
		pthread_mutex_lock(&buffPos_mutex);
	
		//wait on consumer if buffer is full to produce more
		while (buffPos == BUFF_SIZE)
		{
			pthread_cond_wait(&prod_cond, &buffPos_mutex);
		}
	
		//copy char from message to buffer
		buffer[buffPos] = msg[msgPos];
	
		//increment both positions to next available
		buffPos++;
		msgPos++;
	
		//send signal to consumer that items in buffer
		pthread_cond_signal(&cons_cond);
	
		//unlock hold on mutex
		pthread_mutex_unlock(&buffPos_mutex);
	}
	
	pthread_exit(NULL);
}

//Consumes items from buffer and prints
void *consumer(void *ptr)
{
	//lock buff pos while consumer has control of it
	pthread_mutex_lock(&buffPos_mutex);
	
	//while not finished going through message completely
	while (msgPos < msg_sz)
	{
		//wait if buffer empty (cannot consume if nothing produced)
		pthread_cond_wait(&cons_cond, &buffPos_mutex);
		
		//print from buffer
		printf("%s", buffer);
		
		//reset buffer position back to beginning since empty now
		buffPos = 0;
		
		//tell producer to produce more items
		pthread_cond_signal(&prod_cond);
	}
	
	//unlock hold on mutex since done
	pthread_mutex_unlock(&buffPos_mutex);
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	//thread vars for producer and consumer
	pthread_t prod, cons;
	
	//read file and populate message
	readf(fp);
	
	//initialize condition vars and mutex
	pthread_mutex_init(&buffPos_mutex, NULL);
	pthread_cond_init(&prod_cond, NULL);
	pthread_cond_init(&cons_cond, NULL);
	
	//create prod and cons threads
	pthread_create(&prod, NULL, producer, (void *)1);
	pthread_create(&cons, NULL, consumer, (void *)2);
	
	//wait for threads to complete
	pthread_join(prod, NULL);
	pthread_join(cons, NULL);
	
	pthread_mutex_destroy(&buffPos_mutex);
	pthread_cond_destroy(&prod_cond);
	pthread_cond_destroy(&cons_cond);
	
	pthread_exit(NULL);
}
