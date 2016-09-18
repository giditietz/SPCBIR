//
// Created by Ido Ilani on 08/09/2016.
//
#include "stdbool.h"
#include "SPPoint.h"
#include "SPConfig.h"
#ifndef FINALPROJECT_FUNCTIONS_H
#define FINALPROJECT_FUNCTIONS_H
/**
 * checks that all points dimension are the same
 * @param point arr
 * @param num
 * @return true if all dimensions are identical, else false
 */
bool dimensionOk(SPPoint* pointArr, int num);
/**
 * if extractionMode==true this function writes the features into feats file format
 * @param config
 * @param index
 * @param numberOfFeatures
 * @param arr
 * @return SUCCESS if no error occured, else returns the error msg
 */
int writeFeatures(SPConfig config,int index,int numberOfFeatures,SPPoint* arr);
/**
 * if extractionMode==false this function reads the features according to the img feats format
 * @param config
 * @param index
 * @param featuresNumber
 * @param imageAndFeatures
 * @param created
 * @return SUCCESS if no error occured, else returns the error msg
 */
int readFeatures(SPConfig config,int index,int* featuresNumber,SPPoint** imageAndFeatures,bool created);
/**
 * create points array and allocate space for each SPPoint using the len parameter
 * @param ptr
 * @param len
 * @param created
 * @return SUCCESS if no error occured, else returns the error msg
 */
int createPointsArray(SPPoint** ptr, int len,bool created);
/**
 * sum all the features in featureArr
 * @param featuresArr
 * @param imageNum
 * @return sum of all features in featureArr
 */
int sumAllFeatures(int* featuresArr,int imageNum);
/**
 * creats the whole image points arr given the image number and the result arr to put into.
 * @param result
 * @param source
 * @param imageNum
 * @param resultSize
 * @param featuresNum
 * @return SUCCESS if no error occured, else returns the error msg
 */
int createAllImagesPointsArr(SPPoint** result,SPPoint** source,int imageNum,int resultSize,int* featuresNum);

#endif //FINALPROJECT_FUNCTIONS_H
