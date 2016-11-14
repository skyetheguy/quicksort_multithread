#include <LPC17xx.h>
#include <RTL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "quicksort.h"
#include "array_tools.h"

// You decide what the threshold will be
#define USE_INSERTION_SORT 5

// Local prototypes



//structs
typedef struct {
	array_t array;
	size_t a;
	size_t c;
} array_interval_t;

typedef struct{
	array_interval_t interval;
	unsigned char priority;
} qsort_task_parameters_t;


// swap function used in the partition function
__inline void swap(array_type* arr, int a, int b){
    array_type tmp = arr[a];
    arr[a] = arr[b];
    arr[b] = tmp;
}
// This function implements the partitioning and swapping as described in the quicksort algorithm
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




void in_place_sort( array_interval_t interval ) 
{
  // Your implementation here
}




__task void quick_sort_task( void* void_ptr)
{

	int midpoint; 
	int initBeginning;

	// IF LIST IS STILL LARGER THAN INSERTION SORT SIZE
  	if ( (void_ptr->interval.c - void_ptr->interval.a + 1) >= USE_INSERTION_SORT)
  	{
  		initBeginning = void_ptr->interval.a;
  		midpoint = partition( &(void_ptr->interval) );

  		void_ptr->interval.a = midpoint + 1; // split list in two and create thread for upper list
  		void_ptr->priority++;
  		os_tsk_create_ex( quick_sort_task, void_ptr->priority, void_ptr ); 

  		void_ptr->interval.c = midpoint - 1; // recursively go through lower list
  		void_ptr->interval.a = initBeginning;
  		void quick_sort_task(void_ptr);
  	}
  	else
  	{
  		// 	IF LIST IS SMALLER THAN USE_INSERTION_SORT THEN DO INSERTION SORT AND EXIT THREAD
  		in_place_sort(void_ptr->interval);
  		return;
  	}
}



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
	
}
