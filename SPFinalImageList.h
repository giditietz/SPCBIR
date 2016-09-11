#include "SPKDArray.h"
#include "SPBPriorityQueue.h"
#include "SPKDTree.h"
#include "SPConfig.h"

#ifndef SPCBIR_SPFINALIMAGELIST_H
#define SPCBIR_SPFINALIMAGELIST_H

int spGetFinalImageList(SPConfig cfg, SPKDNode kdtree, int *finalIndexArray, SPPoint *queryImageFeatures, int queryFeaturesNum);

#endif //SPCBIR_SPFINALIMAGELIST_H
