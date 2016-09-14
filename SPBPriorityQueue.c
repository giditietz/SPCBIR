/*#include "SPList.h"
#include "SPListElement.h"
#include "SPBPriorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node_t {
	SPListElement data;
	struct node_t* next;
	struct node_t* previous;
}*Node;

typedef struct sp_list_t {
	Node head;
	Node tail;
	Node current;
	int size;
}*List;

struct sp_bp_queue_t {
	List body;
	int maxsize;
};

SPBPQueue spBPQueueCreate(int maxSize) {
	//declare:
	SPBPQueue newQueue;
	List body;
	//verify:
	if (maxSize < 0) {
		return NULL;
	}
	//allocate:
	newQueue = (SPBPQueue) malloc(sizeof(*newQueue));
	if (newQueue == NULL) {
		return NULL;
	}
	body = spListCreate();
	if (body == NULL) {
		return NULL;
	}
	//assign:
	newQueue->body = body;
	newQueue->maxsize = maxSize;
	return newQueue;
}

SPBPQueue spBPQueueCopy(SPBPQueue source) {

	SPBPQueue copiedQueue;
	SPList sourceList;
	SPList copiedList;


	if (source == NULL) {
		return NULL;
	}

	if (source->maxsize < 0) {
		return NULL;
	}
	//Copy the old queue to create the new queue:
	copiedQueue = spBPQueueCreate(spBPQueueGetMaxSize(source));
	if (copiedQueue == NULL) { //Allocation failure
		return NULL;
	}
	//Copy the source list:
	sourceList = source->body;
	copiedList = spListCopy(sourceList);
	if (copiedList == NULL) { //Allocation failure
		spBPQueueDestroy(copiedQueue);
		return NULL;
	}
	//Important! = must free old empty list:
	spListDestroy(copiedQueue->body);
	copiedQueue->body = copiedList;
	copiedQueue->maxsize = source->maxsize;

	return copiedQueue;
}

void spBPQueueDestroy(SPBPQueue source) {
	if (source == NULL) {
		return;
	}
	//destroy the body of the Queue:
	spListDestroy(source->body);

	free(source);

}
void spBPQueueClear(SPBPQueue source) {
	if (source == NULL) {
		return;
	}
	//simply use clear on the list
	spListClear(source->body);

}

int spBPQueueSize(SPBPQueue source) {
	if (source == NULL) {
		return -1;
	}
	int res;
	res = spListGetSize(source->body);
	return res;

}

int spBPQueueGetMaxSize(SPBPQueue source) {
	if (source == NULL) {
		return -1;
	}

	return source->maxsize;

}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element) {
	//initialize
	SPListElement currentElement;
	SPList list;
	SP_LIST_MSG msg;
	bool inserted = false;
	double elementValue, lastElementValue;
	//validate input:
	if (source == NULL || element == NULL || source->body == NULL) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}

	//get the newElement value for future comparisons:
	elementValue = spListElementGetValue(element);
	if (elementValue == -1.0) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	list = source->body;

	//check if queue is empty and insert if it is.
	if (spBPQueueIsEmpty(source)) {
		msg = spListInsertFirst(list, element);
		inserted = true;
	}

	//if full:
	//get the lastElementValue value for future comparison:
	lastElementValue = spListElementGetValue(spListGetLast(list));

	if (lastElementValue == -1.0) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	//check if lastElement's value is bigger:
	if (spBPQueueIsFull(source)) {
		//if not - do not insert
		if (lastElementValue <= elementValue) {
			return SP_BPQUEUE_SUCCESS;
		}
		//if yes - remove the last element and insert the new element instead
		if (lastElementValue > elementValue) {
			msg = spListRemoveCurrent(list);
			if (msg != SP_LIST_SUCCESS) {
				return SP_BPQUEUE_INVALID_ARGUMENT;
			}
		}
	}
	//if not full - iterate over the list elements and find the insertion place.
	currentElement = spListGetFirst(list);
	if (currentElement == NULL) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	while (currentElement != NULL && !inserted) {
		if (elementValue < spListElementGetValue(currentElement)) {
			//Insert element:
			msg = spListInsertBeforeCurrent(list, element);
			inserted = true;
		}
		currentElement = spListGetNext(list);
	}
	if (!inserted) {
		msg = spListInsertLast(list, element);
	}
	if (msg == SP_LIST_NULL_ARGUMENT || msg == SP_LIST_INVALID_CURRENT) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if (msg == SP_LIST_OUT_OF_MEMORY) {
		return SP_BPQUEUE_OUT_OF_MEMORY;
	}
	if (msg == SP_LIST_SUCCESS) {
		return SP_BPQUEUE_SUCCESS;
	}
	return SP_BPQUEUE_INVALID_ARGUMENT;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source) {
	if (source == NULL) {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	SPListElement firstElement;
	SP_LIST_MSG msg;
	//check if the 'body' is empty:
	if (spListGetSize(source->body) == 0) {
		return SP_BPQUEUE_EMPTY;
	}
	firstElement = spListGetFirst(source->body); //now 'current' is pointing to the first element.
	if (firstElement == NULL) {
		return SP_BPQUEUE_OUT_OF_MEMORY;
	}

	msg = spListRemoveCurrent(source->body);
	if (msg == SP_LIST_SUCCESS) {
		return SP_BPQUEUE_SUCCESS;
	} else {
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}

}

SPListElement spBPQueuePeek(SPBPQueue source) {
	if (source == NULL) {
		return NULL;
	}
	SPListElement returnedElement, firstElement;
	firstElement = spListGetFirst(source->body);
	if (firstElement == NULL) {
		return NULL;
	}
	returnedElement = spListElementCopy(firstElement);
	if (returnedElement == NULL) {
		return NULL;
	}

	return returnedElement;
}

SPListElement spBPQueuePeekLast(SPBPQueue source) {
	if (source == NULL) {
		return NULL;
	}
	SPListElement returnedElement, lastElement;
	lastElement = spListGetLast(source->body);

	returnedElement = spListElementCopy(lastElement);
	if (returnedElement == NULL) {
		return NULL;
	}
	if (lastElement == NULL) {
		return NULL;
	}
	return returnedElement;
}

double spBPQueueMinValue(SPBPQueue source) {
	if (source == NULL) {
		return -1;
	}
	SPListElement FirstElement = spBPQueuePeek(source);
	if (FirstElement == NULL) {
		return -1;
	}
	double result = spListElementGetValue(FirstElement);
	spListElementDestroy(FirstElement);
	return result;
}

double spBPQueueMaxValue(SPBPQueue source) {
	if (source == NULL) {
		return -1;
	}
	SPListElement lastElement = spBPQueuePeekLast(source);
	if (lastElement == NULL) {
		return -1;
	}
	double result = spListElementGetValue(lastElement);
	spListElementDestroy(lastElement);
	return result;

}

bool spBPQueueIsEmpty(SPBPQueue source) {
	if (source == NULL) {
		return false;
	}
	if (spListGetSize(source->body) == 0) {
		return true;
	} else {
		return false;
	}
}
bool spBPQueueIsFull(SPBPQueue source) {
	if (source == NULL) {
		return false;
	}
	if (spListGetSize(source->body) == source->maxsize) {
		return true;
	} else {
		return false;
	}

}*/


#include "SPBPriorityQueue.h"
#include "SPList.h"
#include "SPListElement.h"
#include <stdio.h>
#include <stdlib.h>

#define INVALID -1

/**
 * SP Bounded Priority Queue Summary
 *
 * Implements a Bounded Priority Queue using given Linked List (SPList).
 * please refer to SPList.h for usage.
 * The elements of the list are of type SPListElement, please refer
 * to SPListElement.h for usage.
 *
 * In this implementation the first element has the minimum element value.
 * The last element has the maximum element value.
 *
 * The following functions are available:
 *
 *   Create			- Creates an empty queue with a given maximum capacity
 *   Copy			- Creates a copy of a given queue
 *   Destroy		- Frees all memory allocation associated with the queue
 *   Clear			- Removes all the elements in the queue
 *   GetSize		- Returns the number of elements in the queue
 *   GetMaxSize		- Returns the maximum capacity of the queue
 *   Enqueue		- Inserts a NEW COPY (must be allocated) element to the queue
 *   Dequeue		- Removes the element with the lowest value
 *   Peek			- Returns a NEW COPY of the element with the lowest value
 *   PeekLast		- Returns a NEW COPY of the element with the highest value
 *   MinValue		- Returns the minimum value in the queue
 *   MaxValue		- Returns the maximum value in the queue
 *   IsEmpty 		- Returns true if the queue is empty
 *   IsFull			- Returns true if the queue is full
 */

/**
 * Struct representing a Bounded Priority Queue.
 * Contains the following variables:
 * SPList list		- List of elements sorted by priority.
 * int maxSize 		- Maximum number of elements allowed.
 */
struct sp_bp_queue_t{
	SPList list;
	int maxSize;
};

/**
 * Allocates a new Queue.
 *
 * This function creates a new empty queue.
 * @return
 * 	NULL - If allocations failed or maxSize is negative.
 * 	A new Queue in case of success.
 */
SPBPQueue spBPQueueCreate(int maxSize){

	SPBPQueue queue;
	SPList list;

	//invalid argument - negative maxSize not allowed
	if (maxSize < 0){
		return NULL;
	}

	queue = (SPBPQueue) malloc ( sizeof(*queue) );
	if(queue == NULL){//Allocation failure
		return NULL;
	}
	list = spListCreate();
	if(list == NULL){ //Allocation failure
		free(queue);
		return NULL;
	}

	queue -> list = list;
	queue -> maxSize = maxSize;

	return queue;
}

/**
 * Creates a copy of a bounded priority queue.
 *
 * @param source	- a Queue to be copied
 * @return NULL 	- if input is invalid.
 * @return a independent copy of source bounded priority queue (deep clone).
 */
SPBPQueue spBPQueueCopy(SPBPQueue source){

	SPBPQueue newQueue;
	SPList currentList, copiedList;

	if(source == NULL || source -> maxSize < 0){
		return NULL;
	}

	//Create new queue
	newQueue = spBPQueueCreate( spBPQueueSize(source) );
	if(newQueue == NULL){ //Allocation failure
		return NULL;
	}

	//Copy list:
	currentList = source->list;
	copiedList = spListCopy(currentList);
	if(copiedList == NULL){ //Allocation failure
		spBPQueueDestroy(newQueue);
		return NULL;
	}

	//Free old empty list:
	spListDestroy(newQueue->list);

	newQueue->list = copiedList;
	newQueue -> maxSize = source -> maxSize;

	return newQueue;
}

/**
 * Destroys a bounded priority queue and free all allocated space
 * If parameter is NULL nothing happens.
 *
 * @param source - a bounded priority queue to be destroyed.
 */
void spBPQueueDestroy(SPBPQueue source){
	if(source == NULL){
		return;
	}

	spListDestroy(source->list);
	free(source);
}

/**
 * Removes all the elements in the queue.
 * If parameter is NULL nothing happens.
 *
 * @param source - a bounded priority queue to be cleared.
 */
void spBPQueueClear(SPBPQueue source){
	if(source == NULL){
		return;
	}

	spListClear(source->list);
}

/**
 * Returns the number of elements in the queue.
 *
 * @param source - a bounded priority queue.
 * @return number of elements in queue, returns -1 if NULL.
 */
int spBPQueueSize(SPBPQueue source){
	if(source == NULL){
		return INVALID;
	}
	return spListGetSize(source->list);
}

/**
 * Returns the maximum capacity of the queue.
 *
 * @param source - a bounded priority queue.
 * @return maximum capacity of the queue, returns -1 if NULL.
 */
int spBPQueueGetMaxSize(SPBPQueue source){
	if(source == NULL){
		return INVALID;
	}
	return source->maxSize;
}

/**
 * Inserts a new copy of element to the queue.
 *
 * @param source 				- a bounded priority queue.
 * @param element 				- new element to be added.
 *
 * @return SP_BPQUEUE_MSG -
 * SP_BPQUEUE_OUT_OF_MEMORY 	- In case of memory allocation failure.
 * SP_BPQUEUE_INVALID_ARGUMENT 	- source or element are invalid.
 * SP_BPQUEUE_SUCCESS 			- element was successfully added.
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){

	SPListElement lastElement, checkedElement;
	SPList list;
	SP_LIST_MSG status;
	bool found = false;
	double elementValue, lastValue ;

	//Input check
	if(source == NULL || element == NULL || source->list == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}

	//Get the new element value:
	elementValue = spListElementGetValue(element);
	if(elementValue == INVALID){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}

	list = source->list;

	//If list is full:
	if(spListGetSize(list) == source->maxSize){

		/*
		 * Pointer to list's last element.
		 * Notice that calling spListGetLast will set "current" pointer
		 * to the last element in list
		 */
		lastElement = spListGetLast(list);
		if(lastElement == NULL){
			return SP_BPQUEUE_INVALID_ARGUMENT;
		}

		lastValue = spListElementGetValue(lastElement);
		if(lastValue == INVALID){
			return SP_BPQUEUE_INVALID_ARGUMENT;
		}

		//If element's value > queue's last element's value do nothing.
		if(elementValue > lastValue){
			return SP_BPQUEUE_SUCCESS;
		}

		//otherwise - make room for the new element by deleting the last one.
		status = spListRemoveCurrent(list);
		if(status == SP_LIST_NULL_ARGUMENT || status == SP_LIST_INVALID_CURRENT){
			return SP_BPQUEUE_INVALID_ARGUMENT;
		}
	}

	//In case queue is empty:
	if(spBPQueueIsEmpty(source)){
		status = spListInsertFirst(list, element);
		found = true;
	}

	checkedElement = spListGetFirst(list);
	if(!found && checkedElement == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}

	//Find location and insert:
	while(checkedElement != NULL && !found)	{
		if(elementValue < spListElementGetValue(checkedElement)){
			//Insert element:
			status = spListInsertBeforeCurrent(list, element);
			found = true;
		}

		checkedElement = spListGetNext(list);
	}

	//If location was not found - insert last.
	if(!found){
		status = spListInsertLast(list,element);
	}

	switch(status){
		case SP_LIST_OUT_OF_MEMORY:
			return SP_BPQUEUE_OUT_OF_MEMORY;
			break;

		case SP_LIST_SUCCESS:
			return SP_BPQUEUE_SUCCESS;
			break;

		case SP_LIST_NULL_ARGUMENT:
			return SP_BPQUEUE_INVALID_ARGUMENT;

		default:
			break;
	}
	return SP_BPQUEUE_INVALID_ARGUMENT;
}

/**
 * Removes the element with the lowest value.
 *
 * @param source 				- a bounded priority queue.
 *
 * @return SP_BPQUEUE_MSG -
 * SP_BPQUEUE_INVALID_ARGUMENT 	- source or element are invalid.
 * SP_BPQUEUE_SUCCESS 			- element was successfully removed.
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){

	SPListElement firstElement;
	SP_LIST_MSG status;

	if(source == NULL){
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}

	//Move current pointer to first element:
	firstElement = spListGetFirst(source -> list);

	//List is empty
	if(firstElement == NULL || spListGetSize(source->list) <= 0){
		return SP_BPQUEUE_EMPTY;
	}

	status = spListRemoveCurrent(source -> list);
	if(status == SP_LIST_SUCCESS){
		return SP_BPQUEUE_SUCCESS;
	}

	return SP_BPQUEUE_INVALID_ARGUMENT;

}

/**
 * Returns a new copy of the element with the lowest value.
 *
 * @param source 				- a bounded priority queue.
 *
 * @return
 * NULL if source is null, queue is empty or allocation failed.
 * Otherwise the address of the new copy.
 */
SPListElement spBPQueuePeek(SPBPQueue source){

	SPListElement firstElement;
	SPListElement newElement;

	if(source == NULL){
		return NULL;
	}

	//Get the first element:
	firstElement = spListGetFirst(source -> list);

	//List is empty
	if(firstElement == NULL){
		return NULL;
	}

	newElement = spListElementCopy(firstElement);
	//If NULL returns NULL anyway

	return newElement;
}

/**
 * Returns a new copy of the element with the highest value.
 *
 * @param source 				- a bounded priority queue.
 *
 * @return
 * NULL if source is null, queue is empty or allocation failed.
 * Otherwise the address of the new copy.
 */
SPListElement spBPQueuePeekLast(SPBPQueue source){

	SPListElement lastElement;
	SPListElement newElement;

	if(source == NULL){
		return NULL;
	}

	//Get the last element:
	lastElement = spListGetLast(source -> list);

	//List is empty
	if(lastElement == NULL){
		return NULL;
	}

	newElement = spListElementCopy(lastElement);
	//If NULL returns NULL anyway

	return newElement;
}


/**
 * Returns the minimum value in the queue.
 *
 * @param source 				- a bounded priority queue.
 *
 * @return
 * -1 if source is null, queue is empty or allocation failed.
 * Otherwise the minimum value in the queue.
 */
double spBPQueueMinValue(SPBPQueue source){

	SPListElement firstElement;
	double value;

	if(source == NULL){
		return INVALID;
	}

	firstElement = spBPQueuePeek(source);

	if(firstElement == NULL){
		return INVALID;
	}

	value = spListElementGetValue(firstElement);

	spListElementDestroy(firstElement);

	return value;
}

/**
 * Returns the maximum value in the queue.
 *
 * @param source 				- a bounded priority queue.
 *
 * @return
 * -1 if source is null, queue is empty or allocation failed.
 * Otherwise the maximum value in the queue.
 */
double spBPQueueMaxValue(SPBPQueue source){

	SPListElement lastElement;
	double value;

	if(source == NULL){
		return INVALID;
	}

	lastElement = spBPQueuePeekLast(source);

	if(lastElement == NULL){
		return INVALID;
	}

	value = spListElementGetValue(lastElement);

	spListElementDestroy(lastElement);

	return value;
}

/**
 * Returns true if queue is empty.
 *
 * @param source 				- a bounded priority queue.
 *
 * @return
 * false if source is NULL or queue contains elements.
 * true if queue is empty.
 */
bool spBPQueueIsEmpty(SPBPQueue source){
	if(source == NULL){
		return false;
	}

	return spListGetSize(source->list) == 0;
}

/**
 * Returns true if queue is full.
 *
 * @param source 				- a bounded priority queue.
 *
 * @return
 * false if source is NULL or queue is not full.
 * true if queue is full.
 */
bool spBPQueueIsFull(SPBPQueue source){
	if(source == NULL){
		return false;
	}

	return spListGetSize(source->list) == spBPQueueGetMaxSize(source);
}

