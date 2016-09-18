//
// Created by Ido Ilani on 29/08/2016.
//
#include "regError.h"
#define  LINE_STRING "\nLine: "
#define FILE_STRING "File: "
#define MESSAGE_STRING "\nMessage: "
#define INVALID_LINE_MESSAGE "Invalid configuration line\n"
#define CONSTRAINT_MESSAGE "Invalid value - constraint not met\n"
#define PARAMETER "Parameter "
#define NOTSET " is not set\n"
#define CONFIG_FILE "The configuration file "
#define COULDNOT_BE_OPEN "couldn’t be open"
#define NOARGUMENTS "The default configuration file spcbir.config couldn’t be open"
#define FORMAT_ONE "%s%s%s%d%s%s"
#define FORMAT_TWO "%s%s%s%d%s%s%s%s"
#define FORMAT_THREE "%s%s %s\n"
#define FORMAT_FOUR "%s\n"

void printInvalidLineErrorMsg(const char* fileName,int lineNumber){
    printf(FORMAT_ONE,FILE_STRING,fileName,LINE_STRING,lineNumber,MESSAGE_STRING,INVALID_LINE_MESSAGE);
}
void printConstrainsMessage(const char* fileName,int lineNumber){
    printf(FORMAT_ONE,FILE_STRING,fileName,LINE_STRING,lineNumber,MESSAGE_STRING,CONSTRAINT_MESSAGE);
}
void parameterNotSetMessage(const char* fileName,int lineNumber,char* param){
    printf(FORMAT_TWO,FILE_STRING,fileName,LINE_STRING,lineNumber,MESSAGE_STRING,PARAMETER,param,NOTSET);
}
void printCannotOpenFile(char* fileName){
    printf(FORMAT_THREE,CONFIG_FILE,fileName,COULDNOT_BE_OPEN);
}
void printCannotOpenFileNoArguments(){
    printf(FORMAT_FOUR,NOARGUMENTS);
}