CC = gcc
CPP = g++
#put your object files here
OBJS = main.o SPLogger.o SPPoint.o SPKDArray.o SPKDTree.o SPBPriorityQueue.o SPList.o SPListElement.o Functions.o SPConfig.o SPImageProc.o regError.o SPFinalImageList.o IndexedPoint.o
#The executabel filename
EXEC = SPCBIR
INCLUDEPATH=/usr/local/lib/opencv-3.1.0/include/
LIBPATH=/usr/local/lib/opencv-3.1.0/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core


CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -g -pedantic-errors -DNDEBUG

C_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -g -pedantic-errors -DNDEBUG


#Main
$(EXEC): $(OBJS)
	$(CPP) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
main.o: main.cpp SPKDArray.h SPKDTree.h SPFinalImageList.h Functions.h Malloc_Macro.h SPImageProc.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp

#Image Proc
SPImageProc.o: SPImageProc.cpp SPImageProc.h SPConfig.h SPPoint.h SPLogger.h
	$(CPP) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp

#Assignment 3 modules
SPBPriorityQueue.o: SPBPriorityQueue.c SPBPriorityQueue.h SPList.h SPListElement.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPList.o: SPList.c SPList.h SPListElement.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPListElement.o: SPListElement.c SPListElement.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPPoint.o: SPPoint.c SPPoint.h
	$(CC) $(C_COMP_FLAG) -c $*.c

#SPConfig
SPConfig.o: SPConfig.c SPConfig.h SPLogger.h regError.h Malloc_Macro.h
	$(CC) $(C_COMP_FLAG) -c $*.c

#KDTree and KDArray
SPKDArray.o: SPKDArray.c SPKDArray.h  SPLogger.h SPPoint.h Malloc_Macro.h IndexedPoint.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPKDTree.o: SPKDTree.c SPKDTree.h SPPoint.h SPBPriorityQueue.h  SPLogger.h SPKDArray.h SPConfig.h
	$(CC) $(C_COMP_FLAG) -c $*.c
IndexedPoint.o: IndexedPoint.c IndexedPoint.h  SPPoint.h
	$(CC) $(C_COMP_FLAG) -c $*.c

#Function for the query data aggregation
SPFinalImageList.o: SPFinalImageList.c SPFinalImageList.h SPKDArray.h SPBPriorityQueue.h SPKDTree.h SPConfig.h Malloc_Macro.h
	$(CC) $(C_COMP_FLAG) -c $*.c

#Util
Functions.o: Functions.c Functions.h SPPoint.h SPConfig.h Malloc_Macro.h
	$(CC) $(C_COMP_FLAG) -c $*.c
regError.o: regError.c regError.h
	$(CC) $(C_COMP_FLAG) -c $*.c



clean:
	rm -f $(OBJS) $(EXEC)
