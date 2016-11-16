#ifndef __QUICKSORT_H__
#define __QUICKSORT_H__

/*
 * Author names:
 *   1.  uWaterloo User ID:  xxxxxxxx@uwaterloo.ca
 *   2.  uWaterloo User ID:  xxxxxxxx@uwaterloo.ca
 */

#include "array_tools.h"

void quicksort( array_t );
void quicksort_sem( array_t );




// STRUCTS USED FOR LIST AND PARAM OF THREADS
typedef struct {
	array_t array;
	size_t a;
	size_t c;
} array_interval_t;

typedef struct{
	array_interval_t interval;
	unsigned char priority;
} qsort_task_parameters_t;





bool oneTimeBubble(array_interval_t interval, bool sorted);
void bubbleSort( array_interval_t interval ) ;
__task void quick_sort_task( void* void_ptr);


// Uncomment this when you implement Project 4
// void quicksort_sem( array_t );

#endif
