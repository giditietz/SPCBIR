#include <cstdio>
#include <cstdlib>

extern "C" {
#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPBPriorityQueue.h"
#include "SPKDTree.h"
#include "SPLogger.h"
#include "SPFinalImageList.h"

}


int main() {
    spLoggerCreate(NULL, SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL);

/*    int dim = 5;
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
    SPKDArray kd = init(arr, 5);*/

    double arra1[2] = {1, 2};
    SPPoint pp1 = spPointCreate(arra1, 2, 0);
    double arra2[2] = {123, 11};
    SPPoint pp2 = spPointCreate(arra2, 2, 0);
    double arra3[2] = {2, 7};
    SPPoint pp3 = spPointCreate(arra3, 2, 2);
    double arra4[2] = {9, 70};
    SPPoint pp4 = spPointCreate(arra4, 2, 3);
    double arra5[2] = {3, 4};
    SPPoint pp5 = spPointCreate(arra5, 2, 4);
    double arra6[2] = {320, 321};
    SPPoint pp6 = spPointCreate(arra6, 2, 5);
    double arra7[2] = {500, 501};
    SPPoint pp7 = spPointCreate(arra7, 2, 6);
    SPPoint parra[6] = {pp1, pp2, pp3, pp4, pp5, pp6};
    SPKDArray kdb = init(parra, 6);


    double a0[2] = {1, 1};
    SPPoint f0 = spPointCreate(a0, 2, 0);
    double a1[2] = {4, 1};
    SPPoint f1 = spPointCreate(a1, 2, 0);
    double a2[2] = {1, 4};
    SPPoint f2 = spPointCreate(a2, 2, 1);
    double a3[2] = {4, 4};
    SPPoint f3 = spPointCreate(a3, 2, 1);
    double a4[2] = {1, 8};
    SPPoint f4 = spPointCreate(a4, 2, 2);
    double a5[2] = {4, 8};
    SPPoint f5 = spPointCreate(a5, 2, 2);
    double a6[2] = {1, 11};
    SPPoint f6 = spPointCreate(a6, 2, 3);
    double a7[2] = {4, 11};
    SPPoint f7 = spPointCreate(a7, 2,3);
    double a8[2] = {10, 11};
    SPPoint f8 = spPointCreate(a8, 2, 4);
    double a9[2] = {13, 11};
    SPPoint f9 = spPointCreate(a9, 2, 4);
    double a10[2] = {10, 8};
    SPPoint f10 = spPointCreate(a10, 2, 5);
    double a11[2] = {13, 8};
    SPPoint f11 = spPointCreate(a11, 2, 5);
    double a12[2] = {10, 4};
    SPPoint f12 = spPointCreate(a12, 2, 6);
    double a13[2] = {13, 4};
    SPPoint f13 = spPointCreate(a13, 2, 6);
    double a14[2] = {10, 1};
    SPPoint f14 = spPointCreate(a14, 2, 7);
    double a15[2] = {13, 1};
    SPPoint f15 = spPointCreate(a15, 2, 7);
    SPPoint features[16] = {f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15};
    SPKDArray kdarray = init(features, 16);


    double aquery[2] = {2.5, 6};
    SPPoint f1query = spPointCreate(aquery, 2, 17);

    double bquery[2] = {2.5, 9.5};
    SPPoint f2query = spPointCreate(bquery, 2, 17);

    double cquery[2] = {7, 6};
    SPPoint f3query = spPointCreate(cquery, 2, 17);

    double dquery[2] = {7, 9.5};
    SPPoint f4query = spPointCreate(dquery, 2, 17);

    SPPoint qfeatures[4] = {f1query, f2query,f3query,f4query};

/*       SPKDArray l1;
       SPKDArray r1;
       SPKDArray *res = split(kdb, 0);
       l1 = res[0];
       r1 = res[1];
       int a = spKDArrayGetDim(kdb);
       SPPoint *rpoints = spKDArrayGetPointArray(r1);
       SPPoint *lpoints = spKDArrayGetPointArray(l1);

       puts("\nleft array");
       for (int i = 0; i < spKDArrayGetSize(l1); i++) {
           printf("%d ", spPointGetIndex(lpoints[i]));
       }
       puts("\nright array");

       for (int i = 0; i < spKDArrayGetSize(r1); i++) {
           printf("%d ", spPointGetIndex(rpoints[i]));
       }*/

    //printf("dim to split is %d",max_split);
    puts("\n");
    //SPKDNode root =  init_kd_tree(kdb,MAX_SPREAD);
    //SPKDNode root1 =  init_kd_tree(kdb,RANDOM);
    SPKDNode root2 = spKDTreeInit(kdarray, INCREMENTAL);
    SPBPQueue bpq = spBPQueueCreate(5);
    spKDTreeKNNSearch(root2, bpq, f1query);
    spKDTreeKNNSearch(root2, bpq, f1query);
    int* images = spGetFinalImageList(root2,qfeatures,4,8,4,4);

    for (int i=0;i <4;i++){
        printf("%d ",images[i]);
    }
    return 0;
}
