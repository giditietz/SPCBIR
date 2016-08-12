cmake_minimum_required(VERSION 3.5)
project(SPCBIR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp SPKDArray.h SPKDArray.c SPPoint.c SPPoint.h SPBPriorityQueue.h SPBPriorityQueue.c SPList.c SPList.h SPListElement.c SPListElement.h SPKDTree.h SPKDTree.c)
add_executable(SPCBIR ${SOURCE_FILES})