#include "SPKDArray.h"
#include "SPBPriorityQueue.h"
#include "SPKDTree.h"

#ifndef SPCBIR_SPFINALIMAGELIST_H
#define SPCBIR_SPFINALIMAGELIST_H

int *spGetFinalImageList(SPKDNode kdtree, SPPoint *queryImageFeatures, int numOfImages, int spknn, int numOfSimilarImages);
#endif //SPCBIR_SPFINALIMAGELIST_H
