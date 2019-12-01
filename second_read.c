#include "second_read.h"

extern KeyValStack_P entryToExport;
extern KeyValStack_P externToExport;
extern eHeadTNPDb_p lineDb;
extern UDB_P symbolTable;

typedef enum opOrder
{
	DEST_OP,
	SRC_OP
} opOrder;

/******************************************************************************************
* Converting from an opStr format into memory words format
* opStr was created in first read base on command + targets.
* Can reach up to two memory words
*******************************************************************************************/
int initMemoryWord(memoryWord* memoryWords, opStr *op, int currentIc, opOrder srcOrDest, int lineNum)
{
	int dcIcOffset, skip;
	symbolAttribute lSymbolAttribute;
	
	skip = 0;
	
	if (op->access_type == IMIDIATE_ACCESS)
	{
		memoryWords->numberBits.destBits = op->val;
		memoryWords->numberBits.era = LINKER_A;
		skip++;
	}
	else if (op->access_type == DIRECT_ACCESS || op->access_type == FIX_INDEX_ACCESS)
	{
		if (!UDB_getStringlValueAttribute(symbolTable, op->labelName, &dcIcOffset, (int *)&lSymbolAttribute))
			registerError("Symbol was not found in symbol table\n", lineNum);
		else if (lSymbolAttribute != IS_DIRECTIVE  &&
			lSymbolAttribute != IS_EXTERNAL &&
			lSymbolAttribute != IS_INSTRUCTION)
			registerError("Symbol is neither string/data/extern/instruction macro type\n", lineNum);
		else {
			if (lSymbolAttribute == IS_INSTRUCTION)
			{
				memoryWords->adressBits.destBits = dcIcOffset;
			}
			else
			{
				memoryWords->adressBits.destBits = (lSymbolAttribute == IS_EXTERNAL) ? 0 : dcIcOffset;
			}
			memoryWords->numberBits.era = lSymbolAttribute == IS_EXTERNAL ? LINKER_E : LINKER_R;
			if (lSymbolAttribute == IS_EXTERNAL) pushToKVStack(&externToExport, op->labelName, currentIc + skip);
			skip++;
			if (op->access_type == FIX_INDEX_ACCESS)
			{
				if (lSymbolAttribute == IS_EXTERNAL) registerError("Cant access by index to external\n", lineNum);
				else memoryWords[1].numberBits.destBits = op->val;
				skip++;
			}
		}
	}
	else if (op->access_type == REGISTER)
	{
		if (SRC_OP == srcOrDest) memoryWords->regBits.srcBits  = op->val;
		else                     memoryWords->regBits.destBits = op->val;
			 
	}
	return skip;
}
/******************************************************************************************
* Parse the first scan operands.
* Figure out what is next memory word to be written and send them to be convert.
******************************************************************************************/
void initCmd(cmdInfo *info, int *icForExtern, int lineNum)
{
	opStr *opSrc, *opDest;
	memoryWord* memoryWords;
	int totalOps, memIdx;
	
	opDest = info->opDest;
	opSrc = info->opSrc;
	totalOps = (NULL != opDest) + (NULL != opSrc);
	memoryWords = info->cmdWords;

	memIdx = 1; /* First memory word already initialize in first scan */

	if (1 == totalOps) memIdx +=
		initMemoryWord(&(memoryWords[memIdx]), opDest, (*icForExtern + memIdx), DEST_OP, lineNum);
	if (2 == totalOps)
	{
		memIdx += initMemoryWord(&(memoryWords[memIdx]), opSrc, (*icForExtern + memIdx), SRC_OP, lineNum);
		if (REGISTER == opSrc->access_type && REGISTER != opDest->access_type) memIdx++;
		memIdx += initMemoryWord(&(memoryWords[memIdx]), opDest, (*icForExtern + memIdx), DEST_OP, lineNum);
		if (REGISTER == opSrc->access_type && REGISTER == opDest->access_type) memIdx++;
	}
	*icForExtern += memIdx;
}

/*******************************************************
* Check if label that was specified as entry exist
* Register error or push to export data base respectivly
********************************************************/
void checkIfLabelForEntryExportExist(char *entryLabel, int lineNum)
{
	symbolAttribute attribute;
	int outVal;

	if (!UDB_getStringlValueAttribute(symbolTable, entryLabel, &outVal, (int *)&attribute))
	{
		registerError("Couldn't found Label entry to export\n", lineNum);
	}
	else if (attribute != IS_INSTRUCTION && attribute != IS_DIRECTIVE)
	{
		registerError("Trying to export label which is not instruction or directive\n", lineNum);
	}
	else pushToKVStack(&entryToExport, entryLabel, outVal);
}

void secondRead(int DC)
{
	int icForExtern;
	void *info;
	int lineNum;
	lineTypeMarker lt;

	icForExtern = MEMORY_START_ADDRESS;

	/* Read processed line from first read */
	while (readNext(lineDb, &info, &lt, &lineNum))
	{
		if (IS_INSTRUCTION_LINE == lt) initCmd((cmdInfo *)info, &icForExtern, lineNum);
		else if (IS_ENTRY_LINE == lt) checkIfLabelForEntryExportExist((char *)info, lineNum);
	}
	resetNextToHead(lineDb);
}