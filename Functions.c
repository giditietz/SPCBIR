//
// Created by Ido Ilani on 08/09/2016.
//
#include "Functions.h"
#include "string.h"
#include "Malloc_Macro.h"
#include "SPPoint.h"
#include "SPPoint.h"
#include "SPConfig.h"

#define MAX_LEN 1024
#define WRITE "w"
#define SEP "$$$"
#define READ "r"

int writeFeatures(SPConfig config, int index, int numberOfFeatures, SPPoint *arr) {
    int res = SP_CONFIG_SUCCESS;
    char path[MAX_LEN] = "";
    char loggerMessage[MAX_LEN];
    FILE *file = NULL;
    int dimIndex;
    if (!dimensionOk(arr, numberOfFeatures)) {// check if all points have the same dimension
        res = SP_CONFIG_INVALID_ARGUMENT;
        spLoggerPrintError("All points do not have the same dimension", __FILE__, __func__, __LINE__);
        goto fail;
    }
    int dimension = spPointGetDimension(arr[0]);
    //create .feat file path
    spConfigGetFeaturesPathFeats(path, config, index);
    file = fopen(path, WRITE);
    if (file == NULL) {
        sprintf(loggerMessage, "Failed to open file, path:%s", path);
        spLoggerPrintError(loggerMessage, __FILE__, __func__, __LINE__);
        res = SP_CONFIG_CANNOT_OPEN_FILE;
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
    for (int i = 0; i < numberOfFeatures; i++) {
        fprintf(file, "%s\n", SEP);

        for (dimIndex = 0; dimIndex < dimension; dimIndex++) {
            fprintf(file, "%lf\n", spPointGetAxisCoor(arr[i], dimIndex));
        }
    }
    fclose(file);
    return res;

    fail:
    fclose(file);
    return res;
}

int
readFeatures(SPConfig config, int index, int *featuresNumber, SPPoint **imageAndFeatures, bool created, int imageNum) {
    int res = SP_CONFIG_SUCCESS;
    double *axis = NULL;
    FILE *featsFile = NULL;
    char path[MAX_LEN];
    char firstLine[MAX_LEN];
    int imageIndex;
    // int featureNumber;
    int dim;
    char temp[MAX_LEN];
    char loggerMessage[MAX_LEN];
    //get the image"i".feats path
    spConfigGetFeaturesPathFeats(path, config, index);
    featsFile = fopen(path, "rb");
    if (featsFile == NULL) {
        sprintf(loggerMessage, "Failed to open file, path:%s", path);
        spLoggerPrintError(loggerMessage, __FILE__, __func__, __LINE__);
        res = SP_CONFIG_CANNOT_OPEN_FILE;
        return res;
    }
    //store the first line which contains: imageIndex,NumberOfFeatures,Dimension
    fgets(firstLine, MAX_LEN, featsFile);
    char *line = (char *) malloc(MAX_LEN);
    strcpy(line, firstLine);
    res = getIndexFromFeatFile(&line, &imageIndex);
    if (res != SP_CONFIG_SUCCESS) {
        sprintf(loggerMessage, "Failed to retrieve index from imageNumber:%d", index);
        spLoggerPrintError(loggerMessage, __FILE__, __func__, __LINE__);
        goto fail;
    }
    //char* line2=(char*)malloc(MAX_LEN);
    //strcpy(line2,line);
    res = getNumFeaturesFromFeatFile(&line, featuresNumber);
    if (res != SP_CONFIG_SUCCESS) {
        sprintf(loggerMessage, "Failed to retrieve index from imageNumber:%d", index);
        spLoggerPrintError(loggerMessage, __FILE__, __func__, __LINE__);
        goto fail;
    }
    //char* line3=(char*)malloc(MAX_LEN);
    // strcpy(line3,line2);
    getDimensionFromFeatFile(line, &dim);
    // free(line);
    // free(line2);
    //free(line3);
    //read $$$
    fgets(firstLine, MAX_LEN, featsFile);
    if (strcmp(firstLine, "$$$\n") != 0) {
        res = SEP_ERROR;
        return res;
    }

    createPointsArray(imageAndFeatures, *featuresNumber, created);
    for (int k = 0; k < *featuresNumber; k++) {
        if (axis == NULL) {
            axis = (double *) malloc(sizeof(double) * dim);
        } else {
            realloc(axis, sizeof(double) * dim);
        }
        for (int i = 0; i < dim; i++) {//read data
            fgets(temp, MAX_LEN, featsFile);
            sscanf(temp, "%lf", &(axis[i]));
        }
        fgets(temp, MAX_LEN, featsFile);//read sep=$$$
        (*imageAndFeatures)[k] = spPointCreate(axis, dim, index);
        if ((*imageAndFeatures)[k] == NULL) {
            res = FAIL_READ_FEATURE;
            goto fail;
        }
    }
    strcpy(path, "");
    fclose(featsFile);
    free(axis);
    return res;
    fail:
    fclose(featsFile);
    free(axis);
    for (int i = 0; i < *(featuresNumber + i); i++) {
        spPointDestroy(*imageAndFeatures[i]);
    }
    free(imageAndFeatures);
    return res;

}

int createPointsArray(SPPoint **ptr, int len, bool created) {
    int res = SUCCESS;
    int i;
    created = false;
    if (!created) {
        MALLOC_MACRO(*ptr, SPPoint*, len * sizeof(SPPoint));
        //for (int j = 0; j < len; j++) {
        //MALLOC_MACRO(*(ptr + j), SPPoint*, len * sizeof(SPPoint));
        for (i = 0; i < len; i++) {
            (*ptr)[i] = NULL;
        }


        //}
    }


    return res;
    fail:
    FREE_MACRO(*ptr);
    return res;
}

bool dimensionOk(SPPoint *pointArr, int num) {
    int i = 0;
    if (!pointArr || num <= 0) {
        return false;
    }
    num = num - 1;
    for (; i < num; i++) {
        if (spPointGetDimension(pointArr[i]) != spPointGetDimension(pointArr[i + 1])) {
            return false;
        }
    }
    return true;
}

int getIndexFromFeatFile(char **line, int *imageIndex) {
    int res = SP_CONFIG_SUCCESS;
    char *variable;
    char *val;
    char *pointer;
    char *str;
    MALLOC_MACRO(str, char*, MAX_LEN);
    strcpy(str, *line);
    pointer = strchr(str, ',');
    variable = *line;
    val = pointer + 1;
    *pointer = '\0';
    *imageIndex = atoi(variable);
    *line = val;
    goto fail;


    fail:
    FREE_MACRO(str);
    return res;


}

int getNumFeaturesFromFeatFile(char **line, int *featuresNum) {

    int res = SP_CONFIG_SUCCESS;
    char *variable;
    char *val;
    char *pointer;
    pointer = strchr(*line, ',');
    variable = *line;
    val = pointer + 1;

    *pointer = '\0';
    *featuresNum = atoi(variable);

    *line = val;
    goto fail;


    fail:
    // FREE_MACRO(str1);
    return res;
}

int getDimensionFromFeatFile(char *line, int *dimension) {
    int res = SP_CONFIG_SUCCESS;
    *dimension = atoi(line);
    return res;
}

int sumAllFeatures(int *featuresArr, int imageNum) {
    int sum = 0;
    for (int i = 0; i < imageNum; i++) {
        sum += *(featuresArr + i);
    }
    return sum;
}

int createAllImagesPointsArr(SPPoint **result, SPPoint **source, int imageNum, int resultSize, int *featuresNum) {
    int res = SUCCESS;
    int location = 0;
    MALLOC_MACRO(*result, SPPoint*, resultSize * sizeof(SPPoint));


    for (int j = 0; j < imageNum; j++) {
        for (int t = 0; t < featuresNum[j]; t++) {
            SPPoint p = source[j][t];
            (*result)[location] = p;

            location++;
        }
    }

    return res;


    fail:
    for (int i = 0; i < resultSize; i++) {
        spPointDestroy((*result)[i]);
    }
    free(*result);
    return res;


}

