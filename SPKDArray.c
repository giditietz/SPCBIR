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
    //printf("in sort: %lf -%lf\n",val_a,val_b);
    return (int) (val_a - val_b);
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

SPKDArray split(SPKDArray kdArr, int coor) {
    //TODO check if coor <dimention !!
    //TODO malloc/calloc assertion
    int size = spKDArrayGetSize(kdArr);
    int indexOfMedian; //TODO might add odd/even cases, should ask.
    if (size %2 ==0){
        indexOfMedian = size/2 -1;
    }
    else {
        indexOfMedian = size/2;
    }
    int leftSize = indexOfMedian+1;
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
    SPPoint *leftpointArray = (SPPoint *) malloc(indexOfMedian * sizeof(SPPoint));
    SPPoint *rightpointArray = (SPPoint *) malloc((size - indexOfMedian) * sizeof(SPPoint));
    int rightPointArrayIndex = 0;
    int leftPointArrayIndex = 0;
    for (int i = 0; i < size; i++) {
        if (arrayX[i] == 0) {
            leftpointArray[leftPointArrayIndex] = pointArray[i];
            leftPointArrayIndex++;
        }
        else {
            rightpointArray[leftPointArrayIndex] = pointArray[i];
            rightPointArrayIndex++;
        }
    }
    free(arrayX);
    //split the data matrix into two matrices: leftMatrix and rightMatrix (create a copy in O(d*n))
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
            if (num <= indexOfMedian) {
                leftdata[dim][num] = dataMatrix[dim][num];
            }
            else {
                rightdata[dim][num - indexOfMedian - 1] = dataMatrix[dim][num];

            }
        }
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


    //create maps for each new matrix


    left->pointArray = leftpointArray;
    left->size = leftSize;
    left->dim = dimension;
    right->pointArray = rightpointArray;
    right->size = rightSize;
    right->dim = dimension;


    return left, right;
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