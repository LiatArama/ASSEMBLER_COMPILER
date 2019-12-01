#ifndef SECOND_READ_H
#define SECOND_READ_H

#include "utility.h"
#include "common.h"
#include "db_handler.h"

/******************************************************************************************
* This function get the processed DBs from the first scan.
* command opcode, Labels, Entries and Extern
* It convert it to a binary commands, and verify errors in Entries and Externs.
******************************************************************************************/
void secondRead(int DC);

#endif
