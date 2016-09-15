//
// Created by Ido Ilani on 08/09/2016.
//
#include "stdbool.h"
#include "SPPoint.h"
#include "SPConfig.h"
#ifndef FINALPROJECT_FUNCTIONS_H
#define FINALPROJECT_FUNCTIONS_H
bool dimensionOk(SPPoint* pointArr, int num);
int writeFeatures(SPConfig config,int index,int numberOfFeatures,SPPoint* arr);
int readFeatures(SPConfig config,int index,int* featuresNumber,SPPoint** imageAndFeatures,bool created);
int getIndexFromFeatFile(char** line,int* imageIndex);
int getNumFeaturesFromFeatFile(char** line,int* featuresNum,int *dim);
//int getDimensionFromFeatFile(char* line,int* dimension);
int createPointsArray(SPPoint** ptr, int len,bool created);
int sumAllFeatures(int* featuresArr,int imageNum);
int createAllImagesPointsArr(SPPoint** result,SPPoint** source,int imageNum,int resultSize,int* featuresNum);

#endif //FINALPROJECT_FUNCTIONS_H
