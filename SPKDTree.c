#include <stdlib.h>
#include "SPKDArray.h"
#include "SPPoint.h"
#include "SPKDTree.h"
#include <stdio.h>
#include <time.h>
#include <float.h>

int CURRENT_COORDINATE = 0;
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
    double spread;
    double max_spread = 0;
    int dim_for_split = 0;
    int dimension = spKDArrayGetDim(spkdArray);
    int size = spKDArrayGetSize(spkdArray);
    int **dataMatrix = spKDArrayGetDataMatrix(spkdArray);
    SPPoint *pointList = spKDArrayGetPointArray(spkdArray);
    for (int i = 0; i < dimension; i++) {
        min = spPointGetAxisCoor(pointList[dataMatrix[0][i]],
                                 i);//each first entry in a row of the matrix is the index of the point with the minimal value by axis.
        max = spPointGetAxisCoor(pointList[dataMatrix[size - 1][i]],
                                 i);//each last entry in a row of the matrix is the index of the point with the maximal value by axis.
        spread = max - min;
        if (spread > max_spread) {
            max_spread = spread;
            dim_for_split = i;
        }
    }
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
    return spPointGetAxisCoor(pointList[dataMatrix[location_of_median][coor]], coor);


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
        //TODO dim & val suppose to be INVALID, what does that mean?
        root->left = NULL;
        root->right = NULL;
        root->data = spKDArrayGetPointArray(spkdArray)[0];
        return root;
    }
    else {
        if (root->splitMethod == MAX_SPREAD) {
            dim_for_split = findMaxSpreadDimension(spkdArray);//TODO verify correctness
        }
        else if (root->splitMethod == RANDOM) {
            srand((unsigned) time(&t));
            dim_for_split = rand() % (max_dim - 1);
        }
        else if (root->splitMethod == INCREMENTAL) {
            dim_for_split = root->dim + 1; //TODO verify correctness
        }
        res = split(spkdArray, dim_for_split);
        left = res[0];
        right = res[1];
        root->val = findMedianValueByCoor(spkdArray, dim_for_split);
        root->data = NULL;
        root->left = init_kd_tree(left, splitmethod);
        root->right = init_kd_tree(right, splitmethod);
        return root;

    }
}

