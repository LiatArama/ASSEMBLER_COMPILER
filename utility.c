#include "utility.h"

extern eHeadTNPDb_p lineDb;
extern int gErrorCounter;

FILE *openFile(char* fileBaseName, char* ext, char* modifier)
{
	char* fileName;
	FILE* fp;

	fileName = malloc((strlen(fileBaseName) + 5) * sizeof(char));
	if (NULL == fileName) exit(ERROR_MEMORY_ALLOCATION);

	sprintf(fileName, "%s%s", fileBaseName, ext);
	fp = fopen(fileName, modifier);
	free(fileName);
	return fp;
}

void closeFile(FILE* fp)
{
	fclose(fp);
}

void closeFileAndDelete(FILE* fp, char* fileBaseName, char* ext)
{
	char* fileName;
	fclose(fp);
	fileName = malloc((strlen(fileBaseName) + 5) * sizeof(char));
	if (NULL == fileName) exit(ERROR_MEMORY_ALLOCATION);
	sprintf(fileName, "%s%s", fileBaseName, ext);
	remove(fileName);
	free(fileName);
}

void run_forward_to_next_line(FILE* fp)
{
	while ('\n' != fgetc(fp));
}

void trimNewLine(char* line)
{
	while ('\n' != *line && '\0' != *line) line++;
	*line = '\0';
}

bool is_line_empty(char* line)
{
	while (' ' == *line || '\t' == *line) line++;
	if ('\n' == *line || '\0' == *line) return TRUE;
	return FALSE;
}

bool is_line_too_long(char* line)
{
	int lineLength = strlen(line);
	/* if eof, 80 chars */
	if (lineLength <= MAX_LINE_LENGTH) return FALSE;
	/* verify this is not the last line */
	if (lineLength == MAX_LINE_LENGTH)
	{
		while ('\0' != *line && '\n' != *line) line++;
		if ('\n' == *line) return FALSE;
	}
	return TRUE;
}
bool is_well_commented_LINE(char* line)
{
	if (';' == *line)
		return TRUE;
	return FALSE;
}
bool is_badly_commented_LINE(char* line)
{
	if (';' == *line) return FALSE;
	while (' ' == *line || '\t' == *line) line++;
	if (';' == *line) return TRUE;
	return FALSE;
}

/* in case of label */
bool is_label_exist(char* line)
{
	while ('\0' != *line)
	{
		if (':' == *line) return TRUE;
		line++;
	}
	return FALSE;

}
char* split_label(char* line)
{
	char* cmd;
	cmd = strchr(line, ':');
	*cmd = '\0';
	cmd++;
	return cmd;
}
bool is_consecutive_label(char* label)
{
	while (' ' != *label && '\t' != *label && '\0' != *label) label++;
	if (' ' == *label || '\t' == *label)
		return FALSE;
	return TRUE;
}
bool is_legal_label_chars(char* label)
{
	if (isalpha(*label))
	{
		label++;
		while ('\0' != *label)
		{
			if (!isalnum(*label)) return FALSE;
			label++;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
bool is_legal_label_length(char* label)
{
	if (strlen(label) > MAX_LABEL_SIZE) return FALSE;
	return TRUE;
}
bool is_label_register_name(char* label)
{
	if (('r' == *label++) &&
		('0' <= *label) &&
		('7' >= *label++) &&
		('\0' == *label)) return TRUE;
	return FALSE;
}

int getRegisterVal(char* label)
{
	int val;
	sscanf(label, "r%d", &val); /* its safe since we are verifint before this is really the format*/
	return val;
}

char* getNextString(char* inString)
{
	while (*inString == ' ' || *inString == '\t') inString += 1;
	if (*inString == '\0') return NULL;
	while (*inString != ' ' && *inString != '\t' && *inString != '\0') inString += 1;
	return inString;
}

parseString getNextStringFollowedByChar(char** p_inString, char** outString, char seperator)
{
	char* inString = *p_inString;
	while (*inString == '\t' || *inString == ' ') inString++; /* Skip whitespace from start of subText */
	if (*inString == '\0') return NO_TEXT;  /* Found a comma/end of line instead of text */
	if (*inString == seperator) return DIDNT_EXPECT_SEPERATOR;  /* Found a comma/end of line instead of text */
	*outString = inString;
	while (*inString != seperator && *inString != '\t' && *inString != ' ' && *inString != '\0') inString++; /* Skip over text*/
	while (*inString == '\t' || *inString == ' ') /* Skip whitespace from end of subText of line*/
	{
		*inString = '\0';
		inString++;
	}
	if (*inString == '\0')
	{
		*p_inString = inString;
		return TEXT_FOLLOWED_NULL;
	}
	if (*inString != seperator) return MISSING_SEPERATOR; /* Didnt found any comma or end of string*/
	*inString = '\0';
	inString++;
	while (*inString == '\t' || *inString == ' ') inString++; /* Skip whitespace after the comma*/

	*p_inString = inString;
	return NONE;
}

int getTwoStringsSeperatedByChar(char** p_inString,
	char** secondString,
	char seperator,
	parseString* rc)
{
#define NO_STRING 0
#define ONE_STRING 1
#define TWO_STRING 2
#define THREE_STRING 3
	char* testExtraParams;
	*rc = getNextStringFollowedByChar(p_inString, secondString, seperator);
	if (*rc == NO_TEXT || *rc == DIDNT_EXPECT_SEPERATOR) return NO_STRING;
	if (*rc == MISSING_SEPERATOR ||
		*rc == TEXT_FOLLOWED_NULL) return ONE_STRING;
	*rc = getNextStringFollowedByChar(p_inString, &testExtraParams, seperator);
	if (*rc != TEXT_FOLLOWED_NULL) return THREE_STRING;
	*p_inString = testExtraParams;
	return TWO_STRING;
}

char* getStringStartWith(char* inString, char* startWith)
{
	while (*startWith != '\0')
	{
		if (*inString == '\0') return NULL;
		if (*startWith != *inString) return NULL;
		startWith++;
		inString++;
	}
	return inString;
}

bool isEndOfString(char* inString)
{
	while (*inString == ' ' || *inString == '\t') inString++;
	if (*inString == '\0') return TRUE;
	return FALSE;
}

int jumpOverWhitespace(char** inString)
{
	int count = 0;
	while ((' ' == **inString || '\t' == **inString) && '\0' != **inString)
	{
		*inString += 1;
		count++;
	}
	return count;
}

char* expectAndSkipSpaces(char* inString)
{
	if (*inString != ' ' && *inString != '\t') return NULL;
	inString++;
	while (*inString == ' ' || *inString == '\t') inString++;
	return inString;
}

bool checkStringQuotation(char* stringData)
{
	if ('"' != *stringData) return FALSE;
	while ('\0' != *(stringData+1)) stringData++;
	if ('"' != *stringData) return FALSE;
	return TRUE;
}

bool isSingleSentenceRemoveWhitespace(char** inString)
{
	char* temp;
	while (('\t' == **inString || ' ' == **inString) && '\0' != **inString)
		* inString += 1;
	temp = *inString;
	while ('\t' != *temp && ' ' != *temp && '\0' != *temp) temp++;
	while (('\t' == *temp || ' ' == *temp) && '\0' != *temp)
	{
		*temp = '\0';
		temp++;
	}
	if ('\0' == *temp) return TRUE;
	return FALSE;
}

void removeQuotation(char** quotatatedString)
{
	char* temp;
	**quotatatedString = '\0';
	*quotatatedString += 1;
	temp = *quotatatedString;
	while (*temp != '\0' && *temp != '"') temp++;
	*temp = '\0';
}

bool checkStringAlpah(char* inString)
{
	while ('\0' != *inString)
	{
		if (!isalpha(*inString)) return FALSE;
		inString++;
	}
	return TRUE;
}

bool checkStringDigit(char* inString)
{
	while ('\0' != *inString)
	{
		if (!isdigit(*inString)) return FALSE;
		inString++;
	}
	return TRUE;
}

char* copyLabelOnlyReturnIdx(char* fullLabel, char* labelCopy)
{
	while (*fullLabel != '[' && *fullLabel != '\0')
	{
		*labelCopy = *fullLabel;
		labelCopy++;
		fullLabel++;
	}
	*labelCopy = '\0';
	return fullLabel;
}

bool getStringInt(char* labelEnd, int* val)
{
	if (1 == sscanf(labelEnd, "%d", val)) return TRUE;
	else return FALSE;
}

bool getIfImmediateAccessVal(char* inString, int* val)
{
	inString++;
	if (1 == sscanf(inString, "%d", val)) return TRUE;
	else return FALSE;
}

bool checkIfImmediateAccessAndStrip(char** inString)
{
	if (**inString == '#')
	{
		*inString += 1;
		return TRUE;
	}
	return FALSE;
}

bool isNumberInRangeOf12b(int num)
{
#define MAX_RANGE  ((2 << 11) -1)
#define MIN_RANGE -((2 << 11) -1)
	if (num < MAX_RANGE && num > MIN_RANGE) return TRUE;
	return FALSE;
}

/* input after we checked its imidiate, get #-2 for instance*/
bool checkImmediateAccessCorrectness(char* inString)
{
	int tempVal;
	inString++;
	if (1 == sscanf(inString, "%d", &tempVal)) return TRUE;
	return FALSE;
}

/* return false if not well wrap*/
bool checkAndTrimSquareParanteses(char** idxP)
{
	/* We assume that the input string is at least of the form of "[lalaxxx "*/
	char* temp;
	*idxP = *idxP + 1;
	temp = *idxP;
	while ('\0' != *temp && ']' != *temp) temp++;
	if (']' == *temp && '\0' == *(temp + 1))
	{
		*temp = '\0';
		return TRUE;
	}
	return FALSE;
}

void convertWord(int value, char* buffer)
{
	int shifter, nibble, index;
	index = 0;
	for (shifter = 12; shifter >= 0; shifter -= 2)
	{
		nibble = (value >> shifter) & 0x3;
		switch (nibble)
		{
		case 0: buffer[index] = '*'; break;
		case 1: buffer[index] = '#'; break;
		case 2: buffer[index] = '%'; break;
		case 3: buffer[index] = '!'; break;
		}
		index++;
	}
	buffer[7] = '\0';
}

cmdInfo *allocAndCopyCommanInfo(opStr* opSrc, opStr* opDest, int numOp, int cmdIdx, int icDiff)
{
	cmdInfo *cmdToPush;
	cmdToPush = malloc(sizeof(cmdInfo));
	if (NULL == cmdToPush) exit(ERROR_MEMORY_ALLOCATION);
	memset(cmdToPush, 0, sizeof(cmdInfo));

	cmdToPush->numOfCmdWords = icDiff;

	cmdToPush->cmdWords = malloc(icDiff * sizeof(memoryWord));
	if (NULL == cmdToPush->cmdWords) exit(ERROR_MEMORY_ALLOCATION);

	memset(cmdToPush->cmdWords, 0, icDiff * sizeof(memoryWord));
	cmdToPush->cmdWords[0].cmdBits.era = LINKER_A;
	cmdToPush->cmdWords[0].cmdBits.opcode = cmdIdx;

	if (numOp >= 1) cmdToPush->cmdWords[0].cmdBits.dest = opDest->access_type;
	if (numOp == 2) cmdToPush->cmdWords[0].cmdBits.src = opSrc->access_type;

	if (numOp >= 1)
	{
		cmdToPush->opDest = malloc(sizeof(opStr));
		if (NULL == cmdToPush->opDest) exit(ERROR_MEMORY_ALLOCATION);
		memcpy(cmdToPush->opDest, opDest, sizeof(opStr));
	}

	if (numOp == 2)
	{
		cmdToPush->opSrc = malloc(sizeof(opStr));
		if (NULL == cmdToPush->opSrc) exit(ERROR_MEMORY_ALLOCATION);
		memcpy(cmdToPush->opSrc, opSrc, sizeof(opStr));
	}
	return cmdToPush;
}

void deallocCommanInfo(cmdInfo *cmd)
{
	if (NULL != cmd->opDest) free(cmd->opDest);
	if (NULL != cmd->opSrc) free(cmd->opSrc);
	free(cmd->cmdWords);
	free(cmd);
}

char *allocString(char *line)
{
	char *name;
	name = malloc((strlen(line) + 1) * sizeof(char));
	if (NULL == name) exit(ERROR_MEMORY_ALLOCATION);
	strcpy(name, line);
	return name;
}

void deallocString(char *line)
{
	if (NULL != line) free(line);
}


void registerError(char* errorString, int lineNum)
{
	char *name;
	name = allocString(errorString);
	gErrorCounter += 1;
	pushToTail(lineDb, name, IS_ERROR_LINE, lineNum);
}

void registerConcatError(char* errorStrInit, char* errorString, int lineNum)
{
	char *name;
	gErrorCounter += 1;
	name = malloc((strlen(errorString) + strlen(errorStrInit) + 1) * sizeof(char));
	if (NULL == name) exit(ERROR_MEMORY_ALLOCATION);
	strcpy(name, errorStrInit);
	strcat(name, errorString);
	registerError(name, lineNum);
	free(name);
}

