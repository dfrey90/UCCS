#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024
#define NUM_THREADS 6

int total = 0;
int n1,n2;
int active = 0;
char *s1,*s2;
FILE *fp;

int readf(FILE *fp)
{
        if((fp=fopen("strings.txt", "r"))==NULL){
                printf("ERROR: can't open string.txt!\n");
                return 0;
        }
        s1=(char *)malloc(sizeof(char)*MAX);
        if(s1==NULL){
                printf("ERROR: Out of memory!\n");
                return -1;
        }
        s2=(char *)malloc(sizeof(char)*MAX);
        if(s2==NULL){
                printf("ERROR: Out of memory\n");
                return -1;
        }
        /*read s1 s2 from the file*/
        s1=fgets(s1, MAX, fp);
        s2=fgets(s2, MAX, fp);
        n1=strlen(s1)-1;  /*length of s1*/
        n2=strlen(s2)-1; /*length of s2*/

        if(s1==NULL || s2==NULL || n1<n2)  /*when error exit*/
                return -1;
}

void* substring_thread(void* i){

	int j, k, count = 0;
	for(j = (int)i, k = 0; k < n2; j++,k++){
		if(*(s1+j)!=*(s2+k)){
			pthread_exit(NULL);
		}
		else count++;
		if(count == n2) total++;
	}

	pthread_exit(NULL);
}

int num_substring(void)
{
	int i,j,k;
	pthread_t *threads = (pthread_t*)(malloc(sizeof(pthread_t)*(n1 - n2 + 1)));
	pthread_attr_t a;
	pthread_attr_init(&a);
	for(i = 0; i <= (n1-n2); i++){
		int rc = pthread_create(&threads[i], &a, substring_thread, (void*)(i));
		if(rc) exit(rc);
	}
	for(i = 0; i <= (n1-n2); i++){
		pthread_join(threads[i], NULL);
	}
	return total;
}

int main(int argc, char * argv[]){
	readf(fp);
	num_substring();
	while(active);
	printf("%d\n", total);
	pthread_exit(NULL);	
}
