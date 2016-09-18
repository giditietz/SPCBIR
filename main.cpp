#include <cstdio>
#include <cstdlib>
#include "SPImageProc.h"
#include "Malloc_Macro.h"

#define DEFAULT_FILE_NAME "spcbir.config"
#define C "-c"
#define COMMAND_LINE_ERROR "Invalid command line : use -c"
#define CONFIG_FILENAME "<config_filename>"
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define MAX_LEN 1024
#define FORMAT "%s %s\n"
#define LOGGER_INIT "Logger is initialized"
#define IMAGE_PROC_INIT "spImageProc Object is initialized"
#define IMAGE_FEATURE_FAIL "Failed to get image features"
#define FAIL_WRITE_FEAT "Failed to write features"
#define FEAT_EXTRACT "Features were extracted from %d images"
#define SUCCESS_FEAT "Success: Features extracted"
#define RETRIEVE_DATA "Retrieve data from feats files"
#define TOTAL_NUM_FEAT "Total number of features is %d"
#define KDARRAY_INIT_FAIL "KDArray init failed due to a memory allocation error."
#define ENTER_PATH "\nPlease enter an image path:\n"
#define STRING_FORMAT "%s"
#define STRING_FORMATN "%s\n"
#define EXIT_FORMAT "<>"
#define EXIT_MESSAGE "Exiting...\n"
#define CANT_GET_FEAT "Cannot get image features"
#define ALLOC_ERROR "Memory allocation error"
#define CANDIDATED_MSG "Best candidates for - %s - are:\n"
extern "C" {
#include "SPKDArray.h"
#include "SPKDTree.h"
#include "SPFinalImageList.h"
#include "Functions.h"


}

int main(int argc, char *argv[]) {
    //Decelerations & Inits
    SP_CONFIG_MSG msg;
    SPConfig config = NULL;
    bool created = false;
    int res = SP_CONFIG_SUCCESS;
    int *featuresNum = NULL;
    SPPoint **arrImageFeatures = NULL;
    int imageNum;
    SPPoint *totalImageFeaturesArr = NULL;
    SP_KD_TREE_SPLIT_METHOD method;
    bool extraction;
    int temp = 0;
    int sum = 0;
    char imagePath[MAX_LEN];
    bool proceed = true;
    SPPoint *queryPointArray = NULL;
    int numOfFeatsQueryImage[ONE]; //an int[] to comply to imgproc, has a single entry.
    int indexOfQueryImage = INT_MAX;
    int *finalImageIndexes = NULL;
    SPKDNode root = NULL;
    SPKDArray kdarray = NULL;
    bool minimalGUI;
    int numberOfSimilarImages;
    char logger[MAX_LEN];
    char loggerMessage[MAX_LEN];
    SP_LOGGER_LEVEL level;
    sp::ImageProc *imageProcObject;


    //Create SPConfig
    if (argc > THREE) {
        printf(FORMAT, COMMAND_LINE_ERROR, argv[TWO]);
        goto fail;
    }
    if (argc == ONE) {
        config = spConfigCreate(DEFAULT_FILE_NAME, &msg);
        if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {//no arguments were entered by user and the default file cannot be open
            printCannotOpenFileNoArguments();
            fflush(NULL);
            goto fail;
        }
    } else if (argc == TWO && strcmp(argv[ONE], C) == ZERO) {
        printf(FORMAT, COMMAND_LINE_ERROR, CONFIG_FILENAME);
        fflush(NULL);
        goto fail;
    } else if (argc == THREE && strcmp(argv[ONE], C) == ZERO) {
        config = spConfigCreate(argv[TWO], &msg);
        if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {//user entered arguments but the file cannot be open
            printCannotOpenFile(argv[TWO]);
            fflush(NULL);
            goto fail;
        }
        if (!config) {
            goto fail;
        }

    } else {// user didn't entered arguments correctly
        printf(FORMAT, COMMAND_LINE_ERROR, CONFIG_FILENAME);//if the user entered arguments not correctly
        goto fail;
    }
    //Create Logger
    level = spConfigGetLoggerLevel(config, &msg);
    spConfigGetLoggerName(config, logger);
    if (spLoggerCreate(NULL, level) != SP_LOGGER_SUCCESS) {
        goto fail;
    } else {
        spLoggerPrintInfo(LOGGER_INIT);
    }
    extraction = spConfigIsExtractionMode(config, &msg);
    imageProcObject = new sp::ImageProc(config);
    if(imageProcObject==NULL){
        goto fail;
    }
    if (msg != SP_CONFIG_SUCCESS) {
        goto fail;
    }
    spLoggerPrintInfo(IMAGE_PROC_INIT);

    imageNum = spConfigGetNumOfImages(config, &msg);
    if (msg != SP_CONFIG_SUCCESS) {
        goto fail;
    }
    method = spConfigGetSplitMethod(config, &msg);
    if (msg != SP_CONFIG_SUCCESS) {
        goto fail;
    }

    MALLOC_MACRO(arrImageFeatures, SPPoint**, imageNum * sizeof(SPPoint *));
    MALLOC_MACRO(featuresNum, int*, imageNum * sizeof(int));
    //extraction mode
    for (int i = 0; i < imageNum; i++) {
        arrImageFeatures[i] = NULL;
    }
    if (extraction) {
        for (temp = 0; temp < imageNum; temp++) {
            msg = spConfigGetImagePath(imagePath, config, temp);
            if (msg != SP_CONFIG_SUCCESS) {
                goto fail;
            }
            arrImageFeatures[temp] = imageProcObject->getImageFeatures(imagePath, temp, &(featuresNum[temp]));
            if (NULL == arrImageFeatures[temp]) {
                spLoggerPrintError(IMAGE_FEATURE_FAIL, __FILE__, __func__, __LINE__);
                goto fail;
            }
        }


        for (int i = 0; i < imageNum; i++) {
            res = writeFeatures(config, i, featuresNum[i], arrImageFeatures[i]);
            if (res != SP_CONFIG_SUCCESS) {
                spLoggerPrintError(FAIL_WRITE_FEAT, __FILE__, __func__, __LINE__);
                goto fail;
            }
        }
        sprintf(loggerMessage,FEAT_EXTRACT, imageNum);
        spLoggerPrintDebug(loggerMessage, __FILE__, __func__, __LINE__);
        spLoggerPrintInfo(SUCCESS_FEAT);

    } else { //Non-extraction mode
        spLoggerPrintInfo(RETRIEVE_DATA);
        for (int j = 0; j < imageNum; j++) {

            res = readFeatures(config, j, &(featuresNum[j]), &(arrImageFeatures[j]), created);
            if(res!=SP_CONFIG_SUCCESS){
                goto fail;
            }
            created = true;

        }
    }
    //end of extraction/non extraction mode
    sum = sumAllFeatures(featuresNum, imageNum);//get total number of features
    sprintf(loggerMessage,TOTAL_NUM_FEAT , sum);
    spLoggerPrintDebug(loggerMessage, __FILE__, __func__, __LINE__);
    createAllImagesPointsArr(&totalImageFeaturesArr, arrImageFeatures, imageNum, sum, featuresNum);

    free(arrImageFeatures);
    //Init the data structures:
    kdarray = spKDArrayInit(totalImageFeaturesArr, sum);
    if (NULL == kdarray) {
        spLoggerPrintError(KDARRAY_INIT_FAIL, __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
    root = spKDTreeInit(kdarray, method);
    //query
    while (proceed) {
        char queryImagePath[MAX_LEN];
        puts(ENTER_PATH);
        scanf(STRING_FORMAT, queryImagePath);
        fflush(NULL);
        if (0 == strcmp(EXIT_FORMAT, queryImagePath)) {
            puts(EXIT_MESSAGE);
            goto fail; //Program is terminated, so we move to the cleanup phase.
        }
        if (proceed) {
            //TODO what happens if ENTER is pressed?
            queryPointArray = imageProcObject->getImageFeatures(queryImagePath, indexOfQueryImage,
                                                                numOfFeatsQueryImage);
            if (NULL == queryPointArray) {
                spLoggerPrintError(CANT_GET_FEAT, __FILE__, __FUNCTION__, __LINE__);
                goto fail;
            }
            numberOfSimilarImages = spConfigGetspNumOfSimilarImages(config, &msg);
            finalImageIndexes = (int *) calloc((size_t) numberOfSimilarImages, sizeof(int));
            if (NULL == finalImageIndexes) {
                spLoggerPrintError(ALLOC_ERROR, __FILE__, __FUNCTION__, __LINE__);
                goto fail;
            }


            FUNC_MACRO(spGetFinalImageList(config, root, finalImageIndexes, queryPointArray, numOfFeatsQueryImage[0]));
            minimalGUI = spConfigGetMinimalGUI(config, &msg);

            //Two cases: MinimalGUI or Non-MinimalGUI
            char resultPath[MAX_LEN];
            if (!minimalGUI) { printf(CANDIDATED_MSG, queryImagePath); }

            for (int i = 0; i < numberOfSimilarImages; i++) {
                int indexOfImageToShow = finalImageIndexes[i];
                spConfigGetImagePath(resultPath, config, indexOfImageToShow); //FUNC_MALLOC
                //two cases: MinimalGUI of Non-MinimalGUI
                if (minimalGUI) {
                    imageProcObject->showImage(resultPath);
                }
                else {
                    printf(STRING_FORMATN, resultPath);
                }
            }
        }
    }

    fail:
    FREE_MACRO(finalImageIndexes);
    FREE_MACRO(queryPointArray);
    FREE_MACRO(featuresNum);
    spKDTreeDestroy(root);
    FREE_MACRO(root);//check
    if (NULL != config) {
        spConfigDestroy(config);
    } //TODO: if != NULL
    spLoggerDestroy();
    return 0;
}




