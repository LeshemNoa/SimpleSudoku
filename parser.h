#ifndef PARSER_H_
#define PARSER_H_

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * commandType keeps all the possible values for the type attribute of a command struct,
 * and an additional IGNORE value, to be used when the command name the user provided
 * contained only whitespaces.
 */
typedef enum commandType {
	SET,
	HINT,
	VALIDATE,
	RESTART,
	EXIT,
	IGNORE} CommandType;

/**
 * SetCommandArguments is a struct that contains the arguments the user provided
 * for a 'set' type command - the value to set, and the column and row number of the cell 
 * to set this value in. 
 */
typedef struct { /* Note: order of row and col is reverse to that provided by user */
	int row;
	int col;
	int value;
} SetCommandArguments;

#define SET_COMMAND_ARGS_NUM (3)

/**
 * HintCommandArguments is a struct that contains the arguments the user provided
 * for a 'hint' type command - the column and row number of the cell the user requests a 
 * hint for. 
 */
typedef struct { /* Note: order of row and col is reverse to that provided by user */
	int row;
	int col;
} HintCommandArguments;

#define HINT_COMMAND_ARGS_NUM (2)

/**
 * For consistency, we use an minimal size struct to respresent the arguments of 
 * commands with no arguments: 'validate', 'restart' and 'exit'
 */
typedef struct {
	char dummy;
} ValidateCommandArguments, RestartCommandArguments, ExitCommandArguments;

#define VALIDATE_COMMAND_ARGS_NUM (0)
#define RESTART_COMMAND_ARGS_NUM (0)
#define EXIT_COMMAND_ARGS_NUM (0)

/**
 * Command is a struct that represents the user's command. It has two attributes -
 * the command's type, matching one of the types defined in the commandType enum,
 * and the command's arguments, a generic pointer to the concrete arguments struct.  
 */
typedef struct {
	CommandType type;
	void* arguments;

} Command;

void cleanupCommand(Command* command);

bool parseCommand(char* commandStr, Command* commandOut); /* TODO: doc that cleanup must be called after successful call to this function */

#endif /* PARSER_H_ */
