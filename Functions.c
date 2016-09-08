//
// Created by Ido Ilani on 08/09/2016.
//
#include "Functions.h"

#include "SPPoint.h"
#include "SPConfig.h"
#define MAX_LEN 1024
#define WRITE "w"
#define SEP "$$$"

int writeFeatures(SPConfig config, int index, int numberOfFeatures, SPPoint *arr){
    int res=SP_CONFIG_SUCCESS;
    char path[MAX_LEN]="";
    FILE* file=NULL;
    int dimIndex;
    if(!dimensionOk(arr,numberOfFeatures)){// check if all points have the same dimension
        res=SP_CONFIG_INVALID_ARGUMENT;
        goto fail;
    }
    int dimension=spPointGetDimension(arr[0]);
    //create .feat file path
    spConfigGetFeaturesPathFeats(path,config,index);
    file=fopen(path,WRITE);
    if(file==NULL){
        res=SP_CONFIG_CANNOT_OPEN_FILE;
        goto fail;
    }
    /**
     *each file will start like this:
    Imageindex,numberOfFeatures,Dimension
    $$$//feature number 1
     coordinate0
     coordinate1
     ...
     $$$//featuer number 2
     coordinate0
     coordinate1
     ...
     $$$
     */
    fprintf(file, "%d,%d,%d\n", index, numberOfFeatures, dimension);
    for(int i=0;i<numberOfFeatures;i++){
        fprintf(file,"%s\n",SEP);

        for(dimIndex=0;dimIndex<dimension;dimIndex++){
            fprintf(file,"%f\n",spPointGetAxisCoor(arr[i],dimIndex));
        }



    }
    fclose(file);
    return res;

    fail:
        fclose(file);
        return res;
}
bool dimensionOk(SPPoint* pointArr, int num){
    int i=0;
    if (!pointArr || num<=0){
        return false;
    }
    num=num-1;
    for (; i<num; i++){
        if (spPointGetDimension(pointArr[i])!=spPointGetDimension(pointArr[i+1])){
            return false;
        }
    }
    return true;
}



