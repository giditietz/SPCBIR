#include <stdlib.h>
#include <stdio.h>
#include "SPKDArray.h"
#include "SPPoint.h"
#include "SPKDTree.h"
#include "SPBPriorityQueue.h"
#include "SPListElement.h"
#include "SPLogger.h"
#include "SPConfig.h"
#include "Malloc_Macro.h"
#define ZERO 0
#define KDTREE_NULL_STR "Given KDTree is NULL!"
#define KNN_SEARCH_FAIL "KNN Search had failed"
#define KNN_SEARCH_SUCCESS "KNN search finished successfully."
#define ELEMENT_STR "Element was not peeked from BPQueue"
#define BP_ERROR_DQ_STR "BPQueue error in Dequeue"
#define BP_ERROR_EQ_STR "BPQueue error in Enqueue"
#define RESOURCE_FREE_STR "All resources in the scope were peacefully freed."
#define RESOURCE_SCOPE_STR "All resources in the scope were forcefully freed."
#define BEST_IMG_STR "Best images were found!"

int spGetFinalImageList(SPConfig cfg, SPKDNode kdtree, int *finalIndexArray, SPPoint *queryImageFeatures, int queryFeaturesNum) {
    //declare the arrays/structs that will be malloced later
    int res = SUCCESS;
    int *indexArray = NULL;
    SPPoint currPoint = NULL;
    SPListElement curElement = NULL;
    SPListElement newElement = NULL;
    SPBPQueue currBpq = NULL;
    SPBPQueue finalBpq = NULL;
    SP_CONFIG_MSG msg;
    if (NULL == kdtree) {
        spLoggerPrintError(KDTREE_NULL_STR, __FILE__, __FUNCTION__, __LINE__);
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

    currBpq = spBPQueueCreate(spknn);
    finalBpq = spBPQueueCreate(numOfSimilarImages);

    int maxCountOfIndex = 0;
    int curElementIndex;
    double newVal;
    SP_BPQUEUE_MSG bpqMsg;
    //create an int[] 'indexArray' in length numOfImages
    indexArray = (int *) calloc((size_t) numOfImages, sizeof(int));
    //iterate over the queryImageFeatures:
    for (int i = 0; i < queryFeaturesNum; i++) {
        currPoint = queryImageFeatures[i];
        //  -for each feature, run KNN Search
        if (!spKDTreeKNNSearch(kdtree, currBpq, currPoint)) {
            spLoggerPrintError(KNN_SEARCH_FAIL, __FILE__, __FUNCTION__, __LINE__);
            goto fail;
        }
        spLoggerPrintDebug(KNN_SEARCH_SUCCESS,__FILE__,__FUNCTION__,__LINE__);

        // then use its BPQ to extract all the closest image indices
        for (int j = 0; j < spknn; j++) {
            curElement = spBPQueuePeek(currBpq);
            if (NULL == curElement) {
                spLoggerPrintError(ELEMENT_STR, __FILE__, __FUNCTION__, __LINE__);
                goto fail;
            }
            curElementIndex = spListElementGetIndex(curElement);
            // -for an index j: do indexArray[j]+=1;
            indexArray[curElementIndex] += 1;
            spListElementDestroy(curElement);
            //Dequeue the first element after using it
            bpqMsg = spBPQueueDequeue(currBpq);
            if (!(bpqMsg == SP_BPQUEUE_FULL || bpqMsg == SP_BPQUEUE_SUCCESS)) {
                spLoggerPrintError(BP_ERROR_DQ_STR, __FILE__, __FUNCTION__, __LINE__);
                goto fail;
            }
        }
        //TODO Delete prints
    }

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
        if (!(bpqMsg == SP_BPQUEUE_FULL || bpqMsg == SP_BPQUEUE_SUCCESS)) {
            spLoggerPrintError(BP_ERROR_EQ_STR, __FILE__, __FUNCTION__, __LINE__);
            goto fail;
        }
    }
    for (int j = 0; j < numOfSimilarImages; j++) {
        //extractMin the BPqueue and insert the indexes to the new array.
        curElement = spBPQueuePeek(finalBpq);
        curElementIndex = spListElementGetIndex(curElement);
        if (NULL != curElement) {
            spListElementDestroy(curElement);
        }
        finalIndexArray[j] = curElementIndex;
        //Dequeue the first element after using it
        bpqMsg = spBPQueueDequeue(finalBpq);
        if (!(bpqMsg == SP_BPQUEUE_FULL || bpqMsg == SP_BPQUEUE_SUCCESS)) {
            spLoggerPrintError(BP_ERROR_DQ_STR, __FILE__, __FUNCTION__, __LINE__);
            goto fail;
        }
    }

    //free the used resources
    free(indexArray);
    spBPQueueDestroy(currBpq);
    spBPQueueDestroy(finalBpq);
    spListElementDestroy(newElement);
    spLoggerPrintDebug(RESOURCE_FREE_STR, __FILE__, __FUNCTION__, __LINE__);
    spLoggerPrintInfo(BEST_IMG_STR);
    return res;

    fail:
    FREE_MACRO(indexArray);
    if (NULL != currBpq) {
        spBPQueueDestroy(currBpq);
    }
    if (NULL != finalBpq) {
        spBPQueueDestroy(finalBpq);
    }
    if (NULL != newElement) {
        spListElementDestroy(newElement);
    }
    if (NULL != kdtree) {
        spKDTreeDestroy(kdtree);
    }
    res = INVALID_ARGUMENT;
    spLoggerPrintDebug(RESOURCE_SCOPE_STR, __FILE__, __FUNCTION__, __LINE__);

    return res;
}
