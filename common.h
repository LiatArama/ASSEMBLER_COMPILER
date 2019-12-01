#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>

#define MAX_LINE_LENGTH 80
#define MAX_LABEL_SIZE 31
#define MAX_LINES 4096
#define WORD_SIZE_IN_MEM 14
#define MEMORY_START_ADDRESS 100
#define INITIAL_DATA_TABLE_SIZE 2
#define MUL_DATA_TABLE_SIZE 2

#define LINKER_A 0
#define LINKER_E 1
#define LINKER_R 2

typedef unsigned int bool;
#define FALSE	0
#define TRUE	1

#define NO_ERRORS 0
#define ERROR_MEMORY_ALLOCATION 5

typedef enum symbolAttribute
{
	IS_INSTRUCTION, /* mov, add, sub */
	IS_DIRECTIVE,   /* .string .data - get DC as value */
	IS_MACRO,       /* .define, get the value */
	IS_EXTERNAL,
	IS_ENTRY,
	NO_ATTRIBUTE
} symbolAttribute;

typedef enum lineTypeMarker
{
	IS_INSTRUCTION_LINE,
	IS_ERROR_LINE,
	IS_ENTRY_LINE
} lineTypeMarker;


typedef enum accessType
{
	IMIDIATE_ACCESS = 0,
	DIRECT_ACCESS,
	FIX_INDEX_ACCESS,
	REGISTER,
	INVALID
}  accessType;

typedef enum lineStatus
{
	LINE_IS_CMD,
	LINE_IS_ENTRY,
	LINE_IS_EXTERN,
	LINE_HAVE_ERROR
}  lineStatus;

typedef struct op
{
	accessType access_type;
	int val; /* can be reg num/index/immediate val */
	char* labelName;
}opStr;

typedef union /* 14 bits */
{
	/* Commands */
	struct
	{
		unsigned int era : 2;       /* A, R, E - always 00 */
		unsigned int dest : 2;	/* Destination op */
		unsigned int src : 2;	/* Source op */
		unsigned int opcode : 4;	/* Command ID */
		unsigned int : 4;	/* Unused Bit */
	} cmdBits;

	/* Registers */
	struct
	{
		unsigned int era : 2;       /* A,R,E - always 00 */
		unsigned int destBits : 3;  /* Destination register */
		unsigned int srcBits : 3;	/* Source register */
		unsigned int reserved : 6;	/* Unused Bit */
	} regBits;

	/* Adresses */
	struct
	{
		unsigned int era : 2;        /* A,R,E  ->  E=01, R=10 */
		unsigned int destBits : 12;  /* The adress */
	} adressBits;

	/* Number */
	struct
	{
		unsigned int era : 2;        /* A,R,E - always 00 */
		unsigned int destBits : 12;  /* The number */

	} numberBits;
	/* Other operands */
	int value : 14;      /* (14 bits) */

} memoryWord;


typedef struct cmdInfo
{
	opStr* opDest;
	opStr* opSrc;
	memoryWord* cmdWords;
	int numOfCmdWords;
} cmdInfo;

#endif
