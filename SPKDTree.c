#include <stdlib.h>
#include "SPKDArray.h"
#include "SPPoint.h"
#include "SPKDTree.h"
#include <stdio.h>
#include <time.h>

int CURRENT_COORDINATE = 0;
typedef struct sp_kd_node {
    int dim;
    double val;
    SPKDNode *left;
    SPKDNode *right;
    SPPoint data;
    SPLITMETHOD splitMethod;
} *SPKDNode;


SPKDNode init_kd_tree(SPKDArray spkdArray, SPLITMETHOD splitmethod) {
    SPKDNode root = (SPKDNode) malloc(sizeof(*root));
    root->splitMethod = splitmethod;
    time_t t;
    int max_dim = spKDArrayGetDim(spkdArray);
    //TODO raise exception if splitMethod is invalid
    //TODO (spKDArrayGetSize(spkdArray)==1) -> error
    int dim_for_split;
    if (spKDArrayGetSize(spkdArray) == 1) { //stopping condition
        //TODO dim & val suppose to be INVALID, what does that mean?
        root->left = NULL;
        root->right = NULL;
        root->data = spKDArrayGetPointArray(spkdArray)[0];

    }
    else {
        if (root->splitMethod == MAX_SPREAD) {

        }
        if (root->splitMethod == RANDOM) {
            srand((unsigned) time(&t));
            dim_for_split = rand() % (max_dim - 1);
        }
        if (root->splitMethod == INCREMENTAL) {
            dim_for_split = root->dim + 1; //TODO verify correctness
        }
    }

    return root;
}
