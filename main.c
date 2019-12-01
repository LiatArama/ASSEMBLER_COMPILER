/**********************************************************************
* Author :  Liat Arama  302490255 & Idan Calvo  201632163
**********************************************************************/

#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "db_handler.h"
#include "first_read.h"
#include "second_read.h"

/* Data base for each line */
eHeadTNPDb_p lineDb;
/* Entry we need to export to linker in this KV DB */
KeyValStack_P entryToExport;
/* Extern we need to export to linker in this KV DB */
KeyValStack_P externToExport;
/* Array data base hold the data from .data and .string directive */
flexArryP   gDataArray;
/* Unique entry data base to hold the symbol table */
UDB_P symbolTable;

/* Error indicator */
int gErrorCounter;

void printCmdWordsToFile(int numOfWords, memoryWord* cmdWords, int* currentAddr, FILE* fp)
{
	int i;
	char buffer[8];

	for (i = 0; i < numOfWords; i++)
	{
		convertWord(cmdWords[i].value, buffer);
		fprintf(fp, "%04d %s\n", *currentAddr, buffer);
		*currentAddr += 1;
	}
}

void printDbWord(int dcVal, int* currentAddr, FILE* fp)
{
	char buffer[8];
	convertWord(dcVal, buffer);
	fprintf(fp, "%04d %s\n", *currentAddr, buffer);
	*currentAddr += 1;
}

/***************************************************
* printObFiles, create the OB file based on the 
* global data table and the line DB
* Deallocate all the respective data base.
****************************************************/
void printObFiles(char *fileBaseName, int IC, int DC)
{
	FILE* fpOb;
	int lineWritten, currentAddr, dc;
	cmdInfo *info;
	lineTypeMarker lt;
	int lineNumber;

	fpOb = openFile(fileBaseName, ".ob", "w");

	currentAddr = MEMORY_START_ADDRESS;
	fprintf(fpOb, "\t%d %d\n", IC, DC);

	lineWritten = 0;
	while (popHead(lineDb, (void *)&info, &lt, &lineNumber))
	{
		if (lt != IS_INSTRUCTION_LINE) continue;
		lineWritten++;
		printCmdWordsToFile(info->numOfCmdWords, info->cmdWords, &currentAddr, fpOb);
	}

	for (dc = 0; dc < DC; dc++)	printDbWord(readFlexArray(gDataArray, dc), &currentAddr, fpOb);

	if(lineWritten + dc) closeFile(fpOb);
	else closeFileAndDelete(fpOb, fileBaseName, ".ob");
}
/*****************************************
* Get a key value string and print it into
* a filename. deallocate this data base.
************************a*****************/
void printDbToFile(KeyValStack_P *keyValDb, char* fileBaseName, char *ext)
{
	FILE* fp;
	int val, foundName;
	char nameToPrint[MAX_LABEL_SIZE + 1]; /* Max allowed is 31 CHARS, extra for \0*/

	fp = openFile(fileBaseName, ext, "w");

	if (NULL == fp)
	{
		printf("Unable to open file:%s%s", fileBaseName, ext);
		return;
	}
	foundName = 0;
	while (popFromKVStack(keyValDb, nameToPrint, &val) == TRUE)
	{
		fprintf(fp, "%-8s %04d\n", nameToPrint, val);
		foundName++;
	}
	if (foundName) closeFile(fp);
	else closeFileAndDelete(fp, fileBaseName, ext);
}


void printEntryFiles(char* fileBaseName)
{
	printDbToFile(&entryToExport, fileBaseName, ".ent");
}

void printExtFile(char *fileBaseName)
{
	printDbToFile(&externToExport, fileBaseName, ".ext");
}

/*****************************************************
* This is our rollback function.
* in case of error/force switch
* deallocate all the data bases and print all errors
*****************************************************/
bool checkErrorsPrintAndDeallocDB(char *fileBaseName, bool forceDealloc, int *IC, int *DC)
{
	void *info;
	lineTypeMarker lt;
	int lineNumber;
	if (!(gErrorCounter || forceDealloc)) return FALSE;
	while (popHead(lineDb, &info, &lt, &lineNumber))
	{
		if (lt == IS_ERROR_LINE)
		{
			fprintf(stderr, "Error in file : %s line: %d\nMessage: %s\n", fileBaseName, lineNumber + 1, (char *)info);
			deallocString((char *)info);
		}
		else if (lt == IS_INSTRUCTION_LINE) deallocCommanInfo((cmdInfo *)info);
		else if (lt == IS_ENTRY_LINE) deallocString((char *)info);
	}
	while (popFromKVStack(&externToExport, NULL, NULL));
	while (popFromKVStack(&entryToExport, NULL, NULL));

	UDB_deallocDB(&symbolTable);
	tnpDbClose(lineDb);
	gErrorCounter = 0;
	*IC = 0;
	*DC = 0;
	if (!forceDealloc) printf("----------------------------------------\n\n");
	return TRUE;
}

int main(int argc, char* argv[])
{
	int i;
	char* fileBaseName;
	/* Global Data, instruction counter */
	int DC, IC;
	DC = 0;
	IC = 0;


	initFlexArray(&gDataArray);
	for (i = 1; i < argc; i++)
	{
		fileBaseName = argv[i];
		printf("Processing file : %s\n", fileBaseName);
		tnpDbInit(&lineDb);
		firstRead(fileBaseName, &IC, &DC);
		if (checkErrorsPrintAndDeallocDB(fileBaseName, FALSE, &IC, &DC)) continue;
		secondRead(DC);
		if (checkErrorsPrintAndDeallocDB(fileBaseName, FALSE, &IC, &DC)) continue;

		/* Print files*/
		printObFiles(fileBaseName, IC, DC);
		printEntryFiles(fileBaseName);
		printExtFile(fileBaseName);
		
		checkErrorsPrintAndDeallocDB(fileBaseName, TRUE, &IC, &DC);
		printf("Done processing file : %s with no error \n", fileBaseName);
		printf("----------------------------------------\n\n");
	}
	closeFlexArray(gDataArray);
	return NO_ERRORS;
}