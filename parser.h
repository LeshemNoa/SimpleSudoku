#ifndef PARSER_H_
#define PARSER_H_

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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

#define SET_COMMAND_ARGS_NUM (3)

typedef struct { /* Note: order of row and col is reverse to that provided by user */
	int row;
	int col;
} HintCommandArguments;

#define HINT_COMMAND_ARGS_NUM (2)

typedef struct {
	char dummy;
} ValidateCommandArguments, RestartCommandArguments, ExitCommandArguments;

#define VALIDATE_COMMAND_ARGS_NUM (0)
#define RESTART_COMMAND_ARGS_NUM (0)
#define EXIT_COMMAND_ARGS_NUM (0)

typedef struct {
	CommandType type;
	void* arguments;

} Command;

void cleanupCommand(Command* command);

bool parseCommand(char* commandStr, Command* commandOut); /* TODO: doc that cleanup must be called after successful call to this function */

#endif /* PARSER_H_ */
