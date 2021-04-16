#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

char buffer[BUFFER_SIZE] = {0};
int total_size = 0;
int producing = 1;
pthread_mutex_t mutex;
pthread_cond_t producer_condition, consumer_condition;

FILE* fp;

void* producer(void* path){
	int i= 0;
	while(!feof((FILE*)path)){
		pthread_mutex_lock(&mutex);
		while(total_size == BUFFER_SIZE)
			pthread_cond_wait(&producer_condition, &mutex);
		if(i == BUFFER_SIZE){
			i = 0;
		}
		buffer[i++] = fgetc((FILE*)path);
		pthread_cond_signal(&consumer_condition);
		pthread_mutex_unlock(&mutex);
		total_size++; 
	}
	producing = 0;
	pthread_exit(NULL);
}

void* consumer(void* output){
	FILE* output_file = (FILE*)(output);
	int i = 0;
	while(!feof(fp) || total_size){
		pthread_mutex_lock(&mutex);
		if(i == BUFFER_SIZE){
			i = 0;
		}
		while(total_size == 0)
			pthread_cond_wait(&consumer_condition, &mutex);
		if(buffer[i] != -1)
			fprintf(output_file, "%c", buffer[i++]);

		total_size--;
		pthread_cond_signal(&producer_condition);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL); 
}

int main(void){
	pthread_t producer_thread, consumer_thread;
	pthread_attr_t a;
	pthread_attr_init(&a);
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&producer_condition, NULL);
	pthread_cond_init(&consumer_condition, NULL);
	
	fp=fopen("message.txt", "r");
	
	pthread_create(&producer_thread, &a, producer, (void*)fp);
	pthread_create(&consumer_thread, &a, consumer, (void*)stdout);
	
	pthread_join(producer_thread, NULL);
	pthread_join(consumer_thread, NULL);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&producer_condition);
	pthread_cond_destroy(&consumer_condition);
	printf("\n");
	pthread_exit(NULL);
}
