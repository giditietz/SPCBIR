#include <stdlib.h>
#include <stdio.h>
#include "SPKDArray.h"
#include "SPPoint.h"
#include "SPKDTree.h"
#include "SPBPriorityQueue.h"
#include "SPListElement.h"
#include "SPLogger.h"
#include "SPConfig.h"


int spGetFinalImageList(SPConfig cfg, SPKDNode kdtree, int *finalIndexArray, SPPoint *queryImageFeatures) {
    SP_CONFIG_MSG msg;
    int res = SUCCESS;
    int spNumOfFeatures = spConfigGetNumOfFeatures(cfg, &msg);
    if (msg != SP_CONFIG_SUCCESS) {
        goto fail;
    }
    int numOfImages = spConfigGetNumOfImages(cfg, &msg);
    if (msg != SP_CONFIG_SUCCESS) {
        goto fail;
    }
    int spknn = spConfigGetSPKnn(cfg, &msg);
    if (msg != SP_CONFIG_SUCCESS) {
        goto fail;
    }
    int numOfSimilarImages = spConfigGetspNumOfSimilarImages(cfg, &msg);
    if (msg != SP_CONFIG_SUCCESS) {
        goto fail;
    }

    SPBPQueue currBpq = spBPQueueCreate(spknn);
    SPBPQueue finalBpq = spBPQueueCreate(numOfSimilarImages);
    SPPoint currPoint;
    SPListElement curElement;
    SPListElement newElement;
    int maxCountOfIndex = 0;
    int curElementIndex;
    double newVal;
    SP_BPQUEUE_MSG bpqMsg;
    //create an int[] 'indexArray' in length numOfImages
    int *indexArray = (int *) calloc((size_t) numOfImages, sizeof(int));
    //create an int[] 'finalIndexArray' in length numOfImages
    //finalIndexArray = (int *) calloc((size_t) numOfSimilarImages, sizeof(int)); TODO Remove
    //TODO Allocation error + logger
    //iterate over the queryImageFeatures:
    for (int i = 0; i < spNumOfFeatures; i++) {
        currPoint = queryImageFeatures[i];
        //  -for each feature, run KNN Search
        if (!spKDTreeKNNSearch(kdtree, currBpq, currPoint)) {
            //TODO Raise error + logger
        }
        // then use its BPQ to extract all the closest image indices
        for (int j = 0; j < spknn; j++) {
            curElement = spBPQueuePeek(currBpq);
            curElementIndex = spListElementGetIndex(curElement);
            // -for an index j: do indexArray[j]+=1;
            indexArray[curElementIndex] += 1;
            spListElementDestroy(curElement);
            //Dequeue the first element after using it
            bpqMsg = spBPQueueDequeue(currBpq);
            if (!(bpqMsg == SP_BPQUEUE_FULL || bpqMsg == SP_BPQUEUE_SUCCESS)) {
                //TODO Raise error + logger
            }
        }
        for (int k = 0; k < numOfImages; k++) {
            printf("%d ", indexArray[k]);
        }
        puts("\n");
    }
    for (int k = 0; k < numOfImages; k++) {
        printf("%d ", indexArray[k]);
    }
    puts("\n");
    //after loop is done, our list will represent the number of times each index was encountered in the KNN Search
    //find the maximum of the array for future calculations
    for (int i = 0; i < numOfImages; i++) {
        if (indexArray[i] > maxCountOfIndex) {
            maxCountOfIndex = indexArray[i];
        }
    }
    //for each index in imageList: enqueue(index,(max - imageList[index]))
    //the  new BPQueue 'finalBpq' will now hold our 'numOfSimilarImages' best images.
    //we use the BPQueue to sort our indices efficiently.
    for (int imageIndex = 0; imageIndex < numOfImages; imageIndex++) {
        newVal = maxCountOfIndex - indexArray[imageIndex];
        newElement = spListElementCreate(imageIndex, newVal);
        bpqMsg = spBPQueueEnqueue(finalBpq, newElement);
        if (bpqMsg == 1) {
            //TODO Raise error + logger
        }
    }
    for (int j = 0; j < numOfSimilarImages; j++) {
        //extractMin the BPqueue and insert the indexes to the new array.
        curElement = spBPQueuePeek(finalBpq);
        curElementIndex = spListElementGetIndex(curElement);
        spListElementDestroy(curElement);
        finalIndexArray[j] = curElementIndex;
        //Dequeue the first element after using it
        bpqMsg = spBPQueueDequeue(finalBpq);
        if (bpqMsg == 1) {
            //TODO Raise error + logger
        }
    }

    //free the used resources
    free(indexArray);
    spBPQueueDestroy(currBpq);
    spBPQueueDestroy(finalBpq);
    spListElementDestroy(newElement);
    spKDTreeDestroy(kdtree);
    return res;

    fail:
    return NULL; //TODO Change to something meaningful
}
