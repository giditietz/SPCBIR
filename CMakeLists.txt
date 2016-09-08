cmake_minimum_required(VERSION 3.5)

project(FinalProject)
#FIND_PACKAGE( OpenCV REQUIRED core highgui imgproc )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp KDTreeTest.c SPKDArray.h SPKDArray.c SPPoint.c SPPoint.h SPBPriorityQueue.h SPBPriorityQueue.c SPList.c
        SPList.h SPListElement.c SPListElement.h SPKDTree.h SPKDTree.c SPLogger.c SPLogger.h SPFinalImageList.h SPFinalImageList.c
        SPFinalImageList.c IndexedPoint.h IndexedPoint.c SPImageProc.h SPImageProc.cpp SPConfig.c SPConfig.h regError.h
        regError.c Malloc_Macro.h main.cpp Functions.h Functions.c KDTreeTest.c)
#find_package(OpenCV REQUIRED)
link_directories("/usr/local/Cellar/opencv3/3.1.0_3/lib")
set(OpenCV_INCLUDE_DIRS "/usr/local/Cellar/opencv3/3.1.0_3/include")
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(SPCBIR ${SOURCE_FILES})
enable_testing('NO')
target_link_libraries(SPCBIR
        opencv_core
        opencv_imgproc
        opencv_ximgproc
        opencv_contrib
        opencv_legacy
        opencv_imgcodecs
        opencv_ml
        opencv_highgui
        opencv_xfeatures2d)


