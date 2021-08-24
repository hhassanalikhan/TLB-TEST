#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include<math.h>

// source test2Ru.sh
//  gcc -o a test2.c -lm



static void die(const char *fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	fputc('\n', stderr);
	exit(1);
}

int main () {

    int tableSize = 2; //at least 1 kb page size 
    int i;
    int * myArr;
    int maxPageSize = 30;
    int * map;
    unsigned int flags = MAP_PRIVATE | MAP_ANONYMOUS;

    for( ; tableSize <= maxPageSize; tableSize++)
    {

        int pageSize = (pow(2,tableSize));
        int sizeUnit = 0;
        unsigned long int arraySize = pageSize;
        while (pageSize/1024 >= 1)
        {
            pageSize = pageSize/1024;
            sizeUnit += 1;
        }

        printf("Pagesize: %d" ,(int)pageSize);
        // break;

        if (sizeUnit == 0)
        {
            printf("B\n");
        }
        else if (sizeUnit == 1)
        {
            printf("KB\n");
        }
        else if (sizeUnit == 2)
        {
            printf("MB\n");
        }
        else if (sizeUnit == 3)
        {
            printf("GB\n");
        }
        else if (sizeUnit == 4)
        {
            printf("TB\n");
        }


        
        int arraySizeInt  = (int)arraySize/4;
        int incrementVal = (int)(arraySize/256);
        // create array of pagesize 
        map = mmap(map, arraySize, PROT_READ | PROT_WRITE, flags, -1, 0);
        // map = (int *) malloc(arraySize);
        int stride = incrementVal;
        // int stride = 5;

        if (stride < 1)
            stride = 1;

        // printf("\t %d\n",stride);
        int totalMemoryOperations = 0;
        double time_spent = 0;
        


        struct timespec tstart={0,0}, tend={0,0};
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        
        
        for(int reps = 0 ;reps < 1000; reps+=1)
        {
            for(int reps2 = 0 ;reps2 < 1000; reps2+=1)
            {
                
                unsigned int * lastpos = map;
                for (int off = 0; off < arraySizeInt; off += stride) {
                    
                    lastpos = map + off;
                    *lastpos = off + stride;
                    
                    totalMemoryOperations+=2;
                    // printf("             \n %d",off);
                }
                
                *lastpos = 0;
                // free(map);
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &tend);
        // clock_t end2 = clock();
        // time_spent += (double)(end2 - begin);
        printf("\t Memory Accesses: %d\n",totalMemoryOperations);
        printf("\t Total access time: %.5f seconds\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
        munmap(map,arraySize);

            
    }
    return 0;
}

