#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "common.h"
#include "db_handler.h"

typedef enum
{
	NONE = 0, /* Found the seperator */
	DIDNT_EXPECT_SEPERATOR, /* Seperator at the begining of line (imply double comma for instane) */
	NO_TEXT, /* The string is empty */
	TEXT_FOLLOWED_NULL, /* Text and then null terminator (no seperator) */
	MISSING_SEPERATOR /* Found 2 strings but no seperator in between */
} parseString;

/* open file (read/write) by base name + extension, return NULL if fail to do so */
FILE* openFile(char* fileBaseName, char* ext, char* modifier);

/* close the file */
void closeFile(FILE* fp);

/* close file and delete it */
void closeFileAndDelete(FILE* fp, char* fileBaseName, char* ext);

/* get a pointer to next string that
* followed by char seperator or end of line
* report in case unexpected cases
* 1. no seperator. 2. two following seperator. 
* 3. no input text. 4. seperator follow by null
*/
parseString getNextStringFollowedByChar(char** p_inString, char** outString, char seperator);

/* Cut strings into two strings and splinted by a specific separator, report how many strings  */
int getTwoStringsSeperatedByChar(char** p_inString,
	char** secondString,
	char seperator,
	parseString* rc);

/* check if the input string is empty */
bool is_line_empty(char* line);

/* check if the input string is too long */
bool is_line_too_long(char* line);

/* check if is well commented line */
bool is_well_commented_LINE(char* line);

/* check if is badly commented line */
bool is_badly_commented_LINE(char* line);

/* move the file pointer courser to next line */
void run_forward_to_next_line(FILE* fp);

/* trim new line char from the end of line (replace by null terminator) */
void trimNewLine(char* line);

/* Skipping spaces and tabs and updating pointer */
int jumpOverWhitespace(char** inString);

/* Wrap all the above function in order to handle all special lines */
bool skip_line_proceesing_and_register_error(char* line, FILE* fp, int lineNum);

/* Check if label exist in line */
bool is_label_exist(char* line);

/* Splits the label from the rest of the line, returning the following string */
char* split_label(char* line);

/* check if is consecutive label */
bool is_consecutive_label(char* label);

/* check if the label is register name */
bool is_label_register_name(char* label);

/* check if is legal label chars (dont have any numbers) */
bool is_legal_label_chars(char* label);

/* check if is legal label length (no more then defined in common.h) */
bool is_legal_label_length(char* label);

/* Check the label is legal, wrap most of */
bool check_label_correctness(char* label, int lineNum, char* errorStrInit);

/* if have spaces, skip it */
char* expectAndSkipSpaces(char* inString);

/* Retrieve the end of a string if it match the second string. null otherwise */
char* getStringStartWith(char* inString, char* startWith);

/* get a pointer to the following string (skip spaces) */
char* getNextString(char* inString);

/* check if all chars in string are alphabetic  */
bool checkStringAlpah(char* inString);

/* check if all chars in string are digit */
bool checkStringDigit(char* inString);

/* Remove the initial whitespace and check if all the rest is non whitespace string */
bool isSingleSentenceRemoveWhitespace(char** inString);

/* check if string have correct quotation */
bool checkStringQuotation(char* stringData);

/* check if is the end of the string */
bool isEndOfString(char* inString);

/* check if the input string start with the immediate access sign (#) if so, strip it */
bool checkIfImmediateAccessAndStrip(char** inString);

/* If string is integer, then convert into int and return TRUE */
bool getStringInt(char* labelEnd, int* val);

/* Get the register index */
int getRegisterVal(char* label);

/* Copy only the label used in index direct access */
char* copyLabelOnlyReturnIdx(char* fullLabel, char* labelCopy);

/* Remove Squere parantess used for index (if they are well formated() */
bool checkAndTrimSquareParanteses(char** idxP);

/* Remove quatation (if they are well formated() */
void removeQuotation(char** quotatatedString);

/* Checks if the number is in range of 12 bits */
bool isNumberInRangeOf12b(int num);

/* Convert the input word into !@#$ format */
void convertWord(int value, char* buffer);

/* Allocate command info memory before pushing it to DB. */
cmdInfo *allocAndCopyCommanInfo(opStr* opSrc, opStr* opDest, int numOp, int cmdIdx, int icDiff);

/*  Deallocating command info memory after POPing from DB */
void deallocCommanInfo(cmdInfo *cmd);

/* Allocate new string at heap and copy it from input */
char *allocString(char *line);

/* Deallocating input string */
void deallocString(char *line);


/* Register error into line DB */
void registerError(char* errorString, int lineNum);

/* If you want to write an error consisting of 2 strings */
void registerConcatError(char* errorStrInit, char* errorString, int lineNum);

#endif