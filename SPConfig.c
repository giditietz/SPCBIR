//
// Created by Ido Ilani on 25/08/2016.
//

#include <stdlib.h>
#include "SPConfig.h"
#include "Malloc_Macro.h"
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <assert.h>

#define LINE_LENGTH 1024
#define FEATS ".feats"

int createDefaultValuesConfig(SPConfig config);

int parseConfigFile(const char *file, SPConfig config, int *numberOfLines);

int removeSpaceAndCheckValid(char **line);

int isDefaultValuesSet(SPConfig config, const char *file, int numberOfLines);

int setValuesConfig(char *variable, char *val, int lineNum, const char *file, SPConfig config);

SP_CONFIG_MSG getConfigMsg(int res);

//define sp_config_t struct (declared in SPConfig.h)
struct sp_config_t {
    char *spImagesDirectory;
    char *spImagesPrefix;
    char *spImagesSuffix;
    int spNumOfImages;
    int spPCADimension;//default=20
    char *spPCAFilename;//default=pca.yml
    int spNumOfFeatures;//default=100
    bool spExtractionMode;//default=true
    int spNumOfSimilarImages;//default=1
    SP_KD_TREE_SPLIT_METHOD spKDTreeSplitMethod;//default=MAX_SPREAD
    int spKNN;//default=1
    bool spMinimalGUI;//default=false
    int spLoggerLevel;//default=3
    char *spLoggerFilename;//default=stdout
};

SPConfig spConfigCreate(const char *filename, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    SPConfig spConfig = NULL;
    int res = SUCCESS;
    int configLines = 0;
    if (filename == NULL) {
        res = INVALID_ARGUMENT;
        goto fail;
    }
    MALLOC_MACRO(spConfig, SPConfig, sizeof(*spConfig));
    FUNC_MACRO(createDefaultValuesConfig(spConfig));
    FUNC_MACRO(parseConfigFile(filename, spConfig, &configLines));
    FUNC_MACRO(isDefaultValuesSet(spConfig, filename, configLines));
    *msg = getConfigMsg(res);
    if (res == SUCCESS)
        return spConfig;
    fail:
    *msg = getConfigMsg(res);
    spConfigDestroy(spConfig);
    return NULL;
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return false;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        if (config->spExtractionMode)
            return true;
        return false;
    }

}

bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return false;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        if (config->spMinimalGUI)
            return true;
        return false;
    }

}

SP_LOGGER_LEVEL spConfigGetLoggerLevel(const SPConfig config,SP_CONFIG_MSG *msg){
    if(config==NULL){
        *msg=SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    }else{
        *msg=SP_CONFIG_SUCCESS;
        return config->spLoggerLevel;
    }


}
void spConfigGetLoggerName(const SPConfig config,char* name){
    if(config==NULL){
        return;
    }else{
        strcpy(name,config->spLoggerFilename);
    }
}


int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spNumOfImages;
    }
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spNumOfFeatures;
    }
}

int spConfigGetSPKnn(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spKNN;
    }
}

int spConfigGetspNumOfSimilarImages(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spNumOfSimilarImages;
    }
}

SP_CONFIG_MSG spConfigGetFeaturesPathFeats(char* imagePath, const SPConfig config,
                                           int index){
    char indexStr[LINE_LENGTH];
    if(imagePath==NULL||config==NULL){
        return SP_CONFIG_INVALID_ARGUMENT;
    }
    if(index>=config->spNumOfImages){
        return SP_CONFIG_INDEX_OUT_OF_RANGE;
    }
    else{
        strcat(imagePath,config->spImagesDirectory);
        strcat(imagePath,config->spImagesPrefix);
        sprintf(indexStr,"%d",index);
        strcat(imagePath,indexStr);
        strcat(imagePath,FEATS);
        return SP_CONFIG_SUCCESS;


    }
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spPCADimension;
    }
}

SP_CONFIG_MSG spConfigGetImagePath(char *imagePath, const SPConfig config,
                                   int index) {
    char indexStr[LINE_LENGTH];
    if (imagePath == NULL || config == NULL) {
        return SP_CONFIG_INVALID_ARGUMENT;
    }
    if (index >= config->spNumOfImages) {
        return SP_CONFIG_INDEX_OUT_OF_RANGE;
    }
    else {
        strcpy(imagePath,""); //it was an error
        strcat(imagePath, config->spImagesDirectory);
        strcat(imagePath, config->spImagesPrefix);
        sprintf(indexStr, "%d", index);
        strcat(imagePath, indexStr);
        strcat(imagePath, config->spImagesSuffix);
        return SP_CONFIG_SUCCESS;


    }
}
SP_KD_TREE_SPLIT_METHOD spConfigGetSplitMethod(const SPConfig config, SP_CONFIG_MSG* msg){
    assert(msg!=NULL);
    if(config==NULL){
        *msg=SP_CONFIG_INVALID_ARGUMENT;
        return INCREMENTAL;
    }else{
        *msg=SP_CONFIG_SUCCESS;
        return config->spKDTreeSplitMethod;
    }
}


bool spConfigGetMinimalGUI(const SPConfig config, SP_CONFIG_MSG* msg){
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return -1;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spMinimalGUI;
    }
}



SP_CONFIG_MSG spConfigGetPCAPath(char *pcaPath, const SPConfig config) {
    if (pcaPath == NULL || config == NULL) {
        return SP_CONFIG_INVALID_ARGUMENT;
    } else {
        strcat(pcaPath, config->spImagesDirectory);
        strcat(pcaPath, config->spPCAFilename);
        return SP_CONFIG_SUCCESS;
    }
}

void spConfigDestroy(SPConfig config) {
    if (config == NULL)
        return;
    FREE_MACRO(config->spImagesDirectory);
    FREE_MACRO(config->spImagesPrefix);
    FREE_MACRO(config->spImagesSuffix);
    FREE_MACRO(config->spPCAFilename);
    FREE_MACRO(config->spLoggerFilename);
    free(config);


}

SP_CONFIG_MSG getConfigMsg(int res) {
    switch (res) {
        case INVALID_ARGUMENT:
            return SP_CONFIG_INVALID_ARGUMENT;
            break;
        case CANNOT_OPEN_FILE:
            return SP_CONFIG_CANNOT_OPEN_FILE;
            break;
        case ALLOC_FAIL:
            return SP_CONFIG_ALLOC_FAIL;
            break;
        case INVALID_INTEGER:
            return SP_CONFIG_INVALID_INTEGER;
            break;
        case INVALID_STRING:
            return SP_CONFIG_INVALID_STRING;
            break;
        case MISSING_DIR:
            return SP_CONFIG_MISSING_DIR;
            break;
        case MISSING_SUFFIX:
            return SP_CONFIG_MISSING_SUFFIX;
            break;
        case MISSING_PREFIX:
            return SP_CONFIG_MISSING_PREFIX;
        case MISSING_NUM_IMAGES:
            return SP_CONFIG_MISSING_NUM_IMAGES;
            break;
        case LINE_INVALID:
            return SP_CONFIG_INVALID_LINE;
            break;
        case SUCCESS:
            return SP_CONFIG_SUCCESS;
            break;


    }

    return SP_CONFIG_SUCCESS;
}

int createDefaultValuesConfig(SPConfig config) {
    int res = SUCCESS;
    config->spPCADimension = 20;
    config->spNumOfFeatures = 100;
    config->spExtractionMode = true;
    config->spMinimalGUI = false;
    config->spNumOfSimilarImages = 1;
    config->spKNN = 1;
    config->spKDTreeSplitMethod = MAX_SPREAD;
    config->spLoggerLevel = 3;
    config->spImagesDirectory = NULL;
    config->spImagesPrefix = NULL;
    config->spImagesSuffix = NULL;
    config->spPCAFilename = NULL;
    config->spLoggerFilename = NULL;
    config->spNumOfImages = -10;
    MALLOC_MACRO(config->spImagesDirectory, char*, LINE_LENGTH * sizeof(char));
    MALLOC_MACRO(config->spImagesPrefix, char*, LINE_LENGTH * sizeof(char));
    MALLOC_MACRO(config->spImagesSuffix, char*, LINE_LENGTH * sizeof(char));
    MALLOC_MACRO(config->spPCAFilename, char*, LINE_LENGTH * sizeof(char));
    MALLOC_MACRO(config->spLoggerFilename, char*, LINE_LENGTH * sizeof(char));
    strcpy(config->spImagesDirectory, "NULL");
    strcpy(config->spImagesPrefix, "NULL");
    strcpy(config->spImagesSuffix, "NULL");
    strcpy(config->spPCAFilename, "pca.yml");
    strcpy(config->spLoggerFilename, "stdout");
    return res;

    fail:
    FREE_MACRO(config->spImagesDirectory);
    FREE_MACRO(config->spImagesPrefix);
    FREE_MACRO(config->spImagesSuffix);
    FREE_MACRO(config->spPCAFilename);
    FREE_MACRO(config->spLoggerFilename);
    return res;

}

int parseConfigFile(const char *file, SPConfig config, int *numberOfLines) {
    int res = SUCCESS;
    int lineNum = 0;
    FILE *configFile = fopen(file, "r");
    //configFile=;
    if (configFile == NULL) {
        return CANNOT_OPEN_FILE;
    }
    char *variable = NULL;
    char *val = NULL;
    char *str = NULL;
    MALLOC_MACRO(variable, char*, LINE_LENGTH);
    MALLOC_MACRO(val, char*, LINE_LENGTH);
    MALLOC_MACRO(str, char*, LINE_LENGTH);
    //read the file line by line and parse every line
    while (fgets(str, LINE_LENGTH, configFile) != NULL) {
        lineNum++;
        char *pointer;
        if (strchr(str, '#') != NULL) {// this line is a comment

            continue;//go to the next line
        }
        pointer = strchr(str, '=');
        if (pointer == NULL) {//there is no '='
            sscanf(str, "%s", str);//check if this line is empty;
            if (strlen(str) == 0 || *str == '\n') {
                continue;//go to the next line
            } else {
                res = LINE_INVALID;
                printInvalidLineErrorMsg(file, lineNum);
                goto fail;
                break;
            }
        } else {//valid line
            variable = str;
            val = pointer + 1;
            *pointer = '\0';
            FUNC_MACRO(removeSpaceAndCheckValid(&variable));
            FUNC_MACRO(removeSpaceAndCheckValid(&val));
            FUNC_MACRO(setValuesConfig(variable, val, lineNum, file, config));
        }
    }
    *numberOfLines = lineNum;
    return SUCCESS;
    fail:
    FREE_MACRO(variable);
    if (res == LINE_INVALID)
        printInvalidLineErrorMsg(file, lineNum);
    //  FREE_MACRO(val);
    // FREE_MACRO(str);
    return res;
}

int setValuesConfig(char *variable, char *val, int lineNum, const char *file, SPConfig config) {
    if (strcmp(variable, "spImagesDirectory") == 0) {
        strcpy(config->spImagesDirectory, val);
        return SUCCESS;
    }
    if (strcmp(variable, "spImagesPrefix") == 0) {
        strcpy(config->spImagesPrefix, val);
        return SUCCESS;
    }
    if (strcmp(variable, "spImagesSuffix") == 0) {
        if (strcmp(val, ".png") != 0 && strcmp(val, ".jpg") != 0 && strcmp(val, ".bmp") != 0 &&
            strcmp(val, ".gif") != 0) {//constraints doesn't met
            printConstrainsMessage(file, lineNum);
            return INVALID_STRING;
        } else {//everything is ok
            strcpy(config->spImagesSuffix, val);
            return SUCCESS;
        }
    }
    if (strcmp(variable, "spNumOfImages") == 0) {
        if (atoi(val) > 0) {
            config->spNumOfImages = atoi(val);
            return SUCCESS;
        } else {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }
    }
    if (strcmp(variable, "spPCADimension") == 0) {
        if (atoi(val) >= 10 && atoi(val) <= 28) {
            config->spPCADimension = atoi(val);
            return SUCCESS;
        } else {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }

    }
    if (strcmp(variable, "spPCAFilename") == 0) {
        strcpy(config->spPCAFilename, val);
        return SUCCESS;
    }
    if (strcmp(variable, "spNumOfFeatures") == 0) {
        if (atoi(val) > 0) {
            config->spNumOfFeatures = atoi(val);
            return SUCCESS;
        } else {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }
    }
    if (strcmp(variable, "spExtractionMode") == 0) {
        if (strcmp(val, "true") == 0) {
            config->spExtractionMode = true;
            return SUCCESS;
        }
        if (strcmp(val, "false") == 0) {
            config->spExtractionMode = false;
            return SUCCESS;
        }
        else {
            printConstrainsMessage(file, lineNum);
            return INVALID_STRING;
        }
    }
    if (strcmp(variable, "spNumOfSimilarImages") == 0) {
        if (atoi(val) > 0) {
            config->spNumOfSimilarImages = atoi(val);
            return SUCCESS;
        } else {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }
    }
    if (strcmp(variable, "spKDTreeSplitMethod") == 0) {
        if (strcmp(val, "RANDOM") == 0) {
            config->spKDTreeSplitMethod = RANDOM;
            return SUCCESS;
        }
        if (strcmp(val, "MAX_SPREAD") == 0) {
            config->spKDTreeSplitMethod = MAX_SPREAD;
            return SUCCESS;
        }
        if (strcmp(val, "INCREMENTAL") == 0) {
            config->spKDTreeSplitMethod = INCREMENTAL;
            return SUCCESS;
        }
        else {
            printConstrainsMessage(file, lineNum);
            return INVALID_STRING;
        }
    }
    if (strcmp(variable, "spKNN") == 0) {
        if (atoi(val) > 0) {
            config->spKNN = atoi(val);
            return SUCCESS;
        }
        else {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }
    }
    if (strcmp(variable, "spMinimalGUI") == 0) {
        if (strcmp(val, "true") == 0) {
            config->spMinimalGUI = true;
            return SUCCESS;
        }
        if (strcmp(val, "false") == 0) {
            config->spMinimalGUI = false;
            return SUCCESS;
        }
        else {
            printConstrainsMessage(file, lineNum);
            return INVALID_STRING;
        }
    }
    if (strcmp(variable, "spLoggerLevel") == 0) {
        if (strlen(val) != 1) {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }
        switch (atoi(val)) {
            case 1:
                config->spLoggerLevel = 1;
                return SUCCESS;
                break;
            case 2:
                config->spLoggerLevel = 2;
                return SUCCESS;
                break;
            case 3:
                config->spLoggerLevel = 3;
                return SUCCESS;
                break;
            case 4:
                config->spLoggerLevel = 4;
                return SUCCESS;
                break;
            default:
                printConstrainsMessage(file, lineNum);
                return INVALID_INTEGER;
        }
    }
    if (strcmp(variable, "spLoggerFilename") == 0) {
        strcpy(config->spLoggerFilename, val);
        return SUCCESS;
    }
    return LINE_INVALID;
}

int isDefaultValuesSet(SPConfig config, const char *file, int numberOfLines) {
    if (strcmp(config->spImagesDirectory, "NULL") == 0) {
        parameterNotSetMessage(file, numberOfLines, "spImagesDirectory");
        return MISSING_DIR;
    }
    if (strcmp(config->spImagesPrefix, "NULL") == 0) {
        parameterNotSetMessage(file, numberOfLines, "spImagesPrefix");
        return MISSING_PREFIX;
    }
    if (strcmp(config->spImagesSuffix, "NULL") == 0) {
        parameterNotSetMessage(file, numberOfLines, "spImagesSuffix");
        return MISSING_SUFFIX;
    }
    if (config->spNumOfImages == -10) {
        parameterNotSetMessage(file, numberOfLines, "spNumOfImages");
        return MISSING_NUM_IMAGES;
    }
    return SUCCESS;
}

int removeSpaceAndCheckValid(char **line) {
    char temp[1024];
    strcpy(temp, *line);
    int charIndex = 0;
    int index = 0;
    bool firstChar = false;
    bool lastChar = false;

    while (temp[charIndex] != '\0' && temp[charIndex] != '\n' && temp[charIndex] != '\r' ) {
        if (!firstChar && temp[charIndex] == ' ') {
            charIndex++;
            continue;
        }
        if (!firstChar && temp[charIndex] != ' ') {
            firstChar = true;
            (*line)[index] = temp[charIndex];
            charIndex++;
            index++;
            continue;

        }
        if (firstChar && !lastChar && temp[charIndex] != ' ') {
            (*line)[index] = temp[charIndex];
            charIndex++;
            index++;
            continue;
        }
        if (firstChar && !lastChar && temp[charIndex] == ' ') {
            lastChar = true;
            charIndex++;
            continue;

        }
        if (firstChar && lastChar && temp[charIndex] == ' ') {
            charIndex++;
            continue;
        }
        if (firstChar && lastChar && temp[charIndex] != ' ') {
            return LINE_INVALID;
            break;
        }
    }
    (*line)[index] = '\0';
    return SUCCESS;
}












