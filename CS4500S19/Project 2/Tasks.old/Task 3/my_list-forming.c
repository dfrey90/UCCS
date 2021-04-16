/*
  my_list-forming.c: 
  Each thread generates K nodes and stores in a "local" list, then adds to the global list all at once.
  There are num_threads threads. The value of "num_threads" is input by the student.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sched.h>

#define K 200 // genreate a data node for K times in each thread

struct Node
{
    int data;
    struct Node* next;
};

struct list
{
     struct Node * header;
     struct Node * tail;
};

pthread_mutex_t    mutex_lock;

struct list *List;	//global list of nodes
struct list *Local;	//local list of nodes for a thread

void bind_thread_to_cpu(int cpuid) {
     cpu_set_t mask;
     CPU_ZERO(&mask);

     CPU_SET(cpuid, &mask);
     if (sched_setaffinity(0, sizeof(cpu_set_t), &mask)) {
         fprintf(stderr, "sched_setaffinity");
         exit(EXIT_FAILURE);
     }
}

struct Node* generate_data_node()
{
    struct Node *ptr;
    ptr = (struct Node *)malloc(sizeof(struct Node));    

    if( NULL != ptr ){
        ptr->next = NULL;
    }
    else {
        printf("Node allocation failed!\n");
    }
    return ptr;
}

void * producer_thread( void *arg)
{
    //do not bind, allow to run on all CPUs
    //bind_thread_to_cpu(*((int*)arg));//bind this thread to a CPU

    struct Node * ptr, tmp;
    int counter = 0;  

    /* generate and attach K nodes to the global list */
    while( counter  < K )
    {
        ptr = generate_data_node();

        if( NULL != ptr )
        {
		ptr->data  = 1;//generate data
		
		/* attach the generated node to the Local list */
		if( Local->header == NULL )
                {
			Local->header = Local->tail = ptr;
                }
                else
                {
			Local->tail->next = ptr;
                        Local->tail = ptr;
                }
        }
        
        ++counter;
    }
    
    /* add all nodes from Local list to global List */
    //instead of try-lock, which can waste CPU cycles, do all at once
    pthread_mutex_lock(&mutex_lock);
    
    //if the global list is empty copy whole list, else append to end of global list
    if( List->header == NULL )
    {
	List->header = Local->tail;
	List->tail = Local->tail;
    }
    else
    {
	List->tail->next = Local->header;
	List->tail = Local->tail;
    }
    
    pthread_mutex_unlock(&mutex_lock);
}


int main(int argc, char* argv[])
{
    int i, num_threads;

    int NUM_PROCS;//number of CPU
    int* cpu_array = NULL;

    struct Node  *tmp,*next;
    struct timeval starttime, endtime;

    if(argc == 1){
        printf("ERROR: please provide an input arg (the number of threads)\n");
        exit(1);
    }

    num_threads = atoi(argv[1]); //read num_threads from user
    pthread_t producer[num_threads];
    NUM_PROCS = sysconf(_SC_NPROCESSORS_CONF);//get number of CPU
    if( NUM_PROCS > 0)
    {
        cpu_array = (int *)malloc(NUM_PROCS*sizeof(int));
        if( cpu_array == NULL )
        {
            printf("Allocation failed!\n");
            exit(0);
        }
        else
        {
            for( i = 0; i < NUM_PROCS; i++)
               cpu_array[i] = i;
        }

    }

    pthread_mutex_init(&mutex_lock, NULL);

    List = (struct list *)malloc(sizeof(struct list));
    if( NULL == List )
    {
       printf("End here\n");
       exit(0);	
    }
    List->header = List->tail = NULL;
    
    //initialize local list
    Local = (struct list *)malloc(sizeof(struct list));
    if( NULL == Local )
    {
       printf("End here\n");
       exit(0);	
    }
    Local->header = Local->tail = NULL;

    gettimeofday(&starttime,NULL); //get program start time
    for( i = 0; i < num_threads; i++ )
    {
        pthread_create(&(producer[i]), NULL, (void *) producer_thread, &cpu_array[i%NUM_PROCS]); 
    }

    for( i = 0; i < num_threads; i++ )
    {
        if(producer[i] != 0)
        {
            pthread_join(producer[i],NULL);
        }
    }


    gettimeofday(&endtime,NULL); //get the finish time

    if( List->header != NULL )
    {
        next = tmp = List->header;
        while( tmp != NULL )
        {  
           next = tmp->next;
           free(tmp);
           tmp = next;
        }            
    }
    
    //free the allocated space for local list
    if( Local->header != NULL )
    {
        next = tmp = Local->header;
        while( tmp != NULL )
        {  
           next = tmp->next;
           free(tmp);
           tmp = next;
        }            
    }
    
    if( cpu_array != NULL)
       free(cpu_array);
    /* calculate program runtime */
    printf("Total run time is %ld microseconds.\n", (endtime.tv_sec-starttime.tv_sec) * 1000000+(endtime.tv_usec-starttime.tv_usec));
    return 0; 
}
