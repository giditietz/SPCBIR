#include "SPKDArray.h"
#include "SPBPriorityQueue.h"


#ifndef SPKDTREE_H
#define SPKDTREE_H
typedef struct sp_kd_node *SPKDNode;

typedef enum SPSplitMethod {
    MAX_SPREAD, RANDOM, INCREMENTAL
} SPLITMETHOD;

/**
 *
 * @param spkdArray
 * @param splitMethod
 */
SPKDNode spKDTreeInit(SPKDArray spkdArray, SPLITMETHOD splitMethod);

bool spKDTreeIsLeaf(SPKDNode);

int spKDTreeNodeGetDim(SPKDNode);

double spKDTreeNodeGetVal(SPKDNode);

SPKDNode spKDTreeNodeGetLeftChild(SPKDNode);

SPKDNode spKDTreeNodeGetRightChild(SPKDNode);

SPPoint spKDTreeNodeGetData(SPKDNode);

bool spKDTreeKNNSearch(SPKDNode curr, SPBPQueue bpq, SPPoint queryPoint);

void spKDTreeDestroy(SPKDNode node);

#endif //SPCBIR_SPKDTREE_H
