#include <cstdio>
#include <cstdlib>
#include "SPImageProc.h"
#include "Malloc_Macro.h"

#define DEFAULT_FILE_NAME "spcbir.config"
#define C "-c"
#define COMMAND_LINE_ERROR "Invalid command line : use -c"


#define MAX_LEN 1024

extern "C" {
#include "SPPoint.h"
#include "SPKDArray.h"
#include "SPKDTree.h"
#include "SPLogger.h"
#include "SPFinalImageList.h"
#include "SPConfig.h"
#include "SPKDTree.h"
#include <limits.h>
#include "KDTreeTest.h"
#include "SPPoint.h"
#include "Functions.h"


}

int main(int argc, char *argv[]) {
    //TODO MUST CREATE AN SPLOGGER STRUCT!!!!!
    spLoggerCreate(NULL, SP_LOGGER_INFO_WARNING_ERROR_LEVEL); //TODO TEMPORARY!!
    //Decelerations & Inits
    SP_CONFIG_MSG msg;
    SPConfig config = NULL;
    bool created = false;
    int res = SP_CONFIG_SUCCESS;
    int *featuresNum = NULL;
    SPPoint **arrImageFeatures = NULL;
    int imageNum;
    int totalFeatures;
    SPPoint *totalImageFeaturesArr = NULL;
    SP_KD_TREE_SPLIT_METHOD method;
    bool extraction;
    int temp = 0;
    int sum = 0;
    char imagePath[MAX_LEN];
    bool proceed = true;
    SPPoint *queryPointArray;
    int numOfFeatsQueryImage;
    int totalNumberOfFeatures;
    int indexOfQueryImage = INT_MAX;
    int *finalImageIndexes = NULL;
    SPKDNode root = NULL;
    SPKDArray kdarray = NULL;
    bool minimalGUI;
    int numberOfSimilarImages;

    //Create SPConfig
    if (argc == 1) {
        config = spConfigCreate(DEFAULT_FILE_NAME, &msg);
        if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {//no arguments were entered by user and the default file cannot be open
            printCannotOpenFileNoArguments();
        }
    } else if (strcmp(argv[1], C) == 0) {
        config = spConfigCreate(argv[2], &msg);
        if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {//user entered arguments but the file cannot be open
            printCannotOpenFile(argv[2]);
        }
    } else {// user didn't entered arguments correctly
        printf("%s %s\n", COMMAND_LINE_ERROR, argv[2]);//if the user entered arguments not correctly

    }
    extraction = spConfigIsExtractionMode(config, &msg);
    sp::ImageProc *imageProcObject = new sp::ImageProc(config);
    if (msg != SP_CONFIG_SUCCESS) {
        goto fail;
    }
    imageNum = spConfigGetNumOfImages(config, &msg);
    if (msg != SP_CONFIG_SUCCESS) {
        goto fail;
    }
    method = spConfigGetSplitMethod(config, &msg);
    if (msg != SP_CONFIG_SUCCESS) {
        goto fail;
    }

    MALLOC_MACRO(arrImageFeatures, SPPoint**, imageNum * sizeof(SPPoint *));
    // MALLOC_MACRO(arrImageFeatures, SPPoint**, imageNum * sizeof(SPPoint *));
    MALLOC_MACRO(featuresNum, int*, imageNum * sizeof(int));
    //extraction mode



    if (extraction) {
        // imagePath="\0";
        for (temp = 0; temp < imageNum; temp++) {
            // Get image path
            msg = spConfigGetImagePath(imagePath, config, temp);
            if (msg != SP_CONFIG_SUCCESS) {
                goto fail;
            }

            arrImageFeatures[temp] = imageProcObject->getImageFeatures(imagePath, temp, &(featuresNum[temp]));
            if (NULL == arrImageFeatures[temp]) {
                goto fail;
            }
        }
        for (int i = 0; i < imageNum; i++) {
            printf("\n%d", featuresNum[i]);
        }

        for (int i = 0; i < imageNum; i++) {
            res = writeFeatures(config, i, featuresNum[i], arrImageFeatures[i]);
            if (res != SP_CONFIG_SUCCESS) {
                goto fail;
            }
        }

    } else {
        for (int j = 0; j < imageNum; j++) {
            readFeatures(config, j, &featuresNum[j], &arrImageFeatures[j], created, imageNum);
            created = true;
        }
    }
    //end of extraction/non extraction mode
    sum = sumAllFeatures(featuresNum, imageNum);//get total number of features
    createAllImagesPointsArr(&totalImageFeaturesArr, arrImageFeatures, imageNum, sum, featuresNum);

    //Init the data structures:
    kdarray = spKDArrayInit(totalImageFeaturesArr, sum);
    root = spKDTreeInit(kdarray, method);
    //test(config); //temporary test for kdtree, only works with conf-gidi.txt (change the images path)

    //query

    while (proceed) {
        char queryImagePath[MAX_LEN];
        puts("\nPlease enter an image path:\n");
        //fgets(queryImagePath, MAX_LEN, stdin); //user input
        scanf("%s", queryImagePath);
        fflush(NULL);
        if (0 == strcmp("<>", queryImagePath)) {
            proceed = false;
            puts("Exiting...\n");
            goto fail; //Program is terminated, so we move to the cleanup phase.
        }
        if (proceed) {

            queryPointArray = imageProcObject->getImageFeatures(queryImagePath, indexOfQueryImage,
                                                                &numOfFeatsQueryImage);
            if (NULL == queryPointArray) {
                spLoggerPrintError("Cannot get image features", __FILE__, __FUNCTION__, __LINE__);
                goto fail;
            }
            numberOfSimilarImages = spConfigGetspNumOfSimilarImages(config, &msg);
            finalImageIndexes = (int *) calloc((size_t) numberOfSimilarImages, sizeof(int));
            if (NULL == finalImageIndexes) {
                spLoggerPrintError("Memory allocation error", __FILE__, __FUNCTION__, __LINE__);
                goto fail;
            }


            FUNC_MACRO(spGetFinalImageList(config, root, finalImageIndexes, queryPointArray));
            minimalGUI = spConfigGetMinimalGUI(config, &msg); //TODO SAFE_METHOD

            //Two cases: MinimalGUI or Non-MinimalGUI

            char resultPath[MAX_LEN];
            if (!minimalGUI) { printf("Best candidates for - %s - are:\n", queryImagePath); }

            for (int i = 0; i < numberOfSimilarImages; i++) {
                int indexOfImageToShow = finalImageIndexes[i];
                FUNC_MACRO(spConfigGetImagePath(resultPath, config, indexOfImageToShow));
                //two cases: MinimalGUI of Non-MinimalGUI
                if (minimalGUI) {
                    imageProcObject->showImage(resultPath);
                }
                else {
                    printf("%s\n", resultPath);
                }
            }
        }
    }


    fail:
    FREE_MACRO(finalImageIndexes);
    FREE_MACRO(queryPointArray);
    FREE_MACRO(featuresNum);
    FREE_MACRO(arrImageFeatures);
    FREE_MACRO(totalImageFeaturesArr);
    spConfigDestroy(config); //TODO: if != NULL
    //Destroy KDnode
    //


    return 0;
}


///Users/gideontietz/Desktop/Images/quaryA.png

