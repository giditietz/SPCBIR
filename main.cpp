#include <cstdio>
#include <cstdlib>

extern "C" {
#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPBPriorityQueue.h"
}


int main() {
    int dim = 5;
    double arr1[5] = {4.1, 1, 2, 6, 7};
    SPPoint p1 = spPointCreate(arr1, dim, 0);
    double arr2[5] = {3.1, 2, 3, 5, 6.4};
    SPPoint p2 = spPointCreate(arr2, dim, 1);
    double arr3[5] = {2.1, 3, 4, 19, 0.1};
    SPPoint p3 = spPointCreate(arr3, dim, 2);
    double arr4[5] = {1.1, 4, 5, 14.2, 3.45};
    SPPoint p4 = spPointCreate(arr4, dim, 3);
    double arr5[5] = {0.1, 5, 1, 20, 0.5};
    SPPoint p5 = spPointCreate(arr5, dim, 4);
    SPPoint arr[5] = {p1, p2, p3, p4, p5};
    SPKDArray kd = init(arr, 5);


    double arra1[2] = {1, 2};
    SPPoint pp1 = spPointCreate(arra1, 2, 0);
    double arra2[2] = {123, 11};
    SPPoint pp2 = spPointCreate(arra2, 2, 1);
    double arra3[2] = {2, 7};
    SPPoint pp3 = spPointCreate(arra3, 2, 2);
    double arra4[2] = {9, 70};
    SPPoint pp4 = spPointCreate(arra4, 2, 3);
    double arra5[2] = {3, 4};
    SPPoint pp5 = spPointCreate(arra5, 2, 4);
    double arra6[2] = {1000, 1000};
    SPPoint pp6 = spPointCreate(arra6, 2, 5);
    double arra7[2] = {1000, 990.2423};
    SPPoint pp7 = spPointCreate(arra7, 2, 6);
    SPPoint arra[7] = {pp1, pp2, pp3, pp4, pp5,pp6,pp7};
    SPKDArray kdb = init(arra, 7);

    SPKDArray l1;
    SPKDArray r1;
    SPKDArray *res = split(kdb, 0);
    l1 = res[0];
    r1 = res[1];
    int a = spKDArrayGetDim(kdb);
    SPPoint * rpoints = spKDArrayGetPointArray(r1);
    SPPoint * lpoints = spKDArrayGetPointArray(l1);

    puts("\nleft array");
    for (int i = 0; i < spKDArrayGetSize(l1); i++) {
        printf("%d ", spPointGetIndex(lpoints[i]));
    }
    puts("\nright array");

    for (int i = 0; i < spKDArrayGetSize(r1); i++) {
        printf("%d ", spPointGetIndex(rpoints[i]));
    }

    puts("\n");




    return 0;
}
