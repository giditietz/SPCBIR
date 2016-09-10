//
// Created by Ido Ilani on 29/08/2016.
//
#include <stdlib.h>
#include <stdio.h>
#ifndef FINALPROJECT_REGERROR_H
#define FINALPROJECT_REGERROR_H
/**
 *
 * @param fileName
 * @param lineNumber
 * @param msg
 * prints an error msg when an invalid line error occures.
 */
void printInvalidLineErrorMsg(const char* fileName,int lineNumber);
void printConstrainsMessage(const char* fileName,int lineNumber);
void parameterNotSetMessage(const char* fileName,int lineNumber,char* param);
void printCannotOpenFile(char* fileName);
void printCannotOpenFileNoArguments();
#endif //FINALPROJECT_REGERROR_H
