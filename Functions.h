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
#endif //FINALPROJECT_FUNCTIONS_H
