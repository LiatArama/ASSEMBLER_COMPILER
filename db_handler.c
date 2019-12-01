#include "db_handler.h"

typedef struct typeNumPointer
{
	void *pData;
	int number;
	lineTypeMarker lineType;
	struct typeNumPointer *next;

} typeNumPointer;

typedef struct kvEntry
{
	int val;
	char* name;
	struct kvEntry* next;
}kvEntryStr;

typedef struct flexArray
{
	int *arrPtr;
	int currentSize;
} flexArrayStr;

typedef struct headTNPDb
{
	typeNumPointer *head;
	typeNumPointer *tail;
	typeNumPointer *curr;
} headTNPDbStr;

typedef struct uniqueEntryDb
{
	char* name;
	int value;
	int attribute;
	struct uniqueEntryDb *next;
} uniqueEntryDbStr;

/* Flex array, dynamically growing array, Handling flex array*/
void initFlexArray(flexArryP *ptr)
{
	*ptr = malloc(sizeof(flexArryP));
	(*ptr)->currentSize = INITIAL_DATA_TABLE_SIZE;
	(*ptr)->arrPtr = malloc(sizeof(int)*(*ptr)->currentSize);
}
void closeFlexArray(flexArryP ptr)
{
	free(ptr->arrPtr);
}
void updateFlexArray(flexArryP ptr, int idx, int val)
{
	if (idx >= ptr->currentSize)
	{
		ptr->currentSize *= MUL_DATA_TABLE_SIZE;
		ptr->arrPtr = realloc(ptr->arrPtr, ptr->currentSize*sizeof(int));
		if (NULL == ptr->arrPtr) exit(ERROR_MEMORY_ALLOCATION);
	}
	ptr->arrPtr[idx] = val;
}
int readFlexArray(flexArryP ptr, int idx)
{
	return ptr->arrPtr[idx];
}

/* passing pointer, value and type */

void tnpDbInit(eHeadTNPDb_p *dp)
{
	*dp = malloc(sizeof(headTNPDbStr));
	memset(*dp, 0, sizeof(headTNPDbStr));
}
void tnpDbClose(eHeadTNPDb_p dp)
{
	free(dp);
}
void pushToTail(eHeadTNPDb_p db, void *pData, lineTypeMarker lt, int number)
{
	typeNumPointer **next;
	if (NULL == db->tail) next = &db->tail;
	else next = &db->tail->next;
	*next = malloc(sizeof(typeNumPointer));
	memset(*next, 0, sizeof(typeNumPointer));
	(*next)->lineType = lt;
	(*next)->pData = pData;
	(*next)->number = number;

	db->tail = *next;
	if (NULL == db->head)
	{
		db->head = *next;
		db->curr = *next;
	}
}
bool popHead(eHeadTNPDb_p db, void **pData, lineTypeMarker *lt, int *number)
{
	typeNumPointer *next;
	if (NULL == db->head) return FALSE;

	next = db->head->next;
	*lt = db->head->lineType;
	*pData = db->head->pData;
	*number = db->head->number;
	free(db->head);
	db->head = next;
	return TRUE;
}
bool readNext(eHeadTNPDb_p db, void **pData, lineTypeMarker *lt, int *number)
{
	typeNumPointer *curr = db->curr;
	if (NULL == curr) return FALSE;
	*lt = curr->lineType;
	*pData = curr->pData;
	*number = curr->number;
	db->curr = curr->next;
	return TRUE;
}
void resetNextToHead(eHeadTNPDb_p db)
{
	db->curr = db->head;
}

/* KV data base consist of Key value pair */
void pushToKVStack(KeyValStack_P *head, char* name, int val)
{
	KeyValStack_P curr;
	curr = malloc(sizeof(kvEntryStr));
	if (NULL == curr) exit(ERROR_MEMORY_ALLOCATION);
	if (NULL == *head) curr->next = NULL;
	else curr->next = *head;
	*head = curr;
	curr->val = val;
	curr->name = malloc((strlen(name) + 1) * (sizeof(char)));
	if (NULL == curr->name) exit(ERROR_MEMORY_ALLOCATION);
	strcpy(curr->name, name);
}
bool popFromKVStack(KeyValStack_P* head, char* name, int* val)
{
	KeyValStack_P curr;
	if (*head == NULL) return FALSE;
	curr = *head;
	*head = curr->next;
	if (NULL != val) *val = curr->val;
	if (NULL != name) strcpy(name, curr->name);
	free(curr->name);
	free(curr);
	return TRUE;
}

/* Unique Data base */
bool UDB_addUniqueEntry(UDB_P* head, char* name, int value, int attribute)
{

	uniqueEntryDbStr** curr = head;
	uniqueEntryDbStr* currVal;

	while (*curr != NULL)
	{
		currVal = *curr;
		if (strcmp(currVal->name, name) == 0)  return FALSE; /* not unique symbol */
		curr = &(currVal->next);
	}

	*curr = malloc(sizeof(uniqueEntryDbStr));
	if (NULL == *curr) exit(ERROR_MEMORY_ALLOCATION);

	currVal = *curr;
	currVal->attribute = attribute;
	currVal->name = malloc((strlen(name) + 1) * sizeof(char));
	if (NULL == currVal->name) exit(ERROR_MEMORY_ALLOCATION);
	strcpy(currVal->name, name);

	currVal->value = value;
	currVal->next = NULL;
	return TRUE;
}

bool UDB_getStringlValueAttribute(UDB_P head, char* label, int* value, int *attribute)
{
	uniqueEntryDbStr* curr;
	curr = head;
	while (curr)
	{
		if (strcmp(curr->name, label) == 0)
		{
			if (NULL != value)	   *value = curr->value;
			if (NULL != attribute) *attribute = curr->attribute;
			return TRUE;
		}
		curr = curr->next;
	}
	return FALSE;
}

void UDB_increaseAllSameAttribute(UDB_P head, int increase, int attribute)
{
	uniqueEntryDbStr* curr;
	curr = head;
	while (curr)
	{
		if (attribute == curr->attribute)
		{
			curr->value += increase;
		}
		curr = curr->next;
	}

}

void UDB_deallocDB(UDB_P* head)
{
	uniqueEntryDbStr* curr, *next;
	curr = *head;
	while (curr)
	{
		next = curr->next;
		free(curr->name);
		free(curr);
		curr = next;
	}
	*head = NULL;
}