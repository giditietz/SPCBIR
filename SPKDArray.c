#include "SPKDArray.h"
#include <stdio.h>
#include <stdlib.h>
#include "SPPoint.h"

int cur_coor = 0;
typedef struct sp_kdarray {
    int dim;
    int size;
    int **dataMatrix;
    SPPoint *pointArray;
} *KDArray;

//TODO Add logging
//TODO remove commented-out prints.


int compare(const void *a, const void *b) {
    //printf("coor is %d\n",cur_coor);
    SPPoint *pointA = (SPPoint *) a;
    SPPoint *pointB = (SPPoint *) b;
    double val_a = spPointGetAxisCoor(*pointA, cur_coor);
    double val_b = spPointGetAxisCoor(*pointB, cur_coor);
    //printf("in sort: %lf -%lf = %lf\n",val_a,val_b,(val_a - val_b));
    if ((val_a - val_b) > 0) {
        return 1;
    }
    else {
        return 0;
    }
}

SPKDArray init(SPPoint *arr, int size) {
    //TODO malloc/calloc assertion
    //TODO logger
    SPPoint *pointArray = (SPPoint *) malloc(size * sizeof(SPPoint));
    for (int i = 0; i < size; i++) {
        pointArray[i] = spPointCopy(arr[i]);
    }
    SPKDArray final = (SPKDArray) malloc(sizeof(*final)); //might be an issue
    int dimension = spPointGetDimension(arr[0]);
    //dynamically allocate an int matrix, it will assigned to the KDArray
    int **data = (int **) malloc(dimension * sizeof(int *));
    for (int i = 0; i < dimension; i++) {
        data[i] = (int *) malloc(size * sizeof(int));
    }
    for (int coor = 0; coor < dimension; coor++) {
        cur_coor = coor; //update global var for comparison function
        qsort(arr, size, sizeof(SPPoint), compare);
        //printf("printing image order after sort by coor %d\n", coor);
        // for (int j = 0; j < size; j++) {
        //   int ind = spPointGetIndex(arr[j]);
        //  printf("%d ", ind); }
        //puts("\n");
        for (int num = 0; num < size; num++) {
            data[coor][num] = spPointGetIndex(arr[num]);

        }
    }
    printf("\nmatrix\n");
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", data[i][j]);
        }
        puts("");
    }
    final->dim = dimension;
    final->size = size;
    final->dataMatrix = data;
    final->pointArray = pointArray;
    cur_coor = 0;
    return final;
}

SPKDArray *split(SPKDArray kdArr, int coor) {
    //TODO check if coor < dimention !!
    //TODO malloc/calloc assertion
    int size = spKDArrayGetSize(kdArr);
    int indexOfMedian; //TODO might add odd/even cases, should ask.
    if (size % 2 == 0) {
        indexOfMedian = size / 2 - 1;
    }
    else {
        indexOfMedian = size / 2;
    }
    int leftSize = indexOfMedian + 1;
    int rightSize = size - leftSize;
    int dimension = spKDArrayGetDim(kdArr);
    SPKDArray left = (SPKDArray) malloc(sizeof(*left));
    SPKDArray right = (SPKDArray) malloc(sizeof(*right));
    int **dataMatrix = spKDArrayGetDataMatrix(kdArr);
    SPPoint *pointArray = spKDArrayGetPointArray(kdArr);
    //create arrayX:
    int *arrayX = (int *) calloc(size, sizeof(int));
    for (int i = indexOfMedian + 1;
         i < size; i++) { //since the datamatrix is sorted, we just need to take the values to the right of the median
        int index = dataMatrix[coor][i];
        arrayX[index] = 1;
    }
    puts("\nArray X:");
    for (int i = 0; i < size; i++) {
        printf("%d ", arrayX[i]);
    }
    //split the pointArray to two pointArrays using arrayX: leftPointArray and rightPointArray
    SPPoint *rightPointArray = (SPPoint *) malloc((rightSize) * sizeof(SPPoint));
    SPPoint *leftPointArray = (SPPoint *) malloc((leftSize) * sizeof(SPPoint));

    int rightPointArrayIndex = 0;
    int leftPointArrayIndex = 0;
    puts("\npoint array:");
    for (int i = 0; i < size; i++) {
        printf("%d ", spPointGetIndex(pointArray[i]));
    }
    for (int i = 0; i < size; i++) {
        int idexOfPointInGivenCoor = dataMatrix[coor][i];
        if (arrayX[idexOfPointInGivenCoor] == 0) {
            leftPointArray[leftPointArrayIndex] = pointArray[idexOfPointInGivenCoor];
            leftPointArrayIndex++;
        }
        if (arrayX[idexOfPointInGivenCoor] == 1) {
            rightPointArray[rightPointArrayIndex] = pointArray[idexOfPointInGivenCoor];
            rightPointArrayIndex++;
        }
    }

    //create maps for each new matrix
    int *leftMap = (int *) malloc(size * sizeof(int));
    int *rightMap = (int *) malloc(size * sizeof(int));

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
    for (int i = 0; i < dimension; i++) {
        leftdata[i] = (int *) malloc((leftSize - 1) * sizeof(int));
    }
    int **rightdata = (int **) malloc(dimension * sizeof(int *));
    for (int i = 0; i < dimension; i++) {
        rightdata[i] = (int *) malloc((rightSize - 1) * sizeof(int));
    }
    for (int dim = 0; dim < dimension; dim++) {
        for (int num = 0; num < size; num++) {
            if (arrayX[dataMatrix[dim][num]] == 0) { //left KDArray. Using arrayX to determine.
                leftdata[dim][leftInsertionIndex] = leftMap[dataMatrix[dim][num]];
                leftInsertionIndex++;
            }
            else { //right KDArray. Using arrayX to determine.
                rightdata[dim][rightInsertionIndex] = rightMap[dataMatrix[dim][num]];
                rightInsertionIndex++;
            }

        }
        leftInsertionIndex = 0;
        rightInsertionIndex = 0;
    }
    printf("\nleft matrix\n");
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
    }



    puts("\nleft map:");
    for (int i = 0; i < size; i++) {
        printf("%d ", leftMap[i]);
    }
    puts("\nright map:");
    for (int i = 0; i < size; i++) {
        printf("%d ", rightMap[i]);
    }


    left->pointArray = leftPointArray;
    left->size = leftSize;
    left->dim = dimension;
    left->dataMatrix = leftdata;
    right->pointArray = rightPointArray;
    right->size = rightSize;
    right->dataMatrix = rightdata;
    right->dim = dimension;

    SPKDArray ret_array[2] = {left, right};
    free(arrayX);


    return ret_array;
}


int spKDArrayGetDim(SPKDArray kdarray) {
    return kdarray->dim;
}

int spKDArrayGetSize(SPKDArray kdarray) {
    return kdarray->size;
}

SPPoint *spKDArrayGetPointArray(SPKDArray kdarray) {
    return kdarray->pointArray;
}

int **spKDArrayGetDataMatrix(SPKDArray kdarray) {
    return kdarray->dataMatrix;
}