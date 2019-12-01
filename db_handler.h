#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include "utility.h"

/* Key value DB, more info bellow*/
typedef struct kvEntry* KeyValStack_P;
/* Flex array DB, more info bellow */
typedef struct flexArray* flexArryP;
/* Unique entry DB, more info bellow*/
typedef struct uniqueEntryDb* UDB_P;
/* Data type number DB, more info bellow */
typedef struct headTNPDb* eHeadTNPDb_p;


/**********************************************
* Flex size array DB
* Create and index array data base.
* Array is not static but expend as data grow
* good to consecutive index access
**********************************************/
/* Init the array pointer and size */
void initFlexArray(flexArryP *ptr);
/* Deallocate all the related data base*/
void closeFlexArray(flexArryP ptr);
/* Allow update an index (idx) with a specific value (val) */
void updateFlexArray(flexArryP ptr, int idx, int val);
/* Read specific index (idx) value */
int readFlexArray(flexArryP ptr, int idx);

/**************************************************
* Major attribute: 
* Push to Tail, Pop from head, Read the next entry
* This data base allow to keep:
* void *, line attribute enum and a number
**************************************************/
/* Init the data base HEAD */
void tnpDbInit(eHeadTNPDb_p *dp);
/* Deallocate the data base HEAD */
void tnpDbClose(eHeadTNPDb_p dp);
/* Push data to the tail of DB consist of   */
/*1. void pointer, 2. line type, 3. an int. */
void pushToTail(eHeadTNPDb_p db, void *pData, lineTypeMarker lt, int number);
/* POP off the data base the first entry of Data base*/
/* return FALSE when No more entries where found */
bool popHead(eHeadTNPDb_p db, void **pData, lineTypeMarker *lt, int *number);
/* Read the NEXT entry, the first read is the head of DB*/
/* Return FALSE if reach end of DB */
bool readNext(eHeadTNPDb_p db, void **pData, lineTypeMarker *lt, int *number);
/* Rewind the current pointer (changed in "readNext" to the HEAD of the table */
void resetNextToHead(eHeadTNPDb_p db);


/**************************************************
* Key-Value data base, when:
* key is a pointer to string
* Value is Integer
* Simple data base.
**************************************************/
/**************************************************
* Push to the top of the stack string and val 
* Copy the string to the data base
****************************************************/
void pushToKVStack(KeyValStack_P* head, char* name, int val);
/**************************************************
* POP from the top of the stack string and val
* Copy the inside string into a buffer provided
****************************************************/
bool popFromKVStack(KeyValStack_P* head, char* name, int* val);
/**************************************************************/

/************ Unique DB *****************************
* allow to store a string as unique key
* two non unique int params are attached to each key
* its possible to manipulate (increase by number) 
* the value param for each entry that have the same attribute
****************************************************/
/* Add an entry to DB, the key value is a Copy of the input string 
* Store two int params as value */
bool UDB_addUniqueEntry(UDB_P* head, char* name, int value, int attribute);
/* Get two int params if the input string stored in DB
if it dosent, it return FALSE */
bool UDB_getStringlValueAttribute(UDB_P head, char* label,  int* value, int *attribute);
/* Increase all entries value that share the same attribute value by "increase" */
void UDB_increaseAllSameAttribute(UDB_P head, int increase, int attribute);
/* Deallocate the DB */
void UDB_deallocDB(UDB_P* head);

#endif