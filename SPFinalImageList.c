#include <stdlib.h>
#include <stdio.h>
#include "SPKDArray.h"
#include "SPPoint.h"
#include "SPKDTree.h"
#include "SPBPriorityQueue.h"
#include "SPListElement.h"
#include "SPLogger.h"



int *spGetFinalImageList(SPKDNode kdtree, SPPoint *queryImageFeatures, int numOfImages, int spknn,int numOfSimilarImages){
    //create an int[] 'imageList' in length numOfImages
    //iterate over the queryImageFeatures:
    //  -for each feature, run KNN Search
    //  -then use its BPQ to extract all the closest image indices
    //      -for an index j: do imageList[j]+=1;
    //after loop is done, our list will represent the number of times each index was encountered in the KNN Search
    //find the maximum of the list
    //create a new BPqueue in size 'numOfSimilarImages'
    //for each index in imageList:
    //  -enqueue(index,(max - imageList[index]))
    //the new BPQueue will now hold our 'numOfSimilarImages' best images
    // create an int[numOfSimilarImages]
    //extractMin the BPqueue and insert the indexes to the new array.
    //return the array.

}
