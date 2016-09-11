#include "SPKDArray.h"
#include <stdio.h>
#include <stdlib.h>
#include "SPPoint.h"
#include "IndexedPoint.h"
#include "SPLogger.h"
#include "Malloc_Macro.h"

int cur_coor = 0;
typedef struct sp_kdarray {
    int dim;
    int size;
    int dim_to_split;
    int **dataMatrix;
    SPPoint *pointArray;
} *SPKDArray;



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
    if ((val_a - val_b) > 0) {
        return 1;
    }
    if ((val_a - val_b) < 0) {
        return -1;
    }
    else {
        return 0; //TODO might add an index comparison so if 'index_a'<'index_b', 'a' will be first
    }
}

/**
 * Initialize a KDArray struct
 * @param arr - a SPPoint array containing all the features
 * @param size - the number of features/points in arr
 * @return an initialized KDArray.
 */
SPKDArray spKDArrayInit(SPPoint *arr, int size) {
    SPPoint *pointArray = NULL;
    SPKDArray final = NULL;
    int **data = NULL;
    IndexedPoint indexedPointArray[size];
    int dimension = spPointGetDimension(arr[0]);
    //TODO malloc/calloc assertion
    //TODO logger
    pointArray = (SPPoint *) malloc(size * sizeof(SPPoint));
    if (NULL == pointArray) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    for (int i = 0; i < size; i++) {
        pointArray[i] = spPointCopy(arr[i]);
    }
    //we no longer need the "arr":
    for (int k = 0; k < size ; k++) {
        spPointDestroy(arr[k]);
    }
    FREE_MACRO(arr);
    for (int i = 0; i < size; i++) {
        indexedPointArray[i] = indexedPointInit(pointArray[i], i);
    }

    final = (SPKDArray) malloc(sizeof(*final)); //might be an issue
    if (NULL == final) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    //dynamically allocate an int matrix, it will assigned to the KDArray
    data = (int **) malloc(dimension * sizeof(int *));
    if (NULL == data) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }

    for (int i = 0; i < dimension; i++) {
        data[i] = (int *) malloc(size * sizeof(int));
        if (NULL == data[i]) {
            spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);
            goto fail;
        }
    }
    for (int coor = 0; coor < dimension; coor++) {
        cur_coor = coor; //update global var for comparison function
        qsort(indexedPointArray, size, sizeof(IndexedPoint), compare);
/*        printf("printing image order after sort by coor %d\n", coor);
        for (int j = 0; j < size; j++) {
            int ind = spPointGetIndex(arr[j]);
            printf("%d ", ind);
        }
        puts("\n");*/
        for (int num = 0; num < size; num++) {
            data[coor][num] = indexedPointGetIndex(indexedPointArray[num]);

        }
    }
/*    printf("\nmatrix\n");
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", data[i][j]);
        }
        puts("");
    }*/
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
    spLoggerPrintInfo("KDArray has been initialized.");
    spLoggerPrintInfo("KDArray has been initialized.");
    return final;

    fail:
        for (int i = 0; i < size; i++) {
            FREE_MACRO(pointArray[i]);
        }
        FREE_MACRO(final);
        FREE_MACRO(data);
        spLoggerPrintInfo("KDArray initialization has failed.");

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
    //TODO malloc/calloc assertion
    SPKDArray left = NULL;
    SPKDArray right =NULL;
    int **dataMatrix = NULL;


    int size = spKDArrayGetSize(kdArr);
    int indexOfMedian;
    if (size % 2 == 0) {
        indexOfMedian = size / 2 - 1;
    }
    else {
        indexOfMedian = size / 2;
    }
    int leftSize = indexOfMedian + 1;
    int rightSize = size - leftSize;
    int dimension = spKDArrayGetDim(kdArr);
    left = (SPKDArray) malloc(sizeof(*left));
    if (NULL == left) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);

        //TODO raise memory allocation error
    }
    right = (SPKDArray) malloc(sizeof(*right));
    if (NULL == right) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);

        //TODO raise memory allocation error
    }
    dataMatrix = spKDArrayGetDataMatrix(kdArr);
    SPPoint *pointArray = spKDArrayGetPointArray(kdArr);
    //create arrayX:
    int *arrayX = (int *) calloc(size, sizeof(int));
    if (NULL == arrayX) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);

        //TODO raise memory allocation error
    }
    for (int i = indexOfMedian + 1;
         i < size; i++) { //since the datamatrix is sorted, we just need to take the values to the right of the median
        int index = dataMatrix[coor][i];
        arrayX[index] = 1;
    }
/*    puts("\nArray X:");
    for (int i = 0; i < size; i++) {
        printf("%d ", arrayX[i]);
    }*/
    //split the pointArray to two pointArrays using arrayX: leftPointArray and rightPointArray
    SPPoint *rightPointArray = (SPPoint *) malloc((rightSize) * sizeof(SPPoint));
    if (NULL == rightPointArray) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);

        //TODO raise memory allocation error
    }
    SPPoint *leftPointArray = (SPPoint *) malloc((leftSize) * sizeof(SPPoint));
    if (NULL == leftPointArray) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);

        //TODO raise memory allocation error
    }
    int rightPointArrayIndex = 0;
    int leftPointArrayIndex = 0;
     //puts("\npoint array:");
/*    for (int i = 0; i < size; i++) {
         printf("%d ", spPointGetIndex(pointArray[i]));
     }*/
    for (int i = 0; i < size; i++) {
        int idexOfPointInGivenCoor = dataMatrix[coor][i];
        if (arrayX[idexOfPointInGivenCoor] == 0) {
            leftPointArray[leftPointArrayIndex] = spPointCopy(pointArray[idexOfPointInGivenCoor]);
            leftPointArrayIndex++;
        }
        if (arrayX[idexOfPointInGivenCoor] == 1) {
            rightPointArray[rightPointArrayIndex] = spPointCopy(pointArray[idexOfPointInGivenCoor]);
            rightPointArrayIndex++;
        }
    }

    //create maps for each new matrix
    int *leftMap = (int *) malloc(size * sizeof(int));
    if (NULL == leftMap) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);

        //TODO raise memory allocation error
    }
    int *rightMap = (int *) malloc(size * sizeof(int));
    if (NULL == rightMap) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);

        //TODO raise memory allocation error
    }
    //initialize both maps with (-1)s.
    for (int i = 0; i < size; i++) {
        leftMap[i] = -1;
        rightMap[i] = -1;
    }
    int leftNewMapping = 0;
    int rightNewMapping = 0;
    for (int i = 0; i < size; i++) {
        if (arrayX[i] == 0) {
            leftMap[i] = leftNewMapping;
            leftNewMapping++;
        }
        else {
            rightMap[i] = rightNewMapping;
            rightNewMapping++;
        }
    }

    //split the data matrix into two matrices: leftMatrix and rightMatrix (create a copy in O(d*n))
    int rightInsertionIndex = 0;
    int leftInsertionIndex = 0;
    int **leftdata = (int **) malloc(dimension * sizeof(int *));
    if (NULL == leftdata) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);

        //TODO raise memory allocation error
    }
    for (int i = 0; i < dimension; i++) {
        leftdata[i] = (int *) malloc((leftSize) * sizeof(int));
        if (NULL == leftdata[i]) {
            spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);

            //TODO raise memory allocation error
        }
    }
    int **rightdata = (int **) malloc(dimension * sizeof(int *));
    if (NULL == rightdata) {
        spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);

        //TODO raise memory allocation error
    }
    for (int i = 0; i < dimension; i++) {
        rightdata[i] = (int *) malloc((rightSize) * sizeof(int));
        if (NULL == rightdata[i]) {
            spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);

            //TODO raise memory allocation error
        }
    }

/*
    puts("\nleft map:");
    for (int i = 0; i < size; i++) {
        printf("%d ", leftMap[i]);
    }
    puts("\nright map:");
    for (int i = 0; i < size; i++) {
        printf("%d ", rightMap[i]);
    }
*/

    for (int dim = 0; dim < dimension; dim++) {
        for (int num = 0; num < size; num++) {
            if (arrayX[dataMatrix[dim][num]] == 0) { //left KDArray. Using arrayX to determine.
                leftdata[dim][leftInsertionIndex] = leftMap[dataMatrix[dim][num]];
                leftInsertionIndex++;
            }
            else { //right KDArray. Using arrayX to determine.
                int d = rightMap[dataMatrix[dim][num]];
                rightdata[dim][rightInsertionIndex] = d;
                rightInsertionIndex++;
            }

        }
        leftInsertionIndex = 0;
        rightInsertionIndex = 0;
        //puts("");
    }
 /*   printf("\nleft matrix\n");
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < leftSize; j++) {
            printf("%d ", leftdata[i][j]);
        }
        puts("");
    }
    printf("\nright matrix\n");
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < rightSize; j++) {
            printf("%d ", rightdata[i][j]);
        }
        puts("");
    }*/





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


    SPKDArray ret_array[2] = {left, right};
    free(arrayX);
    free(leftMap);
    free(rightMap);
    //spKDArrayDestroy(kdArr);

    return ret_array;

    fail:
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

