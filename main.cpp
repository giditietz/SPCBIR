/*#include <iostream>
extern "C"{
#include "SPConfig.h"
#include "SPImageProc.h"
//#include "SPLogger.h"
//#include "SPLogger.c"
}
#define DEFAULT_FILE_NAME "spcbir.config"
#define C "-c"
#define COMMAND_LINE_ERROR "Invalid command line : use -c"

int main(int argc, char *argv[]) {
    SP_CONFIG_MSG msg;
    SPConfig config=NULL;
    int res;
    //Create SPConfig
    if(argc==1){
        config=spConfigCreate(DEFAULT_FILE_NAME,&msg);
        if(msg==SP_CONFIG_CANNOT_OPEN_FILE){//no arguments were entered by user and the default file cannot be open
            printCannotOpenFileNoArguments();
        }
    }
    else if(strcmp(argv[1],C)==0){
        config=spConfigCreate(argv[2],&msg);
        if(msg==SP_CONFIG_CANNOT_OPEN_FILE){//user entered arguments but the file cannot be open
            printCannotOpenFile(argv[2]);
        }
    }
    else{// user didn't entered arguments correctly
        printf("%s %s\n",COMMAND_LINE_ERROR,argv[2]);//if the user entered arguments not correctly

    }

    //extraction mode
  //  sp:: ImageProc* imageProcObject=new sp::ImageProc(config);




}
*/
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
#include "SPBPriorityQueue.h"
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
    //Decelerations & Inits
    SP_CONFIG_MSG msg;
    SPConfig config = NULL;
    int res = SP_CONFIG_SUCCESS;
    int *featuresNum = NULL;
    SPPoint **arrImageFeatures = NULL;
    int imageNum;
    int totalFeatures;
    SPPoint *totalImageFeatures = NULL;
    SP_KD_TREE_SPLIT_METHOD method;
    bool extraction;
    int temp = 0;
    char imagePath[MAX_LEN];
    bool proceed = true;
    SPPoint *queryPointArray;
    int numOfFeatsQueryImage;
    int indexOfQueryImage = INT_MAX;

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

    }


    test(config); //temporary test for kdtree, only works with conf-gidi.txt (change the images path)

    //query

    while (proceed) {
        char path[MAX_LEN];
        puts("\nPlease enter an image path:\n");
        fgets(path, MAX_LEN, stdin); //user input
        if (0 == strcmp("<>\n", path)) { //TODO Remove "\n"!!!
            proceed = false;
            puts("Exiting...\n");
            goto fail; //Program is terminated, so we move to the cleanup phase.
        }
        if (proceed) {
            queryPointArray = imageProcObject->getImageFeatures(path, indexOfQueryImage,
                                                                &numOfFeatsQueryImage); //need to check
        }
    }
    fail:
    FREE_MACRO(featuresNum);
    FREE_MACRO(arrImageFeatures);
    FREE_MACRO(totalImageFeatures);
    spConfigDestroy(config);


    return 0;
}


///Users/gideontietz/Desktop/Images/quaryA.png

