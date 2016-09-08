#include <cstdio>
#include <cstdlib>
#include "SPImageProc.h"
#define DEFAULT_FILE_NAME "spcbir.config"
#define C "-c"
#define COMMAND_LINE_ERROR "Invalid command line : use -c"

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

}
const int MAX_PATH_LENGTH = 1024;


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

    SPKDNode kdtree = NULL;
    //extraction mode
    sp:: ImageProc* imageProcObject=new sp::ImageProc(config);


    test(config);

    //query
    bool proceed = true;
    SPPoint* queryPointArray;
    int numOfFeatsQueryImage;
    int indexOfQueryImage = INT_MAX;
    while (proceed){
        char path[MAX_PATH_LENGTH];
        puts("\nPlease enter an image path:\n");
        fgets (path, MAX_PATH_LENGTH, stdin); //user input
        if (0 == strcmp("<>\n",path)){ //TODO Remove "\n"!!!
            proceed = false;
            //TODO goto end
        }
        if (proceed){
            queryPointArray = imageProcObject->getImageFeatures(path,indexOfQueryImage,&numOfFeatsQueryImage); //need to check
            int a = 2;









        }

    }

    return 0;
}


///Users/gideontietz/Desktop/Images/quaryA.png