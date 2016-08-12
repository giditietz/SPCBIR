#include "SPPoint.h"
#include<assert.h>
#include <stdlib.h>
#include <stdio.h>

struct sp_point_t{
	double* data;
	int dim;
	int index;
};

/**
 * creats a copy of the array
 */
double* copyArray(double* array,int size){
	double* copy=(double*)malloc(sizeof(double)*size);
	if(copy==NULL)
		return NULL;
	for(int i=0;i<size;i++){
		copy[i]=array[i];
	}
	return copy;
}

/**
 * Allocates a new point in the memory.
 * Given data array, dimension dim and an index.
 * The new point will be P = (p_0,p_2,...,p_{dim-1})
 * such that the following holds
 *
 * - The ith coordinate of the P will be p_i
 * - p_i = data[i]
 * - The index of P = index
 *
 * @return
 * NULL in case allocation failure ocurred OR data is NULL OR dim <=0 OR index <0
 * Otherwise, the new point is returned
 */
SPPoint spPointCreate(double* data, int dim, int index){
	if(data==NULL)
		return NULL;
	if(dim<=0)
		return NULL;
	if(index<0)
		return NULL;
	struct sp_point_t *res=(struct sp_point_t*)malloc(sizeof(struct sp_point_t));
	if(res==NULL){
		return NULL;
	}
	double* dataCopy=copyArray(data,dim);
	res->data=dataCopy;
	res->dim=dim;
	res->index=index;
	return res;
}

/**
 * Allocates a copy of the given point.
 *
 * Given the point source, the functions returns a
 * new pint P = (P_1,...,P_{dim-1}) such that:
 * - P_i = source_i (The ith coordinate of source and P are the same)
 * - dim(P) = dim(source) (P and source have the same dimension)
 * - index(P) = index(source) (P and source have the same index)
 *
 * @param source - The source point
 * @assert (source != NUlL)
 * @return
 * NULL in case memory allocation occurs
 * Others a copy of source is returned.
 */
SPPoint spPointCopy(SPPoint source){
	SPPoint copy;
	assert(source != NULL);
	double* data=copyArray(source->data,source->dim);
	int dim=source->dim;
	int index=source->index;
	copy=spPointCreate(data,dim,index);
	free(data);

	return copy;

}
/**
 * Free all memory allocation associated with point,
 * if point is NULL nothing happens.
 */


void spPointDestroy(SPPoint point){
	if (point==NULL){
		return;
	}
	if(point->data==NULL){
		free(point);
		return;
	}

	free(point->data);
	free(point);
	point=NULL;
	return;
}
/**
 * A getter for the dimension of the point
 *
 * @param point - The source point
 * @assert point != NULL
 * @return
 * The dimension of the point
 */
int spPointGetDimension(SPPoint point){
	assert(point!=NULL);
	return point->dim;
}
/**
 * A getter for the index of the point
 *
 * @param point - The source point
 * @assert point != NULL
 * @return
 * The index of the point
 */
int spPointGetIndex(SPPoint point){
	assert(point!=NULL);
	return point->index;
}
/**
 * A getter for specific coordinate value
 *
 * @param point - The source point
 * @param axis  - The coordinate of the point which
 * 				  its value will be retreived
 * @assert point!=NULL && axis < dim(point)
 * @return
 * The value of the given coordinate (p_axis will be returned)
 */
double spPointGetAxisCoor(SPPoint point, int axis){
	assert(axis>=0);
	assert(point !=NULL);
	assert(axis <=spPointGetDimension(point));
	return point->data[axis];
}
/**
 * Calculates the L2-squared distance between p and q.
 * The L2-squared distance is defined as:
 * (p_1 - q_1)^2 + (p_2 - q_1)^2 + ... + (p_dim - q_dim)^2
 *
 * @param p - The first point
 * @param q - The second point
 * @assert p!=NULL AND q!=NULL AND dim(p) == dim(q)
 * @return
 * The L2-Squared distance between p and q
 */
double spPointL2SquaredDistance(SPPoint p, SPPoint q){
	assert(p!=NULL);
	assert(q!=NULL);
	assert(spPointGetDimension(p)==spPointGetDimension(q));
	int dimLen=spPointGetDimension(p);
	double sum=0;
	for(int i=0;i<dimLen;i++){
		double d=spPointGetAxisCoor(p,i)-spPointGetAxisCoor(q,i);
		sum+=d*d;
	}
	return sum;
}


