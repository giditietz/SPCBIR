#include "SPPoint.h"

#ifndef SPCBIR_INDEXEDPOINT_H
#define SPCBIR_INDEXEDPOINT_H


/*Wraps a point with a new struct, so after sorting the points we will use their original indices.
 *Used only for KDArrayInit.
 */
typedef struct indexed_point {
    int pointIndex;
    SPPoint point;
} *IndexedPoint;


IndexedPoint indexedPointInit(SPPoint point, int index);

double indexedPointGetPointAxisData(IndexedPoint ipoint, int axis);

int indexedPointGetIndex(IndexedPoint);

void indexedPointDestroy(IndexedPoint);


#endif //SPCBIR_INDEXEDPOINT_H
