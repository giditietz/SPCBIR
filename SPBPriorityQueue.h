#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
/**
 * SP Bounded Priority Queue Summary
 *
 * Implementation of a Bounded Priority Queue.
 * based on Linked List (SPList) implementation.
 * The following functions are available:
 *
 *   Create			- Creates an empty queue with a given maximum capacity
 *   Copy			- Creates a copy of a given queue
 *   Destroy		- Deletes an existing queue and frees all resources
 *   Clear			- Removes all the elements in the queue
 *   GetSize		- Returns the current size of the queue
 *   GetMaxSize		- Returns the maximum size of the queue
 *   Enqueue		- Inserts a NEW COPY (must be allocated) of an element to the queue
 *   Dequeue		- Removes the element with the lowest value
 *   Peek			- Returns a NEW COPY of the element with the lowest value
 *   PeekLast		- Returns a NEW COPY of the element with the highest value
 *   MinValue		- Returns the minimum value in the queue
 *   MaxValue		- Returns the maximum value in the queue
 *   IsEmpty 		- Returns true if the queue is empty
 *   IsFull			- Returns true if the queue is full
 */

/**
 * Struct representing a Bounded Priority Queue:
 * Contains the following variables:
 * SPList body		- List of elements sorted by value.
 * int maxSize 		- Maximum number of elements allowed in the queue.
 */
typedef struct sp_bp_queue_t* SPBPQueue;

/** Type used for returning error codes from queue functions */
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a new Queue based on a given maximal size.
 *
 * This function creates a new empty queue.
 * @param maxSize - given maximal size of the queue, cannot be negative.
 * @return
 * 	NULL - If allocations failed
 * 	NULL - if maxSize is negative.
 * 	SPBPQueue - A new Queue in case of success.
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a copy of a bounded priority queue.
 *
 * @param source	- a Queue to be copied
 * @return NULL 	- if input is invalid.
 * @SPBPQueue 		- a copy of source bounded priority queue.
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * Frees a bounded priority queue and free all allocated space
 *
 * @param source - a bounded priority queue to be destroyed.
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Removes all the elements in the queue, but don't destroy it.
 *
 * @param source - a bounded priority queue to be cleared.
 */
void spBPQueueClear(SPBPQueue source);

/**
 * Returns the current number of elements in the queue.
 *
 * @param source - a bounded priority queue.
 * @return number of elements in queue
 * @returns -1 if NULL.
 */
int spBPQueueSize(SPBPQueue source);

/**
 * Returns the maximual number of elements that the queue can hold.
 *
 * @param source - a bounded priority queue.
 * @return  - maximum capacity of the queue.
 * @returns -1 if NULL.
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Inserts a new copy of element to the queue.
 * Cases:
 * 		1.)invalid input - return a message,
 * 		2.)Empty queue - simply enqueue as the first element
 * 		3.)Full queue: if the last element is smaller then the inserted element - do nothing
 * 					   if the last element is bigger then the inserted element - remove the last element, insertion will be handeled later.
 * 		4.)Queue is neither empty not full:
 * 					   Do a recursive sequential search to find the right place for the new element, and insert it.
 *
 * @param source 				- a bounded priority queue.
 * @param element 				- new element to be added (Must be allocated first).
 *
 * @return SP_BPQUEUE_MSG -
 * SP_BPQUEUE_OUT_OF_MEMORY 	- In case of memory allocation failure.
 * SP_BPQUEUE_INVALID_ARGUMENT 	- source or element are invalid.
 * SP_BPQUEUE_SUCCESS 			- element was successfully added.
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the element with the lowest value.
 *
 * @param source 				- a bounded priority queue.
 *
 * @return SP_BPQUEUE_MSG -
 * SP_BPQUEUE_INVALID_ARGUMENT 	- source or element are invalid.
 * SP_BPQUEUE_SUCCESS 			- element was successfully removed.
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * Returns a new copy of the element with the lowest value.
 * DOES NOT remove the first element.
 *
 * @param source - a bounded priority queue.
 *
 * @return
 * NULL if source is null
 * NULL if queue is empty
 * NULL if allocation failed.
 * SPListElement - the new copy.
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Returns a new copy of the element with the highest value.
 * DOES NOT remove the last element.
 *
 * @param source - a bounded priority queue.
 *
* @return
 * NULL if source is null
 * NULL if queue is empty
 * NULL if allocation failed.
 * SPListElement - the new copy.
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * Returns the minimum value in the queue.
 *
 * @param source - a bounded priority queue.
 *
* @return
 * NULL if source is null
 * NULL if queue is empty
 * NULL if allocation failed.
 * If succeeded - the maximal value in the queue.
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * Returns the maximum value in the queue.
 *
 * @param source 				- a bounded priority queue.
 *
 * @return
 * -1 if source is null, queue is empty or allocation failed.* @return
 * NULL if source is null
 * NULL if queue is empty
 * NULL if allocation failed.
 * If succeeded -  the minimal value in the queue.
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * Returns true if queue is empty.
 *
 * @param source 	- a bounded priority queue.
 *
 * @return
 * false if source is NULL
 * false if queue contains elements.
 * true if queue is empty.
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * Returns true if queue is full.
 *
 * @param source 	- a bounded priority queue.
 *
 * @return
 * false if source is NULL
 * false if queue is not full.
 * true if queue is full.
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
