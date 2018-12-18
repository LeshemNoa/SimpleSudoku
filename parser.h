#ifndef PARSER_H_
#define PARSER_H_

/* TODO: VISIBILITY */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define UNUSED(x) (void)(x)

#define COMMAND_ARGUMENTS_MAX_SIZE (sizeof(SetCommandArguments)) /* TODO: Should perhaps actually perform maximisation calculation here */
#define COMMAND_DELIMITERS " \t\r\n"

typedef enum commandType {
	SET,
	HINT,
	VALIDATE,
	RESTART,
	EXIT,
	IGNORE} CommandType;

typedef struct { /* Note: order of row and col is reverse to that provided by user */
	int row;
	int col;
	int value;
} SetCommandArguments;

typedef struct { /* Note: order of row and col is reverse to that provided by user */
	int row;
	int col;
} HintCommandArguments;

typedef struct {
	char dummy;
} ValidateCommandArguments, RestartCommandArguments, ExitCommandArguments;

typedef struct {
	CommandType type;
	char arguments[COMMAND_ARGUMENTS_MAX_SIZE]; /* TODO: this argument is to be casted appropriately */
												/* TODO: consider using a pointer here, although that would entail memory allocation and therefore more overhead for the user, which may be unwarranted in this case */

} Command;

bool parseSetCommand(char* commandStr, Command* commandOut);

bool parseHintCommand(char* commandStr, Command* commandOut);

bool parseValidateCommand(char* commandStr, Command* commandOut);

bool parseRestartCommand(char* commandStr, Command* commandOut);

bool parseExitCommand(char* commandStr, Command* commandOut);

bool parseCommand(char* commandStr, Command* commandOut);

#endif /* PARSER_H_ */
