#ifndef FIRST_READ_H
#define FIRST_READ_H

#include "utility.h"
#include "common.h"
#include "db_handler.h"

/**********************************************************************************
* This file parses a assembly language file.
* It saves the processed data into data structures, and finds the errors.
**********************************************************************************/
void firstRead(char* fileBaseName, int* IC, int* DC);

#endif
