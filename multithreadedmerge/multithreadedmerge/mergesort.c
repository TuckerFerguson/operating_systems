#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>

#include "mergesort.h"


#define TRUE 1
#define FALSE 0

// function prototypes
void *thread_mergesort(void*);
void merge(int data[], int beginIndex, int pivotIndex, int endIndex);
void insertion_sort(int data[], int beginIndex, int endIndex);
void alt_mergesort(int[], int, int);

int *global_data;
int num_threads = 0;
long max_threads = 0 ;
sem_t * mutex;

const int INSERTION_SORT_CUTOFF = 100; //based on trial and error
/*
 * insertion_sort(int data[], int beginIndex, int endIndex):
 *
 * description: Sort the section of the array data[beginIndex..endIndex].
 */
void insertion_sort(int data[], int beginIndex, int endIndex) 
{
	int j;

	for (j=beginIndex+1; j<=endIndex; j++) {
		int key = data[j];
		int i = j-1;
		while ((i > beginIndex-1) && (data[i] > key)) {	
			data[i+1] = data[i];
			i--;
		}
		data[i+1] = key;
	}
}

void serial_mergesort(int data[], int beginIndex, int endIndex)
{
    mutex = sem_open("mutex", O_CREAT);
    if (mutex == SEM_FAILED){
        printf("Semaphore failed to initialize\n");
        printf("errno %d\n0", errno);
        exit(EXIT_FAILURE);
    }
    global_data = data;
    max_threads = sysconf(_SC_NPROCESSORS_CONF);
    
    void **arr = {data, (void*)beginIndex, (void*)endIndex};

//    struct thread_args *inital_args = (struct thread_args *)malloc(sizeof(struct thread_args));
//    inital_args->beginIndex = beginIndex;
//    inital_args->endIndex = endIndex;
    
    (*thread_mergesort)((void*)inital_args);
}

/*
 * serial_mergesort(int data[], int beginIndex, int endIndex):
 *
 * description: Sort the section of the array data[beginIndex..endIndex].
 */
void *thread_mergesort(void * voidArgs)
{
    int *args = (int*)voidArgs;
    int beginIndex = args[0];
    int endIndex = args[1];
    
    pthread_t thread1, thread2;

//	if (endIndex-beginIndex+1 <= INSERTION_SORT_CUTOFF)  {
//        insertion_sort(global_data,beginIndex,endIndex);
//        return NULL;
//	} else {
        int pivotIndex = (beginIndex+endIndex)/2;
    
        if (num_threads <= (int)(max_threads/2))
        {
            sem_wait(mutex);
            ++num_threads;
            sem_post(mutex);
            printf("Created thread #%d", num_threads);
            
            int thread1_args[] = {beginIndex, pivotIndex};
            int thread2_args[] = {pivotIndex+1, endIndex};

            pthread_create(&thread1, NULL, thread_mergesort, (void*)thread1_args);
            pthread_create(&thread2, NULL, thread_mergesort, (void*)thread2_args);
            
            pthread_join(thread1, NULL);
            pthread_join(thread2, NULL);
            merge(global_data, beginIndex, pivotIndex, endIndex);
        }
        else
        {
            alt_mergesort(global_data, beginIndex, pivotIndex);
            alt_mergesort(global_data, pivotIndex+1, endIndex);
            merge(global_data, beginIndex, pivotIndex, endIndex);
        }
        return NULL;
//    }
}

void alt_mergesort(int A[], int p, int r)
{
    if (r-p+1 <= INSERTION_SORT_CUTOFF)  {
        insertion_sort(A,p,r);
    } else {
        int q = (p+r)/2;
        alt_mergesort(A, p, q);
        alt_mergesort(A, q+1, r);
        merge(A, p, q, r);
    }
}

/*
 * merge(int data[], int beginIndex, int pivotIndex, int endIndex):
 *
 * description: Merge two sorted sequences data[beginIndex..pivotIndex] and data[pivotIndex+1..endIndex] 
 *              and place merged output back in array data. Uses extra
 *              space proportional to data[beginIndex..endIndex].
 */
void merge(int data[], int beginIndex, int pivotIndex, int endIndex)
{
	int *copyOfData = (int *) malloc(sizeof(int) * (endIndex-beginIndex+1));
		
	int i = beginIndex;
	int j = pivotIndex+1;
	int k = 0;
	int l;
		
	// as long as both lists have unexamined elements
	// this loop keeps executing.
	while ((i <= pivotIndex) && (j <= endIndex)) {
		if (data[i] < data[j]) {
			copyOfData[k] = data[i];
			i++;
		} else {
			copyOfData[k] = data[j];
			j++;
		}
		k++;
	}
		
	// now only at most one list has unprocessed elements.
		
	if (i <= pivotIndex) { 
		// copy remaining elements from the first list
		for (l=i; l<=pivotIndex; l++) {
			copyOfData[k] = data[l];
			k++;
		}
	} else {
		// copy remaining elements from the second list
		for (l=j; l<=endIndex; l++) {
			copyOfData[k] = data[l];
			k++;
		}
	}
		
	// copy merged output from array copyOfData back to array data
	k=0;
	for (l=beginIndex; l<=endIndex; l++) {
		data[l] = copyOfData[k];
		k++;
	}

	free(copyOfData);
}