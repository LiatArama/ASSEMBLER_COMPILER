#include "utility.h"
#include "first_read.h"
extern eHeadTNPDb_p lineDb;
extern flexArryP   gDataArray;
extern UDB_P symbolTable;

typedef struct
{
	char* name;
	unsigned int opcode : 4;
	int numOfParams;
	int destination[5];
	int source[5];
} command;

#define ACCESS_ALL {IMIDIATE_ACCESS, DIRECT_ACCESS, FIX_INDEX_ACCESS, REGISTER, INVALID}
#define ACCESS_ALL_NO_IM { DIRECT_ACCESS, FIX_INDEX_ACCESS, REGISTER, INVALID}
#define ACCESS_DIRECT_AND_REG { DIRECT_ACCESS, REGISTER, INVALID}
const command cmd_array[] =
{	/* Name | Opcode | NumOfParams | destination |	source */
{ "mov",    0,         2,       ACCESS_ALL_NO_IM,		ACCESS_ALL} ,
{ "cmp",    1,         2,       ACCESS_ALL,				ACCESS_ALL} ,
{ "add",    2,         2,       ACCESS_ALL_NO_IM,		ACCESS_ALL} ,
{ "sub",    3,         2,       ACCESS_ALL_NO_IM,		ACCESS_ALL} ,
{ "not",    4,         1,       ACCESS_ALL_NO_IM,		{INVALID}} ,
{ "clr",    5,         1,       ACCESS_ALL_NO_IM,		{INVALID}} ,
{ "lea",    6,         2,       ACCESS_ALL_NO_IM,		{DIRECT_ACCESS,FIX_INDEX_ACCESS,INVALID}} ,
{ "inc",    7,         1,       ACCESS_ALL_NO_IM,		{INVALID}} ,
{ "dec",    8,         1,       ACCESS_ALL_NO_IM,		{INVALID}} ,
{ "jmp",    9,         1,       ACCESS_DIRECT_AND_REG,	{INVALID}} ,
{ "bne",    10,        1,       ACCESS_DIRECT_AND_REG,	{INVALID}} ,
{ "red",    11,        1,       ACCESS_ALL_NO_IM,		{INVALID}} ,
{ "prn",    12,        1,       ACCESS_ALL,				{INVALID}} ,
{ "jsr",    13,        1,       ACCESS_DIRECT_AND_REG,	{INVALID}} ,
{ "rts",    14,        0,		{INVALID},				{INVALID}} ,
{ "stop",   15,        0,		{INVALID},				{INVALID}} ,
{ NULL } /* Break point for cmd search */
};

/* Get the command index */
#define CMD_NOT_FOUND -1
#define CMD_W_EXTRA_SUFFIX -2
int get_cmd_idx(char* cmd)
{
	int i;
	char* restOfCmd;
	for (i = 0; cmd_array[i].name; i++)
		if ((restOfCmd = getStringStartWith(cmd, cmd_array[i].name)) != NULL)
		{
			if (*restOfCmd == ' ' || *restOfCmd == '\t' || *restOfCmd == '\0') return i;
			else return CMD_W_EXTRA_SUFFIX;
		}
	return CMD_NOT_FOUND;
}

/* Check if label follow the correct format, register error otherwise */
bool check_label_correctness(char* label, int lineNum, char *errorStrInit)
{
	if (!is_consecutive_label(label))
	{
		registerConcatError(errorStrInit, " is non consecutive\n", lineNum);
		return FALSE;
	}
	if (is_label_register_name(label))
	{
		registerConcatError(errorStrInit, " is register name\n", lineNum);
		return FALSE;
	}
	if (get_cmd_idx(label) >= 0)
	{
		registerConcatError(errorStrInit, " is Command name\n", lineNum);
		return FALSE;
	}

	if (!is_legal_label_chars(label))
	{
		registerConcatError(errorStrInit, " using illegal chars\n", lineNum);
		return FALSE;
	}
	if (!is_legal_label_length(label))
	{
		registerConcatError(errorStrInit, " is toooo long\n", lineNum);
		return FALSE;
	}
	return TRUE;
}

/* Check if the access of the Destination operand is legal */
bool checkIfDestAccessIsLegal(int cmdIdx, int accessUt)
{
	int i = 0;
	for (;cmd_array[cmdIdx].destination[i] != INVALID; i++)	if (cmd_array[cmdIdx].destination[i] == accessUt) return TRUE;
	return FALSE;
}

/* Check if the access of the Source operand is legal */
bool checkIfSrcAccessIsLegal(int cmdIdx, int accessUt)
{
	int i = 0;
	for (;cmd_array[cmdIdx].source[i] != INVALID; i++)	if (cmd_array[cmdIdx].source[i] == accessUt) return TRUE;
	return FALSE;
}

/* Figure out what is the type of the operand and store it into opStr */
bool processAndCheckOperand(opStr* op, char* opString, int lineNum)
{
	char* idxP, * labelCopy;
	symbolAttribute attribute;
	if (checkIfImmediateAccessAndStrip(&opString)) /* start with # */
	{
		op->access_type = IMIDIATE_ACCESS;
		/*  only numbers and Macro !!! */
		if (!getStringInt(opString, &op->val)) /* not a number, can be data/macro*/
		{
			if (is_label_register_name(opString))
			{
				registerError("Immediate is a register name!\n", lineNum);
				return TRUE;
			}
			if (!UDB_getStringlValueAttribute(symbolTable, opString, &op->val, (int *)&attribute))
			{
				/* couldn't find a macro with this name*/
				registerError("Trying to use undefined MACRO in immediate access\n", lineNum);
				return TRUE;
			}
			else if (attribute != IS_MACRO)
			{
				registerError("Trying to use Non  MACRO in immediate access\n", lineNum);
				return TRUE;
			}
		}
		else /* A number */
		{
			if (!isNumberInRangeOf12b(op->val))
			{
				registerError("Command immediate number is out of 12b range\n", lineNum);
				return TRUE;
			}
		}
	} else if (is_label_register_name(opString))
	{
		op->access_type = REGISTER;
		op->val = getRegisterVal(opString);
		return FALSE;
	} else /* probably label */
	{
		op->access_type = DIRECT_ACCESS;
		labelCopy = malloc((strlen(opString) + 1) * sizeof(char));
		if (NULL == labelCopy) exit(ERROR_MEMORY_ALLOCATION);

		idxP = copyLabelOnlyReturnIdx(opString, labelCopy);
		op->labelName = labelCopy;
		if (!check_label_correctness(labelCopy, lineNum, "FIX Direct IDX"))	return TRUE;
		else
		{
			if (strlen(idxP)) /* have idx */
			{
				if (TRUE == checkAndTrimSquareParanteses(&idxP))
				{
					if (!checkStringDigit(idxP))
					{
						if (!UDB_getStringlValueAttribute(symbolTable, idxP, &op->val, (int *)&attribute))
						{
							registerError("Trying to use non existed MACRO as index for direct access\n", lineNum);
							return TRUE;
						} else if (attribute != IS_MACRO)
						{
							registerError("Trying to use Non MACRO as index for direct access\n", lineNum);
							return TRUE;
						}
					}
					else getStringInt(idxP, &op->val);
					if (op->val < 0) /* Negative index is not allowed */
					{
						registerError("Error, immediate access with negative fix value \n", lineNum);
						return TRUE;
					}
				} else
				{
					registerError("Fix direct access parentheses are not well formated!! \n", lineNum);
					return TRUE;
				}
				op->access_type = FIX_INDEX_ACCESS;
			}
		}
	}
	return FALSE;
}
/* Calculate what is the size of the input command */
int increaseInstructionCount(opStr* opSrc, opStr* opDest, int cmdIdx)
{
	int icDiff;
	icDiff = 1;
	if (2 == cmd_array[cmdIdx].numOfParams)
	{
		icDiff += 2;
		if (opSrc->access_type == FIX_INDEX_ACCESS) icDiff++;
		if (opDest->access_type == FIX_INDEX_ACCESS) icDiff++;
		if (opSrc->access_type == REGISTER && opDest->access_type == REGISTER) icDiff = 2;
	}
	else if (1 == cmd_array[cmdIdx].numOfParams)
	{
		icDiff++;
		if (opDest->access_type == FIX_INDEX_ACCESS) icDiff++;
	}
	return icDiff;
}

/* Create an cmdInfo instand and push it to data base */
void pushCmdInfoToDb(int lineNum, opStr* opSrc, opStr* opDest, int numOp, int cmdIdx, int icDiff)
{
	
	cmdInfo *cmdToPush = allocAndCopyCommanInfo(opSrc, opDest, numOp, cmdIdx, icDiff);
	pushToTail(lineDb, cmdToPush, IS_INSTRUCTION_LINE, lineNum);
}

/***************************************************************************
* Figure out if what command is being declared.
* parse the dest and src operands and push them into the DB.
* Create the first word which will be later completed.
* increase Instruction count respectivly.
* register errors if needed.
****************************************************************************/
bool handleCommands(char* cmdLine,
	char* label,
	int* IC,
	int	lineNum)
{
	int paramsFound, cmdIdx, icDiff;
	opStr opDest, opSrc;
	parseString rc;
	char* firstOp, * secondOp, * restOfCmdLine;

	cmdIdx = get_cmd_idx(cmdLine);
	if (cmdIdx == CMD_NOT_FOUND) return FALSE;

	if (cmdIdx == CMD_W_EXTRA_SUFFIX)
	{
		registerError("Expected space after command name\n", lineNum);
		return TRUE;
	}

	if (label != NULL)
	{
		if (!UDB_addUniqueEntry(&symbolTable, label, *IC, IS_INSTRUCTION))
		{
			registerError("Label already exist\n", lineNum);
			return TRUE;
		}
	}

	restOfCmdLine = getNextString(cmdLine);
	paramsFound = getTwoStringsSeperatedByChar(&restOfCmdLine, &firstOp, ',', &rc);
	secondOp = restOfCmdLine;

	if (paramsFound == 0)
	{
		if (rc != NO_TEXT)
		{
			registerError("Didnt expect comma after command\n", lineNum);
			return TRUE;
		}
	}
	else if (paramsFound == 1)
	{
		if (rc != TEXT_FOLLOWED_NULL)
		{
			registerError("Found two strings separated by space\n", lineNum);
			return TRUE;
		}
	}

	if (cmd_array[cmdIdx].numOfParams != paramsFound)
	{
		registerError("Wrong input params\n", lineNum);
		return TRUE;
	}

	if (paramsFound == 1)
	{
		if (processAndCheckOperand(&opDest, firstOp, lineNum)) return TRUE;
		if (!checkIfDestAccessIsLegal(cmdIdx, opDest.access_type))
		{
			registerError("This cmd Don't support this access type for Dest operand\n", lineNum);
			return TRUE;
		}
	}

	if (paramsFound == 2)
	{
		if (processAndCheckOperand(&opSrc, firstOp, lineNum)) return TRUE;
		if (!checkIfSrcAccessIsLegal(cmdIdx, opSrc.access_type))
		{
			registerError("This cmd Don't have this access type for Source operand\n", lineNum);
			return TRUE;
		}

		if (processAndCheckOperand(&opDest, secondOp, lineNum)) return TRUE;
		if (!checkIfDestAccessIsLegal(cmdIdx, opDest.access_type))
		{
			registerError("This cmd Don't support this access type for destination operand\n", lineNum);
			return TRUE;
		}

	}

	icDiff = increaseInstructionCount(&opSrc, &opDest, cmdIdx); /* Increase instruction */
	pushCmdInfoToDb(lineNum, &opSrc, &opDest, paramsFound, cmdIdx, icDiff);
	*IC += icDiff;
	return TRUE;
}

void pushStringToDb(int lineNum, char *line, lineTypeMarker type)
{
	char *name;
	name = allocString(line);
	pushToTail(lineDb, name, type, lineNum);
}

/***************************************************************************
* Figure out if the input cmdLine is a well formated entry deceleration.
* register it with into the line DB.
* Will be checked later on if label do exist and export to DB and file
* register errors if needed.
****************************************************************************/
bool registerIfEntry(char* cmdLine, char* label, int lineNum)

{
	char* followingString = getStringStartWith(cmdLine, ".entry");
	if (followingString)
	{
		if (isEndOfString(followingString))
		{
			registerError("Expected some data after .entry\n", lineNum);
			return TRUE;
		}

		followingString = expectAndSkipSpaces(followingString);
		if (followingString == NULL)
		{
			registerError("Expected spaces after .entry\n", lineNum);
			return TRUE;
		}

		if (!check_label_correctness(followingString, lineNum, "entry")) return TRUE;
		pushStringToDb(lineNum, followingString, IS_ENTRY_LINE);
		return TRUE;
	}
	return FALSE;
}

/***************************************************************************
* Figure out if the input cmdLine is a well formated extern deceleration.
* register it with DC offset into symbol table.
* Will be use later on when completing the Command words
* register errors if needed.
****************************************************************************/
bool handleExtern(char* cmdLine, char* label, int lineNum)
{
	char* followingString = getStringStartWith(cmdLine, ".extern");
	if (followingString)
	{
		if (isEndOfString(followingString))
		{
			registerError("Expected some data after .entry\n", lineNum);
			return TRUE;
		}

		followingString = expectAndSkipSpaces(followingString);
		if (followingString == NULL)
		{
			registerError("Expected spaces after .extern\n", lineNum);
			return TRUE;
		}

		if (!check_label_correctness(followingString, lineNum, "Extern")) return TRUE;
		if(!UDB_addUniqueEntry(&symbolTable, followingString, 0, IS_EXTERNAL))
			registerError("Extern already declared\n", lineNum);
		return TRUE;
	}
	return FALSE;
}

/* Handle and check .string chars input to data table */
void checkAndAddStringToDataSection(char* iString, int lineNum, int* DC)
{
	char inC;
	if (!checkStringQuotation(iString))
	{
		registerError("String is not encapsulated right in quotation\n", lineNum);
		return;
	}
	removeQuotation(&iString);

	while (1)
	{
		if (1 == sscanf(iString, "%c", &inC))
		{
			updateFlexArray(gDataArray, *DC, inC);
			iString++;
			*DC += 1;
		}
		else
		{
			updateFlexArray(gDataArray, *DC, 0);
			*DC += 1;
			return;
		}
	}
}

/* Handle and check .data digit input to data table */
void checkAndAddDataToDataSection(char* stringData, int lineNum, int* DC)
{
	parseString rc;
	symbolAttribute macroAttribute;

	char* inProcess;
	int storeVal;

	while (TRUE)
	{
		rc = getNextStringFollowedByChar(&stringData, &inProcess, ',');
		if (rc == NO_TEXT) break;
		if (rc == DIDNT_EXPECT_SEPERATOR)
		{
			registerError("Didnt expect 2 following commas\n", lineNum);
			return;
		} else if (rc == MISSING_SEPERATOR)
		{
			registerError("No comma to separate input\n", lineNum);
			return;
		}
		if (1 != sscanf(inProcess, "%d", &storeVal))
		{
			if (!check_label_correctness(inProcess, lineNum, "Data var")) return;
			
			if (!UDB_getStringlValueAttribute(symbolTable, inProcess, &storeVal, (int *)&macroAttribute))
			{
				registerError("Macro was not initialize\n", lineNum);
				return;
			}
			if (macroAttribute != IS_MACRO)
			{
				registerError("Given data name is not a macro\n", lineNum);
				return;
			}
		}

		updateFlexArray(gDataArray, *DC, storeVal);
		*DC += 1;
	}
}

/***************************************************************************
* Figure out if the input cmdLine a .string directive.
* register it with DC offset into symbol table.
* register chars into data table
* register errors if needed.
****************************************************************************/
bool handleString(char* cmdLine, char* label, int* DC, int  lineNum)
{
	char* followingString = getStringStartWith(cmdLine, ".string");
	if (followingString)
	{
		if (isEndOfString(followingString))
		{
			registerError("Expected some data after .string\n", lineNum);
			return TRUE;
		}

		followingString = expectAndSkipSpaces(followingString);
		if (followingString == NULL)
		{
			registerError("Expected spaces after .string\n", lineNum);
			return TRUE;
		}

		if (!isSingleSentenceRemoveWhitespace(&followingString))
		{
			registerError(".string string is not a single sentence \n", lineNum);
			return TRUE;
		}

		
		if ((label != NULL) && !UDB_addUniqueEntry(&symbolTable, label, *DC, IS_DIRECTIVE))
		{
			registerError("String already declared\n", lineNum);
			return TRUE;
		}

		checkAndAddStringToDataSection(followingString, lineNum, DC);
		return TRUE;
	}
	return FALSE;
}

/***************************************************************************
* Figure out if the input cmdLine a .data directive.
* register it with DC offset into symbol table.
* register data into data table
* register errors if needed.
****************************************************************************/
bool handleData(char* cmdLine,
	char* label,
	int* DC,
	int	lineNum)
{
	char* followingString = getStringStartWith(cmdLine, ".data");
	if (followingString)
	{
		if (isEndOfString(followingString))
		{
			registerError("Expected some data after .data\n", lineNum);
			return TRUE;
		}

		followingString = expectAndSkipSpaces(followingString);
		if (followingString == NULL)
		{
			registerError("Expected spaces after .data\n", lineNum);
			return TRUE;
		}

		if ((label != NULL) && !UDB_addUniqueEntry(&symbolTable, label, *DC, IS_DIRECTIVE))
		{
			registerError("Data already declared\n", lineNum);
			return TRUE;
		}

		checkAndAddDataToDataSection(followingString, lineNum, DC);
		return TRUE;
	}
	return FALSE;
}
/***************************************************************************
* Figure out if the input cmdLine is well formated macro.
* register it into symbol table.
* register errors if needed.
****************************************************************************/
bool handleMacro(char* cmdLine, char* label, int lineNum)
{
	parseString rc;
	int val, numberOfStringsFound;
	char* defineNameStart, * defineMacroName;
	char* followingDefine = getStringStartWith(cmdLine, ".define");
	if (followingDefine)
	{
		if (isEndOfString(followingDefine))
		{
			registerError("Expected some data after .define\n", lineNum); /* TODO: register the error */
			return TRUE;
		}

		if (label != NULL)
		{
			registerError("Macro cant have a label\n", lineNum);
			return TRUE;
		}

		defineNameStart = expectAndSkipSpaces(followingDefine);

		if (defineNameStart == NULL)
		{
			registerError("Expected spaces after MACRO\n", lineNum);
			return TRUE;
		}

		numberOfStringsFound = getTwoStringsSeperatedByChar(&defineNameStart, &defineMacroName, '=', &rc);
		if (numberOfStringsFound != 2)
		{
			switch (numberOfStringsFound)
			{
			case 0: registerError("couldn't found any params\n", lineNum); break;
			case 1: registerError("couldn't found second params\n", lineNum); break;
			case 3: registerError("found too many params\n", lineNum); break;
			default:; break;
			}
			return TRUE;
		}

		if (!checkStringDigit(defineNameStart))
		{
			registerError("Macro value is not an int...\n", lineNum);
			return TRUE;
		}

		getStringInt(defineNameStart, &val);
		if (!isNumberInRangeOf12b(val))
		{
			registerError("Macro define a number out of 12b range\n", lineNum);
			return TRUE;
		}

		if (!check_label_correctness(defineMacroName, lineNum, "Macro")) return TRUE;
		if(!UDB_addUniqueEntry(&symbolTable, defineMacroName, val, IS_MACRO))
			registerError("String already declared\n", lineNum);

		return TRUE;
	}
	return FALSE;
}

/* check if label exist, separate it from the rest of the line, register error if not well formated */
bool splitAndHndleLabel(char** inLine, char** label, char** restOfLine, int lineNum)
{
	*label = NULL;
	*restOfLine = *inLine;
	jumpOverWhitespace(restOfLine);
	if (is_label_exist(*restOfLine))
	{
		*label = *restOfLine;
		*restOfLine = split_label(*restOfLine);
		if (!jumpOverWhitespace(restOfLine))
		{
			registerError("No spaces after LABEL :\n", lineNum);
			return FALSE;
		}
		jumpOverWhitespace(label);
		if (!check_label_correctness(*label, lineNum, "Label")) return FALSE;
	}
	return TRUE;
}

/* Check if we need to skip this line or process. register errors if any.*/
bool skip_line_proceesing_and_register_error(char* line, FILE* fp, int lineNum)
{
	if (is_line_too_long(line))
	{
		registerError("line too long\n", lineNum);
		run_forward_to_next_line(fp);
		return TRUE;
	}
	if (is_well_commented_LINE(line)) return TRUE;

	if (is_line_empty(line)) return TRUE;

	if (is_badly_commented_LINE(line)) return TRUE;
	trimNewLine(line);
	return FALSE;
}

/*****************************************************************
* Read and parse the "as" file. export info into data base
*****************************************************************/
void firstRead(char* fileBaseName, int* IC, int* DC)
{
	FILE* fp;
	int lineNum;
	char line[MAX_LINE_LENGTH + 2];
	char *linePtr, * label, * rest_of_the_line;
	
	fp = openFile(fileBaseName, ".as", "r");
	if (NULL == fp)
	{
		registerError("Could not open as file\n", 0);
		return;
	}

	linePtr = line;
	for(lineNum = 0; fgets(line, MAX_LINE_LENGTH + 2, fp); lineNum++) /* fgets read n-1 */
	{
		if (skip_line_proceesing_and_register_error(line, fp, lineNum))	continue;
		if (!splitAndHndleLabel(&linePtr, &label, &rest_of_the_line, lineNum)) continue;

		if      (handleCommands(rest_of_the_line, label, IC, lineNum))	continue;
		else if (handleMacro(rest_of_the_line, label, lineNum))			continue;
		else if (handleData(rest_of_the_line, label, DC, lineNum))		continue;
		else if (handleString(rest_of_the_line, label, DC, lineNum))	continue;
		else if (handleExtern(rest_of_the_line, label, lineNum))    	continue;
		else if (registerIfEntry(rest_of_the_line, label, lineNum))		continue;
		else     registerError("Could not identify this line meaning\n", lineNum);
	}

	UDB_increaseAllSameAttribute(symbolTable, *IC + MEMORY_START_ADDRESS, IS_DIRECTIVE);
	UDB_increaseAllSameAttribute(symbolTable, MEMORY_START_ADDRESS, IS_INSTRUCTION);
}