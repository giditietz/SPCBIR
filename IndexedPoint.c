#include <stdlib.h>
#include <stdio.h>
#include "SPPoint.h"
#include "IndexedPoint.h"

SPPoint getPoint(IndexedPoint ipoint) {
    return ipoint->point;
}

IndexedPoint indexedPointInit(SPPoint point, int index) {
    IndexedPoint ipoint = (IndexedPoint) malloc(sizeof(ipoint));
    ipoint->point = point;
    ipoint->pointIndex = index;
    return ipoint;
}

double indexedPointGetPointAxisData(IndexedPoint ipoint, int axis) {
    return spPointGetAxisCoor(getPoint(ipoint), axis);
}


int indexedPointGetIndex(IndexedPoint ipoint) {
    return ipoint->pointIndex;
}


void indexedPointDestroy(IndexedPoint ipoint) {
    free(ipoint);
}
