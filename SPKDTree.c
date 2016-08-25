#include <stdlib.h>
#include "SPKDArray.h"
#include "SPPoint.h"
#include "SPKDTree.h"
#include <stdio.h>
#include <time.h>
#include <float.h>

int IS_ROOT = true;

typedef struct sp_kd_node {
    int dim;
    double val;
    SPKDNode left;
    SPKDNode right;
    SPPoint data;
    SPLITMETHOD splitMethod;
} *SPKDNode;


int findMaxSpreadDimension(SPKDArray spkdArray) {
    double min;
    double max;
    double spread = 0;
    double max_spread = 0;
    int dim_for_split = 0;
    int dimension = spKDArrayGetDim(spkdArray);
    int size = spKDArrayGetSize(spkdArray);
    int **dataMatrix = spKDArrayGetDataMatrix(spkdArray);
    SPPoint *pointList = spKDArrayGetPointArray(spkdArray);
    for (int i = 0; i < dimension; i++) {
        min = spPointGetAxisCoor(pointList[dataMatrix[i][0]],
                                 i);//each first entry in a row of the matrix is the index of the point with the minimal value by axis.
        max = spPointGetAxisCoor(pointList[dataMatrix[i][size - 1]],
                                 i);//each last entry in a row of the matrix is the index of the point with the maximal value by axis.
        spread = max - min;
        if (spread > max_spread) {
            max_spread = spread;
            dim_for_split = i;
        }
    }
    printf("\ndim to split: %d, max spread: %lf\n", dim_for_split, max_spread);
    return dim_for_split;
}


double findMedianValueByCoor(SPKDArray spkdArray, int coor) {
    int location_of_median;
    int point_num = spKDArrayGetSize(spkdArray);
    int **dataMatrix = spKDArrayGetDataMatrix(spkdArray);
    SPPoint *pointList = spKDArrayGetPointArray(spkdArray);
    if (point_num % 2 == 0) {
        location_of_median = point_num / 2 - 1;
    }
    else {
        location_of_median = point_num / 2;
    }
    return spPointGetAxisCoor(pointList[dataMatrix[coor][location_of_median]], coor);


}

SPKDNode init_kd_tree(SPKDArray spkdArray, SPLITMETHOD splitmethod) {
    SPKDNode root = (SPKDNode) malloc(sizeof(*root));
    root->splitMethod = splitmethod;
    time_t t;
    int max_dim = spKDArrayGetDim(spkdArray);
    SPKDArray *res;
    SPKDArray left;
    SPKDArray right;

    //TODO raise exception if splitMethod is invalid
    //TODO (spKDArrayGetSize(spkdArray)==1) -> error
    int dim_for_split = 0;
    if (spKDArrayGetSize(spkdArray) == 1) { //stopping condition
        //TODO dim & val suppose to be INVALID, what does that mean? INFTY?
        root->dim =-1;
        root->left = NULL;
        root->right = NULL;
        root->data = spKDArrayGetPointArray(spkdArray)[0];
        return root;
    }
    else {
        if (root->splitMethod == MAX_SPREAD) {
            dim_for_split = findMaxSpreadDimension(spkdArray);
        }
        else if (root->splitMethod == RANDOM) {
            srand((unsigned) time(&t));
            dim_for_split = rand() % (max_dim);
        }
        else if (root->splitMethod == INCREMENTAL) {
            if (IS_ROOT) {
                dim_for_split = 0;
                IS_ROOT = false;
            }
            else {
                dim_for_split = (spKDArrayGetDimToSplit(spkdArray) + 1) % (max_dim); //TODO verify correctness}
            }
        }
        res = split(spkdArray, dim_for_split);
        left = res[0];
        right = res[1];
        root->dim = dim_for_split;
        root->val = findMedianValueByCoor(spkdArray, dim_for_split);
        root->data = NULL;
        root->left = init_kd_tree(left, splitmethod);
        root->right = init_kd_tree(right, splitmethod);
        return root;

    }
}

