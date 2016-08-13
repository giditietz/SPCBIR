#include "SPKDArray.h"


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
SPKDNode init_kd_tree(SPKDArray spkdArray, SPLITMETHOD splitMethod);


//isLeaf
//getPoint
#endif //SPCBIR_SPKDTREE_H
