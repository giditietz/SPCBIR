#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"
#define ERROR_TITLE "---ERROR---"
#define WARNING_TITLE "---WARNING---"
#define FILE_TITLE "- file: "
#define FUNCTION_TITLE "- function: "
#define LINE_TITLE  "- line: "
#define MESSAGE_TITLE "- message: "
#define DEBUG_TITLE "---DEBUG---"
#define INFO_TITLE "---INFO---"


// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};
/**
 * Creates a logger. This function should be called once, prior
 * to the usage of any SP Logger print functions. It is the responsibility
 * of the user to create the logger prior to usage, and the logger
 * must be destroyed at the end of usage.
 *
 * @param filename - The name of the log file, if not specified stdout is used
 * 					 as default.
 * @param level - The level of the logger prints
 * @return
 * SP_LOGGER_DEFINED 			- The logger has been defined
 * SP_LOGGER_OUT_OF_MEMORY 		- In case of memory allocation failure
 * SP_LOGGER_CANNOT_OPEN_FILE 	- If the file given by filename cannot be opened
 * SP_LOGGER_SUCCESS 			- In case the logger has been successfully opened
 */
SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}
/**
 * Frees all memory allocated for the logger. If the logger is not defined
 * then nothing happens.
 */
void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}
/**
 * 	Prints error message. The error message format is given below:
 * 	---ERROR---
 * 	- file: <file>
 *  - function: <function>
 *  - line: <line>
 *  - message: <msg>
 *
 * 	<file> 	   - is the string given by file, it represents the file in which
 * 		   		 the error print call occurred.
 * 	<function> - is the string given by function, it represents the function in which
 * 			   	 the error print call occurred.
 * 	<line> 	   - is the string given by line, it represents the line in which
 * 		   		 the error print call occurred
 * 	<msg> 	   - is the string given by msg, it contains the msg given by the user
 *
 * 	Error messages will be printed at levels:
 *
 * 	SP_LOGGER_ERROR_LEVEL,
 *	SP_LOGGER_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_INFO_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL
 *
 * 	A new line will be printed after the print call.
 *
 * @param msg     	- The message to printed
 * @param file    	- A string representing the filename in which spLoggerPrintError call occurred
 * @param function 	- A string representing the function name in which spLoggerPrintError call ocurred
 * @param line		- A string representing the line in which the function call occurred
 * @return
 * SP_LOGGER_UNDIFINED 			- If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If any of msg or file or function are null or line is negative
 * SP_LOGGER_WRITE_FAIL			- If Write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line){
	if(logger==NULL){
		return SP_LOGGER_UNDIFINED;
	}
	if(msg==NULL||file==NULL||function==NULL||line<0){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}
	if(fprintf(logger->outputChannel,"%s\n",ERROR_TITLE)<0)
		return SP_LOGGER_WRITE_FAIL;
	if(fprintf(logger->outputChannel,"%s%s\n",FILE_TITLE,file)<0)
		return SP_LOGGER_WRITE_FAIL;
	if(fprintf(logger->outputChannel,"%s%s\n",FUNCTION_TITLE,function)<0)
		return SP_LOGGER_WRITE_FAIL;
	if(fprintf(logger->outputChannel,"%s%d\n",LINE_TITLE,line)<0)
		return SP_LOGGER_WRITE_FAIL;
	if(fprintf(logger->outputChannel,"%s%s\n",MESSAGE_TITLE,msg)<0)
		return SP_LOGGER_WRITE_FAIL;
	return SP_LOGGER_SUCCESS;
}
/**
 * 	Prints warning message. The warning message format is given below:
 * 	---WARNING---
 * 	- file: <file>
 *  - function: <function>
 *  - line: <line>
 *  - message: <msg>
 *
 * 	<file> 	   - is the string given by file, it represents the file in which
 * 		   		 the warning print call occurred.
 * 	<function> - is the string given by function, it represents the function in which
 * 			   	 the warning print call occurred.
 * 	<line> 	   - is the string given by line, it represents the line in which
 * 		   		 the warning print call occurred
 * 	<msg> 	   - is the string given by msg, it contains the msg given by the user
 *
 * 	Warning messages will be printed at levels:
 *
 *	SP_LOGGER_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_INFO_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL
 *
 *	A new line will be printed after the print call.
 *
 * @param msg     	- The message to printed
 * @param file    	- A string representing the filename in which spLoggerPrintWarning call occurred
 * @param function 	- A string representing the function name in which spLoggerPrintWarning call ocurred
 * @param line		- A string representing the line in which the spLoggerPrintWarning call occurred
 * @return
 * SP_LOGGER_UNDIFINED 			- If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If any of msg or file or function are null or line is negative
 * SP_LOGGER_WRITE_FAIL			- If write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line){
	if(logger==NULL){
			return SP_LOGGER_UNDIFINED;
	}
	if(msg==NULL||file==NULL||function==NULL||line<0){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}
	if(logger->level==SP_LOGGER_ERROR_LEVEL){
		return SP_LOGGER_SUCCESS;
	}else{
			if(fprintf(logger->outputChannel,"%s\n",WARNING_TITLE)<0)
				return SP_LOGGER_WRITE_FAIL;
			if(fprintf(logger->outputChannel,"%s%s\n",FILE_TITLE,file)<0)
				return SP_LOGGER_WRITE_FAIL;
			if(fprintf(logger->outputChannel,"%s%s\n",FUNCTION_TITLE,function)<0)
				return SP_LOGGER_WRITE_FAIL;
			if(fprintf(logger->outputChannel,"%s%d\n",LINE_TITLE,line)<0)
				return SP_LOGGER_WRITE_FAIL;
			if(fprintf(logger->outputChannel,"%s%s\n",MESSAGE_TITLE,msg)<0)
				return SP_LOGGER_WRITE_FAIL;
			return SP_LOGGER_SUCCESS;

	}
}
/**
 * 	Prints Info message. The info message format is given below:
 * 	---INFO---
 *  - message: <msg>
 *
 * 	<msg> 	   - is the string given by msg, it contains the msg given by the user
 *
 * 	Info messages will be printed at levels:
 *
 *	SP_LOGGER_INFO_WARNING_ERROR_LEVEL,
 *	SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL
 *
 * 	A new line will be printed after the print call.
 *
 * @param msg     	- The message to printed
 * @return
 * SP_LOGGER_UNDIFINED 			- If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If msg is null
 * SP_LOGGER_WRITE_FAIL			- If Write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG spLoggerPrintInfo(const char* msg){
	if(logger==NULL){
		return SP_LOGGER_UNDIFINED;
	}
	if(msg==NULL){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}
	if(logger->level==SP_LOGGER_INFO_WARNING_ERROR_LEVEL||logger->level==SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
		if(fprintf(logger->outputChannel,"%s\n",INFO_TITLE)<0)
			return SP_LOGGER_WRITE_FAIL;
		if(fprintf(logger->outputChannel,"%s%s\n",MESSAGE_TITLE,msg)<0)
			return SP_LOGGER_WRITE_FAIL;
		return SP_LOGGER_SUCCESS;

	}else{
		return SP_LOGGER_SUCCESS;
	}
}
/**
 * 	Prints the debug message. The debug message format is given below:
 * 	---DEBUG---
 * 	- file: <file>
 *  - function: <function>
 *  - line: <line>
 *  - message: <msg>
 *
 * 	<file> 	   - is the string given by file, it represents the file in which
 * 		   		 the debug print call occurred.
 * 	<function> - is the string given by function, it represents the function in which
 * 			   	 the debug print call occurred.
 * 	<line> 	   - is the string given by line, it represents the line in which
 * 		   		 the debug print call occurred
 * 	<msg> 	   - is the string given by msg, it contains the msg given by the user
 *
 * 	Debug messages will be printed at level:
 *
 *	SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL
 *
 * 	A new line will be printed after the print call.
 *
 * @param msg     	- The message to printed
 * @param file    	- A string representing the filename in which spLoggerPrintWarning call occurred
 * @param function 	- A string representing the function name in which spLoggerPrintWarning call ocurred
 * @param line		- A string representing the line in which the function call occurred
 * @return
 * SP_LOGGER_UNDIFINED 			- If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If any of msg or file or function are null or line is negative
 * SP_LOGGER_WRITE_FAIL			- If Write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line){
	if(logger==NULL){
				return SP_LOGGER_UNDIFINED;
		}
		if(msg==NULL||file==NULL||function==NULL||line<0){
			return SP_LOGGER_INVAlID_ARGUMENT;
		}
	if(logger->level!=SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL){
		return SP_LOGGER_SUCCESS;
	}else{
					if(fprintf(logger->outputChannel,"%s\n",DEBUG_TITLE)<0)
						return SP_LOGGER_WRITE_FAIL;
					if(fprintf(logger->outputChannel,"%s%s\n",FILE_TITLE,file)<0)
						return SP_LOGGER_WRITE_FAIL;
					if(fprintf(logger->outputChannel,"%s%s\n",FUNCTION_TITLE,function)<0)
						return SP_LOGGER_WRITE_FAIL;
					if(fprintf(logger->outputChannel,"%s%d\n",LINE_TITLE,line)<0)
							return SP_LOGGER_WRITE_FAIL;
					if(fprintf(logger->outputChannel,"%s%s\n",MESSAGE_TITLE,msg)<0)
						return SP_LOGGER_WRITE_FAIL;
					return SP_LOGGER_SUCCESS;
	}
}

/**
 * The given message is printed. A new line is printed at the end of msg
 * The message will be printed in all levels.
 *
 * @param msg - The message to be printed
 * @return
 * SP_LOGGER_UNDIFINED 			- If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If msg is null
 * SP_LOGGER_WRITE_FAIL			- If Write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG spLoggerPrintMsg(const char* msg){
	if(logger==NULL){
			return SP_LOGGER_UNDIFINED;
		}
	if(msg==NULL){
		return SP_LOGGER_INVAlID_ARGUMENT;
	}
	else{
		if(fprintf(logger->outputChannel,"%s\n",msg)<0)
			return SP_LOGGER_WRITE_FAIL;
		return SP_LOGGER_SUCCESS;
	}
}
