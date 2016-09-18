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
/**
 * print constrains Not Met error msg when constarins not met
 * @param fileName
 * @param lineNumber
 */
void printConstrainsMessage(const char* fileName,int lineNumber);
/**
 * prints error msg when parameter is not set
 * @param fileName
 * @param lineNumber
 * @param param
 */
void parameterNotSetMessage(const char* fileName,int lineNumber,char* param);
/**
 * print cannot open file err msg when cannot open file which path was given as an argument
 * @param fileName
 */
void printCannotOpenFile(char* fileName);
/**
 * prints cannot open file err msg when cannot open default file
 */
void printCannotOpenFileNoArguments();
#endif //FINALPROJECT_REGERROR_H
