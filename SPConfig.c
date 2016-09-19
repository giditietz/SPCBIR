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

#define MAX_LEN 1024
#define FEATS ".feats"
#define INT_FORMAT "%d"
#define STR_FORMAT "%s"
#define NEGETIVE -1
#define NEGETIVETEN -10
#define EMPTY_STRING ""
#define PCA_DEF_VAL 20
#define NUM_FEAT_DEF 100
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define NULL_STR "NULL"
#define PCA_DEF "pca.yml"
#define STDOUT "stdout"
#define READ "r"
#define HASH_TAG '#'
#define EQUALS_STR '='
#define END_LINEN '\n'
#define END_LINER '\r'
#define END_STR '\0'
#define IMG_DIR "spImagesDirectory"
#define IMG_PREFIX "spImagesPrefix"
#define IMG_SUFFIX "spImagesSuffix"
#define PNG ".png"
#define JPG ".jpg"
#define BMP ".bmp"
#define GIF ".gif"
#define IMAGE_NUM "spNumOfImages"
#define PCA_DIM "spPCADimension"
#define PCA_LOWER_BOUND 10
#define PCA_UPPER_BOUND 28
#define PCA_FILE "spPCAFilename"
#define NUM_FEAT "spNumOfFeatures"
#define EXTRACT_MODE "spExtractionMode"
#define TRUE "true"
#define FALSE "false"
#define NUM_SIMILAR_IMAGES "spNumOfSimilarImages"
#define KD_SPLIT_METHOD_STR "spKDTreeSplitMethod"
#define RANDOM_STR "RANDOM"
#define INCREMENTAL_STR "INCREMENTAL"
#define MAX_SPREAD_STR "MAX_SPREAD"
#define SPKNN_STR "spKNN"
#define MIN_GUI_STR "spMinimalGUI"
#define LOGGER_LEV_STR "spLoggerLevel"
#define LOGGER_FILE_STR "spLoggerFilename"
#define SPACE_CHAR ' '


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
    int spPCADimension;//default
    char *spPCAFilename;//default=pca.yml
    int spNumOfFeatures;//default
    bool spExtractionMode;//default=true
    int spNumOfSimilarImages;//default
    SP_KD_TREE_SPLIT_METHOD spKDTreeSplitMethod;//default=MAX_SPREAD
    int spKNN;//default
    bool spMinimalGUI;//default=false
    int spLoggerLevel;//default
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

SP_LOGGER_LEVEL spConfigGetLoggerLevel(const SPConfig config, SP_CONFIG_MSG *msg) {
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return NEGETIVE;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spLoggerLevel;
    }


}

void spConfigGetLoggerName(const SPConfig config, char *name) {
    if (config == NULL) {
        return;
    } else {
        strcpy(name, config->spLoggerFilename);
    }
}


int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return NEGETIVE;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spNumOfImages;
    }
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return NEGETIVE;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spNumOfFeatures;
    }
}

int spConfigGetSPKnn(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return NEGETIVE;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spKNN;
    }
}

int spConfigGetspNumOfSimilarImages(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return NEGETIVE;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spNumOfSimilarImages;
    }
}

SP_CONFIG_MSG spConfigGetFeaturesPathFeats(char *imagePath, const SPConfig config,
                                           int index) {
    char indexStr[MAX_LEN];
    if (imagePath == NULL || config == NULL) {
        return SP_CONFIG_INVALID_ARGUMENT;
    }
    if (index >= config->spNumOfImages) {
        return SP_CONFIG_INDEX_OUT_OF_RANGE;
    }
    else {
        strcat(imagePath, config->spImagesDirectory);
        strcat(imagePath, config->spImagesPrefix);
        sprintf(indexStr, INT_FORMAT, index);
        strcat(imagePath, indexStr);
        strcat(imagePath, FEATS);
        return SP_CONFIG_SUCCESS;


    }
}

int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return NEGETIVE;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spPCADimension;
    }
}

SP_CONFIG_MSG spConfigGetImagePath(char *imagePath, const SPConfig config,
                                   int index) {
    char indexStr[MAX_LEN];
    if (imagePath == NULL || config == NULL) {
        return SP_CONFIG_INVALID_ARGUMENT;
    }
    if (index >= config->spNumOfImages) {
        return SP_CONFIG_INDEX_OUT_OF_RANGE;
    }
    else {
        strcpy(imagePath, EMPTY_STRING); //it was an error
        strcat(imagePath, config->spImagesDirectory);
        strcat(imagePath, config->spImagesPrefix);
        sprintf(indexStr, INT_FORMAT, index);
        strcat(imagePath, indexStr);
        strcat(imagePath, config->spImagesSuffix);
        return SP_CONFIG_SUCCESS;


    }
}

SP_KD_TREE_SPLIT_METHOD spConfigGetSplitMethod(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return INCREMENTAL;
    } else {
        *msg = SP_CONFIG_SUCCESS;
        return config->spKDTreeSplitMethod;
    }
}


bool spConfigGetMinimalGUI(const SPConfig config, SP_CONFIG_MSG *msg) {
    assert(msg != NULL);
    if (config == NULL) {
        *msg = SP_CONFIG_INVALID_ARGUMENT;
        return NEGETIVE;
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
    config->spPCADimension = PCA_DEF_VAL;
    config->spNumOfFeatures = NUM_FEAT_DEF;
    config->spExtractionMode = true;
    config->spMinimalGUI = false;
    config->spNumOfSimilarImages = ONE;
    config->spKNN = ONE;
    config->spKDTreeSplitMethod = MAX_SPREAD;
    config->spLoggerLevel = THREE;
    config->spImagesDirectory = NULL;
    config->spImagesPrefix = NULL;
    config->spImagesSuffix = NULL;
    config->spPCAFilename = NULL;
    config->spLoggerFilename = NULL;
    config->spNumOfImages = NEGETIVETEN;
    MALLOC_MACRO(config->spImagesDirectory, char*, MAX_LEN * sizeof(char));
    MALLOC_MACRO(config->spImagesPrefix, char*, MAX_LEN * sizeof(char));
    MALLOC_MACRO(config->spImagesSuffix, char*, MAX_LEN * sizeof(char));
    MALLOC_MACRO(config->spPCAFilename, char*, MAX_LEN * sizeof(char));
    MALLOC_MACRO(config->spLoggerFilename, char*, MAX_LEN * sizeof(char));
    strcpy(config->spImagesDirectory, NULL_STR);
    strcpy(config->spImagesPrefix, NULL_STR);
    strcpy(config->spImagesSuffix, NULL_STR);
    strcpy(config->spPCAFilename, PCA_DEF);
    strcpy(config->spLoggerFilename, STDOUT);
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
    int res = SP_CONFIG_SUCCESS;
    int lineNum = 0;
    FILE *configFile = fopen(file, READ);
    //configFile=;
    if (configFile == NULL) {
        return CANNOT_OPEN_FILE;
    }
   // char *variable = NULL;
  //  char *val = NULL;
    //char *str = NULL;
    char str[MAX_LEN];
    char* variable=NULL;
    char* val=NULL;
    //MALLOC_MACRO(variable, char*, MAX_LEN);
    //MALLOC_MACRO(val, char*, MAX_LEN);
    //MALLOC_MACRO(str, char*, MAX_LEN);
    //read the file line by line and parse every line
    while (fgets(str, MAX_LEN, configFile) != NULL) {
        lineNum++;
        char *pointer;
        if (strchr(str, HASH_TAG) != NULL) {// this line is a comment

            continue;//go to the next line
        }
        pointer = strchr(str, EQUALS_STR);
        if (pointer == NULL) {//there is no '='
            sscanf(str, STR_FORMAT, str);//check if this line is empty;
            if (strlen(str) == ZERO || str[ZERO] == END_LINEN || str[ZERO] == END_LINER) {
                continue;//go to the next line
            } else {
                res = LINE_INVALID;
                //printInvalidLineErrorMsg(file, lineNum);
                goto fail;
                break;
            }
        } else {//valid line
            variable = str;
            val = pointer + 1;
            *pointer = END_STR;
            FUNC_MACRO(removeSpaceAndCheckValid(&variable));
            FUNC_MACRO(removeSpaceAndCheckValid(&val));
            FUNC_MACRO(setValuesConfig(variable, val, lineNum, file, config));
        }
    }
    *numberOfLines = lineNum;
   // free(str);
   // free(val);
    //free(variable);
    //FREE_MACRO(str);
    //FREE_MACRO(val);

    //FREE_MACRO(variable);
    return SUCCESS;
    fail:
   // FREE_MACRO(variable);
    if (res == LINE_INVALID)
        printInvalidLineErrorMsg(file, lineNum);
    //  FREE_MACRO(val);
    // FREE_MACRO(str);
    return res;
}

int setValuesConfig(char *variable, char *val, int lineNum, const char *file, SPConfig config) {
    if (strcmp(variable, IMG_DIR) == ZERO) {
        strcpy(config->spImagesDirectory, val);
        return SUCCESS;
    }
    if (strcmp(variable, IMG_PREFIX) == ZERO) {
        strcpy(config->spImagesPrefix, val);
        return SUCCESS;
    }
    if (strcmp(variable, IMG_SUFFIX) == ZERO) {
        if (strcmp(val, PNG) != ZERO && strcmp(val, JPG) != 0 && strcmp(val, BMP) != ZERO &&
            strcmp(val, GIF) != ZERO) {//constraints doesn't met
            printConstrainsMessage(file, lineNum);
            return INVALID_STRING;
        } else {//everything is ok
            strcpy(config->spImagesSuffix, val);
            return SUCCESS;
        }
    }
    if (strcmp(variable, IMAGE_NUM) == ZERO) {
        if (atoi(val) > ZERO) {
            config->spNumOfImages = atoi(val);
            return SUCCESS;
        } else {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }
    }
    if (strcmp(variable, PCA_DIM) == ZERO) {
        if (atoi(val) >= PCA_LOWER_BOUND && atoi(val) <= PCA_UPPER_BOUND) {
            config->spPCADimension = atoi(val);
            return SUCCESS;
        } else {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }

    }
    if (strcmp(variable,PCA_FILE ) == ZERO) {
        strcpy(config->spPCAFilename, val);
        return SUCCESS;
    }
    if (strcmp(variable, NUM_FEAT) == ZERO) {
        if (atoi(val) > ZERO) {
            config->spNumOfFeatures = atoi(val);
            return SUCCESS;
        } else {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }
    }
    if (strcmp(variable, EXTRACT_MODE) == ZERO) {
        if (strcmp(val, TRUE) == ZERO) {
            config->spExtractionMode = true;
            return SUCCESS;
        }
        if (strcmp(val, FALSE) == ZERO) {
            config->spExtractionMode = false;
            return SUCCESS;
        }
        else {
            printConstrainsMessage(file, lineNum);
            return INVALID_STRING;
        }
    }
    if (strcmp(variable,NUM_SIMILAR_IMAGES ) == ZERO) {
        if (atoi(val) > ZERO) {
            config->spNumOfSimilarImages = atoi(val);
            return SUCCESS;
        } else {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }
    }
    if (strcmp(variable,KD_SPLIT_METHOD_STR ) == ZERO) {
        if (strcmp(val, RANDOM_STR) == ZERO) {
            config->spKDTreeSplitMethod = RANDOM;
            return SUCCESS;
        }
        if (strcmp(val, MAX_SPREAD_STR) == ZERO) {
            config->spKDTreeSplitMethod = MAX_SPREAD;
            return SUCCESS;
        }
        if(strcmp(val,INCREMENTAL_STR)==ZERO){
            config->spKDTreeSplitMethod = INCREMENTAL;
            return SUCCESS;
        }
        else {
            printConstrainsMessage(file, lineNum);
            return INVALID_STRING;
        }
    }
    if (strcmp(variable, SPKNN_STR) == ZERO) {
        if (atoi(val) > ZERO) {
            config->spKNN = atoi(val);
            return SUCCESS;
        }
        else {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }
    }
    if (strcmp(variable,MIN_GUI_STR) == ZERO) {
        if (strcmp(val, TRUE) == ZERO) {
            config->spMinimalGUI = true;
            return SUCCESS;
        }
        if (strcmp(val, FALSE) == ZERO) {
            config->spMinimalGUI = false;
            return SUCCESS;
        }
        else {
            printConstrainsMessage(file, lineNum);
            return INVALID_STRING;
        }
    }
    if (strcmp(variable, LOGGER_LEV_STR) == ZERO) {
        if (strlen(val) != ONE) {
            printConstrainsMessage(file, lineNum);
            return INVALID_INTEGER;
        }
        switch (atoi(val)) {
            case 1:
                config->spLoggerLevel = ONE;
                return SUCCESS;
                break;
            case 2:
                config->spLoggerLevel = TWO;
                return SUCCESS;
                break;
            case 3:
                config->spLoggerLevel = THREE;
                return SUCCESS;
                break;
            case 4:
                config->spLoggerLevel = FOUR;
                return SUCCESS;
                break;
            default:
                printConstrainsMessage(file, lineNum);
                return INVALID_INTEGER;
        }
    }
    if (strcmp(variable,LOGGER_FILE_STR ) == ZERO) {
        strcpy(config->spLoggerFilename, val);
        return SUCCESS;
    }
    return LINE_INVALID;
}

int isDefaultValuesSet(SPConfig config, const char *file, int numberOfLines) {
    if (strcmp(config->spImagesDirectory, NULL_STR) == ZERO) {
        parameterNotSetMessage(file, numberOfLines, IMG_DIR);
        return MISSING_DIR;
    }
    if (strcmp(config->spImagesPrefix, NULL_STR) == ZERO) {
        parameterNotSetMessage(file, numberOfLines, IMG_PREFIX);
        return MISSING_PREFIX;
    }
    if (strcmp(config->spImagesSuffix, NULL_STR) == ZERO) {
        parameterNotSetMessage(file, numberOfLines, IMG_SUFFIX);
        return MISSING_SUFFIX;
    }
    if (config->spNumOfImages == NEGETIVETEN) {
        parameterNotSetMessage(file, numberOfLines, IMAGE_NUM);
        return MISSING_NUM_IMAGES;
    }
    return SUCCESS;
}

int removeSpaceAndCheckValid(char **line) {
    char temp[MAX_LEN];
    strcpy(temp, *line);
    int charIndex = ZERO;
    int index = ZERO;
    bool firstChar = false;
    bool lastChar = false;

    while (temp[charIndex] != END_STR && temp[charIndex] != END_LINEN && temp[charIndex] != END_LINER) {
        if (!firstChar && temp[charIndex] == SPACE_CHAR) {
            charIndex++;
            continue;
        }
        if (!firstChar && temp[charIndex] != SPACE_CHAR) {
            firstChar = true;
            (*line)[index] = temp[charIndex];
            charIndex++;
            index++;
            continue;

        }
        if (firstChar && !lastChar && temp[charIndex] != SPACE_CHAR) {
            (*line)[index] = temp[charIndex];
            charIndex++;
            index++;
            continue;
        }
        if (firstChar && !lastChar && temp[charIndex] == SPACE_CHAR) {
            lastChar = true;
            charIndex++;
            continue;

        }
        if (firstChar && lastChar && temp[charIndex] == SPACE_CHAR) {
            charIndex++;
            continue;
        }
        if (firstChar && lastChar && temp[charIndex] != SPACE_CHAR) {
            return LINE_INVALID;
            break;
        }
    }
    (*line)[index] = END_STR;
    return SUCCESS;
}












