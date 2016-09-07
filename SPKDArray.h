//
// Created by gideon tietz on 05/08/2016.
//
#include "SPPoint.h"

#ifndef SPKDARRAY_H_
#define SPKDARRAY_H_

typedef struct sp_kdarray *SPKDArray;

/** Initializes the kd-array with the data given by arr.
 * The complexity of this operation is 𝑂(𝑑 × 𝑛𝑙𝑜𝑔(𝑛)).
 **/
SPKDArray spKDArrayInit(SPPoint *arr, int size);

/**Returns two kd-arrays (kdLeft, kdRight) such that the first ⌈𝒏/𝟐⌉ points with
 * respect to the coordinate coor are in kdLeft , and the rest of the points are in kdRight
 **/
SPKDArray* split(SPKDArray kdArr, int coor);

/**
 * A getter for the dimension of the kdarray
 **/
int spKDArrayGetDim(SPKDArray kdarray);

/**
 * A getter for the size of the kdarray
 **/
int spKDArrayGetSize(SPKDArray kdarray);

/**
 * A getter for the pointArray of the kdarray
 **/
SPPoint *spKDArrayGetPointArray(SPKDArray kdarray);

/**
 * A getter for the dataMatrix of the kdarray
 **/
int **spKDArrayGetDataMatrix(SPKDArray kdarray);

int spKDArrayGetDimToSplit(SPKDArray kdarray);





//TODO Might add destroy/free
#endif //SPKDARRAY_H
