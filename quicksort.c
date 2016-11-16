#include <LPC17xx.h>
#include <RTL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "quicksort.h"
#include "array_tools.h"

// You decide what the threshold will be
#define USE_INSERTION_SORT 5


#define TASK_PARAM ((qsort_task_parameters_t*)void_ptr)

// Local prototypes







// =======================================================================
// SWAP FUNCTION USED IN THE PARTITION FUNCTION
// ======================================================================
__inline void swap(array_type* arr, int a, int b){
    array_type tmp = arr[a];
    arr[a] = arr[b];
    arr[b] = tmp;
}



// =======================================================================
// IMPLEMENTS PARTITIONING AND SWAPPING AS DECRIBED IN QUICKSORT ALGO.
// ======================================================================
// It has one input argument which is "Interval"
// interval is defined as <array_interval_t* interval;> and passed from the tasks when this function is called
// When you define inteval in your code using the following
//array_interval_t* interval;
//.....
//cur_params = (qsort_task_parameters_t*) void_ptr;
//interval = &(cur_params->interval);
//.......
/// rest of your code
//The definition of interval is the same for both tasks
// When you want to call the partition functio you should use
// pivot_index = partition(interval)

int partition(array_interval_t* interval){
    int pivot_index, tmp_index, i;
    array_type pivot_value;
    
    tmp_index = interval->a;
    pivot_index = interval->a+ (interval->c - interval->a)/2; // You can change this line to choose your best method
    pivot_value = interval->array.array[pivot_index];
    
    swap(interval->array.array, pivot_index, interval->c);
    
    for (i = interval->a; i<interval->c; i++){
        if (interval->array.array[i] < pivot_value){
            swap(interval->array.array, i, tmp_index);
            ++tmp_index;
        }
    }
    
    swap(interval->array.array, tmp_index, interval->c);
    return tmp_index;
}




/*void in_place_sort( array_interval_t interval ) 
{
  // Your implementation here
	
}*/



// =======================================================================
// DOES BUBBLE SORT ON LIST OF INTERVAL interval
// ======================================================================
void bubbleSort( array_interval_t interval ) 
{
	//printf ("\n\n **BUBBLESORT a is %d c is %d", interval.a, interval.c);
	if (interval.a >= interval.c)
	{
		return;
	}
	else
	{
		if(oneTimeBubble(interval, true))
		{
			return;
		}
		else
		{
			interval.c--;
			bubbleSort(interval);
		}
	}
	
}


// =======================================================================
// USED IN bubbleSort() FUNTION TO BUBBLE THROUGH LIST ONCE
// =======================================================================
bool oneTimeBubble(array_interval_t interval, bool sorted)
{
	//printf ("\n ONE BUBBLE a is %d c is %d", interval.a, interval.c);
	if(interval.a >= interval.c)
	{
		return sorted;
	}
	else
	{
		//printf ("--> Value a %d value c %d", interval.array.array[interval.a], interval.array.array[interval.c]);
		if (interval.array.array[interval.a] > interval.array.array[interval.a+1])
		{
			//printf ("__SWAPPED!");
			sorted = false;
			swap(interval.array.array, interval.a, interval.a+1);
		}
		interval.a++;
		return oneTimeBubble(interval, sorted);
	}
}


// =======================================================================
// QUICK SORTS LIST USING MULTIPLE THREADS (NO RECURSION)
// =======================================================================
__task void quick_sort_task( void* void_ptr)
{
	int midpoint; 
	int initBeginning;

	//printf("\n TASK STARTED interval a %d c %d \n", TASK_PARAM->interval.a, TASK_PARAM->interval.c);
	// IF LIST IS STILL LARGER THAN INSERTION SORT SIZE
  	if ( (TASK_PARAM->interval.c - TASK_PARAM->interval.a + 1) >= USE_INSERTION_SORT)
  	{
  		initBeginning = TASK_PARAM->interval.a;
  		midpoint = partition( &(TASK_PARAM->interval) );

			if (midpoint < TASK_PARAM->interval.c)
			{
				TASK_PARAM->interval.a = midpoint + 1; // split list in two and create thread for upper list
				TASK_PARAM->priority++;
				os_tsk_create_ex( quick_sort_task, TASK_PARAM->priority, void_ptr );
			}
			
			if (midpoint > initBeginning)
  		{
				TASK_PARAM->interval.c = midpoint - 1; // recursively go through lower list
				TASK_PARAM->interval.a = initBeginning;
				quick_sort_task(void_ptr);
			}
  	}
  	else
  	{
  		// 	IF LIST IS SMALLER THAN USE_INSERTION_SORT THEN DO BUBBLE SORT AND EXIT THREAD
  		bubbleSort(TASK_PARAM->interval);
  		os_tsk_delete_self();
  	}
		
		os_tsk_delete_self();
}


// =======================================================================
// BASICALLY AN INIT FOR THE quick_sort_task() FUNCTION
// =======================================================================
void quicksort( array_t array ) {
	array_interval_t interval;
	qsort_task_parameters_t task_param;
	
	// Based on MTE 241 course notes--you can change this if you want
	//  - in the course notes, this sorts from a to c - 1
	interval.array =  array;
	interval.a     =  0;
	interval.c     =  array.length-1;
	
	task_param.interval = interval;

	// If you are using priorities, you can change this
	task_param.priority = 10;
	
	//start the quick_sort threading
	os_tsk_create_ex( quick_sort_task, task_param.priority, &task_param ); 
	//bubbleSort( interval ); 
}
