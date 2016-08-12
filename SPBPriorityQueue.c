#include "SPList.h"
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

}
