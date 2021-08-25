#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>



void *print_message_function( void *ptr );

typedef struct custom_thread {
   int id;
   int returnValue;
   pthread_t thread;
   int arrayRange;
} custom_thread;

char * sharedArr;

sem_t mutex; 

int pageSize;
int main()
{
    
    custom_thread * myThreads;
    int totalThreads = 100;
    pageSize = 64*1024;

    int arrSize = totalThreads*pageSize;
    int maxThreads = 4;
    


    sharedArr = (char*) malloc(sizeof(char)*arrSize);
    
    
    myThreads = (custom_thread*) malloc(sizeof(custom_thread)*totalThreads);

    sem_init(&mutex, 0, maxThreads);

    for(int i =0; i < totalThreads; i++)
    {
        myThreads[i].id = i;
        myThreads[i].returnValue = -1;
        myThreads[i].arrayRange = pageSize;
    }

    for(int i =0; i < totalThreads; i++)
    {
        custom_thread * temp = &(myThreads[i]);
        // printf("%p %d\n", temp,temp->id);
        pthread_create( &(myThreads[i].thread), NULL, print_message_function, (void*)temp);
    }
    struct timespec tstart={0,0}, tend={0,0};
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    for(int i =0; i < totalThreads; i++)
    {
        pthread_join( myThreads[i].thread, NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &tend);
    // printf("Total time: %.5f seconds\n",
    //     ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
    //     ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    printf("%.5f\n",
        ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
        ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    return 0;
}

void *print_message_function( void *ptr )
{
    custom_thread *relatedObject;
    relatedObject = (custom_thread *) ptr;
    
    int maxTurns = 1; // change this variable to see the effect of TLB hits
    int totalReaps = 1000;

    int rep = 0;

    while( rep < totalReaps)
    {
        int maxLimit = maxTurns+ rep;
        sem_wait(&mutex);
        while (rep < maxLimit)
        {
            for(int i = relatedObject->id*pageSize; i < (relatedObject->id+1)*pageSize; i++)
            {
                sharedArr[i] = 'a';
            }
            rep += 1;
            
        }
        sem_post(&mutex);

    }
    // printf("Thread %d %d\n", relatedObject->id, relatedObject->id*pageSize);
    // sleep(4);
    
}