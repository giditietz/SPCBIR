#include <stdlib.h>
#include "SPKDArray.h"
#include "SPPoint.h"
#include "SPKDTree.h"
#include <stdio.h>
#include <time.h>
#include "SPBPriorityQueue.h"
#include "SPListElement.h"
#include "SPLogger.h"
#include <math.h>
#include <float.h>
#include "Malloc_Macro.h"

#define ERROR_MSG_KNN_SEARCH_FAILED "Error: KNN search has failed"
#define ERROR_MSG_KNN_SEARCH_BPQ_ENQUEUE_FAILED "Error: KNN search has failed due to a BPQueue issue"
#define ERROR_MSG_NULL_NODE "Error: node is NULL"
#define ZERO 0
#define NEGETIVE -1
#define ONE 1
#define TWO 2


int IS_ROOT = true;

struct sp_kd_node {
    int dim;
    double val;
    SPKDNode left;
    SPKDNode right;
    SPPoint data;
    SP_KD_TREE_SPLIT_METHOD splitMethod;
};


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
        int smallestPointIndex = dataMatrix[i][ZERO];
        int largestPointIndex = dataMatrix[i][size - ONE];
        min = spPointGetAxisCoor(pointList[smallestPointIndex],
                                 i);//each first entry in a row of the matrix is the index of the point with the minimal value by axis.
        max = spPointGetAxisCoor(pointList[largestPointIndex],
                                 i);//each last entry in a row of the matrix is the index of the point with the maximal value by axis.
        spread = max - min;
        if (spread > max_spread) {
            max_spread = spread;
            dim_for_split = i;
        }
    }

    //printf("\ndim to split: %d, max spread: %lf\n", dim_for_split, max_spread);
    return dim_for_split;
}


double findMedianValueByCoor(SPKDArray spkdArray, int coor) {
    int location_of_median;
    int point_num = spKDArrayGetSize(spkdArray);
    int **dataMatrix = spKDArrayGetDataMatrix(spkdArray);
    SPPoint *pointList = spKDArrayGetPointArray(spkdArray);
    if (point_num % TWO == ZERO) {
        location_of_median = point_num / TWO - ONE;
    }
    else {
        location_of_median = point_num / TWO;
    }
    return spPointGetAxisCoor(pointList[dataMatrix[coor][location_of_median]], coor);

}

SPKDNode spKDTreeInit(SPKDArray spkdArray, SP_KD_TREE_SPLIT_METHOD splitmethod) {
    SPKDNode root = (SPKDNode) malloc(sizeof(*root));
    root->splitMethod = splitmethod;
    time_t t;
    int max_dim = spKDArrayGetDim(spkdArray);
    SPKDArray *res;
    SPKDArray left;
    SPKDArray right;

    //TODO (spKDArrayGetSize(spkdArray)==1) -> error
    int dim_for_split = ZERO;
    if (spKDArrayGetSize(spkdArray) == ONE) { //stopping condition
        root->val = DBL_MAX;
        root->dim = NEGETIVE;
        root->left = NULL;
        root->right = NULL;
        root->data =spPointCopy(spKDArrayGetPointArray(spkdArray)[ZERO]);
	spKDArrayDestroy(spkdArray);
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
                dim_for_split = ZERO;
                IS_ROOT = false;
            }
            else {
                dim_for_split = (spKDArrayGetDimToSplit(spkdArray) + ONE) % (max_dim);
            }
        }
        root->dim = dim_for_split;
        root->val = findMedianValueByCoor(spkdArray, dim_for_split);
        root->data = NULL;
        res = split(spkdArray, dim_for_split);
        left = res[ZERO];
        right = res[ONE];
        free(res);
        root->left = spKDTreeInit(left, splitmethod);
        root->right = spKDTreeInit(right, splitmethod);
        return root;

    }
}

bool spKDTreeIsLeaf(SPKDNode node) {
    if ((NULL == node->left) && (NULL == node->right)) {
        return true;
    }
    else {
        return false;
    }
}

///Getters and setter for SPKDTree
int spKDTreeNodeGetDim(SPKDNode node) {
    return node->dim;
}

double spKDTreeNodeGetVal(SPKDNode node) {
    return node->val;
}

SPKDNode spKDTreeNodeGetLeftChild(SPKDNode node) {
    return node->left;
}

SPKDNode spKDTreeNodeGetRightChild(SPKDNode node) {
    return node->right;
}

SPPoint spKDTreeNodeGetData(SPKDNode node) {
    return node->data;
}


bool spKDTreeKNNSearch(SPKDNode curr, SPBPQueue bpq, SPPoint queryPoint) {
    int curr_index;
    double curr_distance;
    int cur_dim;
    SPKDNode nextNode;
    SPKDNode remainingNode;
    SP_BPQUEUE_MSG msg;

    cur_dim = spKDTreeNodeGetDim(curr);

    //if curr is null
    if (NULL == curr) {
        spLoggerPrintError(ERROR_MSG_NULL_NODE, __FILE__, __func__, __LINE__); //TODO Might change to info
        return true;
    }
        /*if curr is a leaf just enqueue it - if it's not good it won't be stored due to BPQueue's properties.*/
    else if (spKDTreeIsLeaf(curr)) {
        curr_index = spPointGetIndex(spKDTreeNodeGetData(curr));
        curr_distance = spPointL2SquaredDistance(queryPoint, spKDTreeNodeGetData(curr));
        SPListElement newElement = spListElementCreate(curr_index, curr_distance);
        msg = spBPQueueEnqueue(bpq, newElement);
        if ((msg != SP_BPQUEUE_SUCCESS) && (msg != SP_BPQUEUE_FULL)) {
            spListElementDestroy(newElement);//TODO ido handeled free
            spLoggerPrintError(ERROR_MSG_KNN_SEARCH_BPQ_ENQUEUE_FAILED, __FILE__, __func__, __LINE__);
            return false;
        }
        spListElementDestroy(newElement);//TODO ido handeled free
        return true;
    }
    else {
        /* Recursively search the half of the tree that contains the test point. */
        if (spPointGetAxisCoor(queryPoint, cur_dim) <= spKDTreeNodeGetVal(curr)) { //search the left subtree
            nextNode = spKDTreeNodeGetLeftChild(curr);
            remainingNode = spKDTreeNodeGetRightChild(curr);
            if (!spKDTreeKNNSearch(nextNode, bpq, queryPoint)) {
                spLoggerPrintError(ERROR_MSG_KNN_SEARCH_FAILED, __FILE__, __func__, __LINE__);
                return false;
            }
        }
        else {//search the right subtree
            nextNode = spKDTreeNodeGetRightChild(curr);
            remainingNode = spKDTreeNodeGetLeftChild(curr);
            if (!spKDTreeKNNSearch(nextNode, bpq, queryPoint)) {
                spLoggerPrintError(ERROR_MSG_KNN_SEARCH_FAILED, __FILE__, __func__, __LINE__);
                return false;
            }
        }
        SPListElement lastElement = spBPQueuePeekLast(bpq);
        double lastElementPriority = spListElementGetValue(lastElement);
        double distance_from_median_line = (fabs(spKDTreeNodeGetVal(curr) - spPointGetAxisCoor(queryPoint, cur_dim)));
        spListElementDestroy(lastElement); //used only for the above variable

        /* If the candidate hypersphere crosses this splitting plane,
         * look on the other side of the plane by examining the other subtree*/
        if (!spBPQueueIsFull(bpq) || distance_from_median_line < lastElementPriority) {
            if (!spKDTreeKNNSearch(remainingNode, bpq, queryPoint)) {
                spLoggerPrintError(ERROR_MSG_KNN_SEARCH_FAILED, __FILE__, __func__, __LINE__);
                return false;

            }

        }

        return true;
    }
}

//TODO verify that it's working. The tests have shown that it's fine, but we need to make sure.
void spKDTreeDestroy(SPKDNode node) {
/*    if(!node){
        return;
    }
    if (spKDTreeIsLeaf(node)) {
        if (NULL != node->data) { //data can only appear in leaves, but not every leaf contains data.
            spPointDestroy(node->data);
        }
        free(node);
    }
    else {
        if (NULL != node->left) {
            spKDTreeDestroy(node->left);
            node->left = NULL;

        }
        if (NULL != node->right) {
            spKDTreeDestroy(node->right);
            node->right = NULL;

        }
    }*/
if(node != NULL){
		spKDTreeDestroy(node->left);
		spKDTreeDestroy(node->right);
        if(node->data!=NULL){
                spPointDestroy(node->data);
        }
	}
	FREE_MACRO(node);
}
