#include "SPKDArray.h"
#include <stdio.h>
#include <stdlib.h>
#include "SPPoint.h"
#include "IndexedPoint.h"
#include "SPLogger.h"
#include "Malloc_Macro.h"
#define ALLOC_ERR_STR "Memory allocation error"
#define KD_ARR_INIT "KDArray has been initialized."
#define KD_ARR_INIT_FAIL "KDArray initialization has failed."
#define ARR_CREATED_STR "ArrayX has been created"
#define LEFT_RIGHT_STR "new Left & Right point arrays were created"
#define NEW_LEFT_RIGHT_STR "new Left & Right maps were created"
#define NEW_LEFT_RIGHT_STR_MAT "new Left & Right data matrices arrays were created"
#define SPLIT_KD_STR "split KDArray was successful."
#define NEGETIVE -1
#define ZERO 0
#define ONE 1
#define TWO 2
int cur_coor = 0;
struct sp_kdarray {
    int dim;
    int size;
    int dim_to_split;
    int **dataMatrix;
    SPPoint *pointArray;
};




//TODO Add logging
//TODO remove commented-out prints.


/**
 * Destroys a KDArray
 * @param kdarray
 */
void spKDArrayDestroy(SPKDArray kdarray) {
    int size = kdarray->size;
    int dim = kdarray->dim;
    for (int i = 0; i < size; i++) {
        spPointDestroy((kdarray->pointArray)[i]);
    }
    for (int i = 0; i < dim; i++) {
        free((kdarray->dataMatrix)[i]);
    }
    free(kdarray->dataMatrix);
    free(kdarray->pointArray);
    free(kdarray);
}

/**
 * A comparator function for qsort
 * @param a - an IndexedPoint pointer
 * @param b - an IndexedPoint pointer
 */
int compare(const void *a, const void *b) {
    //printf("coor is %d\n", cur_coor);
    IndexedPoint *ipointA = (IndexedPoint *) a;
    IndexedPoint *ipointB = (IndexedPoint *) b;
    double val_a = indexedPointGetPointAxisData(*ipointA, cur_coor);
    double val_b = indexedPointGetPointAxisData(*ipointB, cur_coor);
    //printf("in sort: %lf -%lf = %lf\n", val_a, val_b, (val_a - val_b));
    if ((val_a - val_b) > ZERO) {
        return ONE;
    }
    if ((val_a - val_b) < ZERO) {
        return NEGETIVE;
    }
    else {
        return ZERO;
    }
}

/**
 * Initialize a KDArray struct
 * @param arr - a SPPoint array containing all the features
 * @param size - the number of features/points in arr
 * @return an initialized KDArray.
 */
SPKDArray spKDArrayInit(SPPoint *arr, int size) {
    //declare:
    SPPoint *pointArray = NULL;
    SPKDArray final = NULL;
    int **data = NULL;
    IndexedPoint indexedPointArray[size];
    int dimension = spPointGetDimension(arr[0]);
    pointArray = (SPPoint *) malloc(size * sizeof(SPPoint));
    if (NULL == pointArray) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    //Copy the given array "arr"
    for (int i = 0; i < size; i++) {
        pointArray[i] = spPointCopy(arr[i]);
    }
    //we no longer need the "arr", so we can destroy it:
    for (int k = 0; k < size; k++) {
        spPointDestroy(arr[k]);
    }
    FREE_MACRO(arr);

    for (int i = 0; i < size; i++) {
        indexedPointArray[i] = indexedPointInit(pointArray[i], i);
    }

    final = (SPKDArray) malloc(sizeof(*final)); //might be an issue
    if (NULL == final) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    //dynamically allocate an int matrix, it will assigned to the KDArray
    data = (int **) malloc(dimension * sizeof(int *));
    if (NULL == data) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }

    for (int i = 0; i < dimension; i++) {
        data[i] = NULL;
        data[i] = (int *) malloc(size * sizeof(int));
        if (NULL == data[i]) {
            spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
            goto fail;
        }
    }
    //Sort the list by a specific coordinate, save the sorted list in the data Matrix
    for (int coor = 0; coor < dimension; coor++) {
        cur_coor = coor; //update global var for comparison function
        qsort(indexedPointArray, size, sizeof(IndexedPoint), compare);
        for (int num = 0; num < size; num++) {
            data[coor][num] = indexedPointGetIndex(indexedPointArray[num]);

        }
    }
    final->dim = dimension;
    final->size = size;
    final->dataMatrix = data;
    final->pointArray = pointArray;
    final->dim_to_split = 0;
    cur_coor = 0;
    //destroy the indexedPointArray, it will not be used again.
    for (int i = 0; i < size; i++) {
        indexedPointDestroy(indexedPointArray[i]);
    }
    spLoggerPrintInfo(KD_ARR_INIT);
    return final;

    fail:
    for (int i = 0; i < size; i++) {
        FREE_MACRO(pointArray[i]);
    }
    FREE_MACRO(final);
    for (int i = 0; i < dimension; i++) {
        FREE_MACRO(data[i]);
    }
    FREE_MACRO(data);
    spLoggerPrintInfo(KD_ARR_INIT_FAIL);

    return NULL;
}

/**
 * Split a KDArray by a specific coordinate (axis)
 * @param kdArr - a KDArray to split
 * @param coor - the coordinate to split
 * @return
 */
SPKDArray *split(SPKDArray kdArr, int coor) {
    //TODO check if coor < dimention !!
    SPKDArray left = NULL;
    SPKDArray right = NULL;
    int **dataMatrix = NULL;
    int* arrayX = NULL;
    int *leftMap = NULL;
    int *rightMap = NULL;
    int **leftdata = NULL;
    int **rightdata = NULL;

    int size = spKDArrayGetSize(kdArr);
    int indexOfMedian;
    if (size % TWO == ZERO) {
        indexOfMedian = size / TWO - ONE;
    }
    else {
        indexOfMedian = size / TWO;
    }

    int leftSize = indexOfMedian + ONE;
    int rightSize = size - leftSize;
    int dimension = spKDArrayGetDim(kdArr);
    left = (SPKDArray) malloc(sizeof(*left));
    if (NULL == left) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    right = (SPKDArray) malloc(sizeof(*right));
    if (NULL == right) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    dataMatrix = spKDArrayGetDataMatrix(kdArr);
    SPPoint *pointArray = spKDArrayGetPointArray(kdArr);
    //create arrayX:
    arrayX = (int *) calloc(size, sizeof(int));
    if (NULL == arrayX) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    for (int i = indexOfMedian + ONE;
         i < size; i++) { //since the datamatrix is sorted, we just need to take the values to the right of the median
        int index = dataMatrix[coor][i];
        arrayX[index] = ONE;
        spLoggerPrintDebug(ARR_CREATED_STR,__FILE__,__FUNCTION__,__LINE__);
    }

    //split the pointArray to two pointArrays using arrayX: leftPointArray and rightPointArray
    SPPoint *rightPointArray = (SPPoint *) malloc((rightSize) * sizeof(SPPoint));
    if (NULL == rightPointArray) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    SPPoint *leftPointArray = (SPPoint *) malloc((leftSize) * sizeof(SPPoint));
    if (NULL == leftPointArray) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    int rightPointArrayIndex = ZERO;
    int leftPointArrayIndex = ZERO;

    for (int i = 0; i < size; i++) {
        int idexOfPointInGivenCoor = dataMatrix[coor][i];
        if (arrayX[idexOfPointInGivenCoor] == ZERO) {
            leftPointArray[leftPointArrayIndex] = spPointCopy(pointArray[idexOfPointInGivenCoor]);
            leftPointArrayIndex++;
        }
        if (arrayX[idexOfPointInGivenCoor] == ONE) {
            rightPointArray[rightPointArrayIndex] = spPointCopy(pointArray[idexOfPointInGivenCoor]);
            rightPointArrayIndex++;
        }
    }
    spLoggerPrintDebug(LEFT_RIGHT_STR,__FILE__,__FUNCTION__,__LINE__);
    //create maps for each new matrix
    leftMap = (int *) malloc(size * sizeof(int));
    if (NULL == leftMap) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    rightMap = (int *) malloc(size * sizeof(int));
    if (NULL == rightMap) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    //initialize both maps with (-1)s.
    for (int i = 0; i < size; i++) {
        leftMap[i] = NEGETIVE;
        rightMap[i] = NEGETIVE;
    }
    int leftNewMapping = ZERO;
    int rightNewMapping = ZERO;
    for (int i = 0; i < size; i++) {
        if (arrayX[i] == ZERO) {
            leftMap[i] = leftNewMapping;
            leftNewMapping++;
        }
        else {
            rightMap[i] = rightNewMapping;
            rightNewMapping++;
        }
    }
    spLoggerPrintDebug(NEW_LEFT_RIGHT_STR,__FILE__,__FUNCTION__,__LINE__);

    //split the data matrix into two matrices: leftMatrix and rightMatrix (create a copy in O(d*n))
    int rightInsertionIndex = ZERO;
    int leftInsertionIndex = ZERO;
    leftdata = (int **) malloc(dimension * sizeof(int *));
    if (NULL == leftdata) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    for (int i = 0; i < dimension; i++) {
        leftdata[i] = (int *) malloc((leftSize) * sizeof(int));
        if (NULL == leftdata[i]) {
            spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
            goto fail;
        }
    }
    rightdata = (int **) malloc(dimension * sizeof(int *));
    if (NULL == rightdata) {
        spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    for (int i = 0; i < dimension; i++) {
        rightdata[i] = (int *) malloc((rightSize) * sizeof(int));
        if (NULL == rightdata[i]) {
            spLoggerPrintError(ALLOC_ERR_STR, __FILE__, __FUNCTION__, __LINE__);
            goto fail;
        }
    }


    for (int dim = 0; dim < dimension; dim++) {
        for (int num = 0; num < size; num++) {
            if (arrayX[dataMatrix[dim][num]] == ZERO) { //left KDArray. Using arrayX to determine.
                leftdata[dim][leftInsertionIndex] = leftMap[dataMatrix[dim][num]];
                leftInsertionIndex++;
            }
            else { //right KDArray. Using arrayX to determine.
                int d = rightMap[dataMatrix[dim][num]];
                rightdata[dim][rightInsertionIndex] = d;
                rightInsertionIndex++;
            }

        }
        leftInsertionIndex = ZERO;
        rightInsertionIndex = ZERO;

    }
    spLoggerPrintDebug(NEW_LEFT_RIGHT_STR_MAT,__FILE__,__FUNCTION__,__LINE__);


    //time to assign all data fields!
    left->pointArray = leftPointArray;
    left->size = leftSize;
    left->dim = dimension;
    left->dim_to_split = coor;
    left->dataMatrix = leftdata;
    right->pointArray = rightPointArray;
    right->size = rightSize;
    right->dim = dimension;
    right->dim_to_split = coor;
    right->dataMatrix = rightdata;


    //SPKDArray ret_array[2] = {left, right};
    SPKDArray* ret_array=(SPKDArray*)malloc(TWO*sizeof(SPKDArray));
    ret_array[ZERO]=left;
    ret_array[ONE]=right;
    free(arrayX);
    free(leftMap);
    free(rightMap);
    spKDArrayDestroy(kdArr);
    spLoggerPrintDebug(SPLIT_KD_STR,__FILE__,__FUNCTION__,__LINE__);
    return ret_array;

    fail:
        free(arrayX);
        free(leftMap);
        free(rightMap);
        spKDArrayDestroy(kdArr);
        spKDArrayDestroy(left);
        spKDArrayDestroy(right);
        //TODO Some frees are missing (for matrices)
        return NULL;

}

/**
 * a getter function
 * @param kdarray
 * @return dim of the KDArray
 */
int spKDArrayGetDim(SPKDArray kdarray) {
    return kdarray->dim;
}

/**
 * a getter function
 * @param kdarray
 * @return size of the KDArray
 */
int spKDArrayGetSize(SPKDArray kdarray) {
    return kdarray->size;
}

/**
 * a getter function
 * @param kdarray
 * @return point array of the KDArray
 */
SPPoint *spKDArrayGetPointArray(SPKDArray kdarray) {
    return kdarray->pointArray;
}

/**
 * a getter function
 * @param kdarray
 * @return data matrix of the KDArray
 */
int **spKDArrayGetDataMatrix(SPKDArray kdarray) {
    return kdarray->dataMatrix;
}

/**
 * a getter function
 * @param kdarray
 * @return dim_to_split of the KDArray
 */
int spKDArrayGetDimToSplit(SPKDArray kdarray) {
    return kdarray->dim_to_split;
}

