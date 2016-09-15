#include <cstdio>
#include <cstdlib>
#include "SPImageProc.h"
#include "Malloc_Macro.h"

#define DEFAULT_FILE_NAME "spcbir.config"
#define C "-c"
#define COMMAND_LINE_ERROR "Invalid command line : use -c"
#define CONFIG_FILENAME "<config_filename>"

#define MAX_LEN 1024

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
    int numOfFeatsQueryImage[1]; //an int[] to comply to imgproc, has a single entry.
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
    if (argc > 3) {
        printf("%s %s\n", COMMAND_LINE_ERROR, argv[2]);
        goto fail;
    }
    if (argc == 1) {
        config = spConfigCreate(DEFAULT_FILE_NAME, &msg);
        if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {//no arguments were entered by user and the default file cannot be open
            printCannotOpenFileNoArguments();
            goto fail;
        }
    } else if (argc == 2 && strcmp(argv[1], C) == 0) {
        printf("%s %s\n", COMMAND_LINE_ERROR, CONFIG_FILENAME);
        goto fail;
    } else if (argc == 3 && strcmp(argv[1], C) == 0) {
        config = spConfigCreate(argv[2], &msg);
        if (msg == SP_CONFIG_CANNOT_OPEN_FILE) {//user entered arguments but the file cannot be open
            printCannotOpenFile(argv[2]);
            goto fail;
        }
        if (!config) {
            goto fail;
        }

    } else {// user didn't entered arguments correctly
        printf("%s %s\n", COMMAND_LINE_ERROR, CONFIG_FILENAME);//if the user entered arguments not correctly
        goto fail;
    }
    //Create Logger
    level = spConfigGetLoggerLevel(config, &msg);
    spConfigGetLoggerName(config, logger);
    if (spLoggerCreate(NULL, level) != SP_LOGGER_SUCCESS) {
        goto fail;
    } else {
        spLoggerPrintInfo("Logger is initialized");
    }
    extraction = spConfigIsExtractionMode(config, &msg);
    imageProcObject = new sp::ImageProc(config);
    if (msg != SP_CONFIG_SUCCESS) {
        goto fail;
    }
    spLoggerPrintInfo("spImageProc Object is initialized");

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
    for (int i = 0; i < imageNum; i++) {
        arrImageFeatures[i] = NULL;
    }


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
                spLoggerPrintError("Failed to get image features", __FILE__, __func__, __LINE__);
                goto fail;
            }
        }
/*        for (int i = 0; i < imageNum; i++) {
            printf("\n%d", featuresNum[i]);
        }*/

        for (int i = 0; i < imageNum; i++) {
            res = writeFeatures(config, i, featuresNum[i], arrImageFeatures[i]);
            if (res != SP_CONFIG_SUCCESS) {
                spLoggerPrintError("Failed to write features", __FILE__, __func__, __LINE__);
                goto fail;
            }
        }
        sprintf(loggerMessage, "Features were extracted from %d images", imageNum);
        spLoggerPrintDebug(loggerMessage, __FILE__, __func__, __LINE__);
        spLoggerPrintInfo("Success: Features extracted");

    } else { //Non-extraction mode
        spLoggerPrintInfo("Retrieve data from feats files");
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
    sprintf(loggerMessage, "Total number of features is %d", sum);
    spLoggerPrintDebug(loggerMessage, __FILE__, __func__, __LINE__);
    createAllImagesPointsArr(&totalImageFeaturesArr, arrImageFeatures, imageNum, sum, featuresNum);
/*    for (int i = 0; i < imageNum; ++i) {
        for (int j = 0; j <featuresNum[i] ; ++j) {
            spPointDestroy(arrImageFeatures[i][j]);

        }

    }*/
    free(arrImageFeatures);
    //Init the data structures:
    kdarray = spKDArrayInit(totalImageFeaturesArr, sum);
    if (NULL == kdarray) {
        spLoggerPrintError("KDArray init failed due to a memory allocation error.", __FILE__, __FUNCTION__, __LINE__);
        goto fail;
    }
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
            //TODO what happens if ENTER is pressed?
            queryPointArray = imageProcObject->getImageFeatures(queryImagePath, indexOfQueryImage,
                                                                numOfFeatsQueryImage);
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


            FUNC_MACRO(spGetFinalImageList(config, root, finalImageIndexes, queryPointArray, numOfFeatsQueryImage[0]));
            minimalGUI = spConfigGetMinimalGUI(config, &msg);

            //Two cases: MinimalGUI or Non-MinimalGUI
            char resultPath[MAX_LEN];
            if (!minimalGUI) { printf("Best candidates for - %s - are:\n", queryImagePath); }

            for (int i = 0; i < numberOfSimilarImages; i++) {
                int indexOfImageToShow = finalImageIndexes[i];
                spConfigGetImagePath(resultPath, config, indexOfImageToShow); //FUNC_MALLOC
                //printf("%s\n", resultPath);
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
    spKDTreeDestroy(root);
    FREE_MACRO(root);//check
    if (NULL != config) {
        spConfigDestroy(config);
    } //TODO: if != NULL
    spLoggerDestroy();
    return 0;
}


///Users/gideontietz/Desktop/Images/quaryA.png

