#include "parser.h"

#define UNUSED(x) (void)(x)

#define COMMAND_DELIMITERS " \t\r\n"

typedef bool (*commandArgsParser)(char* arg, void* argsState, int argNo);
typedef void (*commandArgsCleaner)(void* argsState);

bool parseIntArg(char* arg, int* dst) {
	*dst = atoi(arg); /* For the time being we assume the argument to be a number */
	return true;
}

bool setArgsParser(char* arg, void* argsState, int argNo) {
	SetCommandArguments* setArgsState = (SetCommandArguments*)argsState;
	switch (argNo) {
	case 1:
		return parseIntArg(arg, &(setArgsState->col));
	case 2:
		return parseIntArg(arg, &(setArgsState->row));
	case 3:
		return parseIntArg(arg, &(setArgsState->value));
	}
	return false;
}

bool hintArgsParser(char* arg, void* argsState, int argNo) {
	HintCommandArguments* hintArgsState = (HintCommandArguments*)argsState;
	switch (argNo) {
	case 1:
		return parseIntArg(arg, &(hintArgsState->col));
	case 2:
		return parseIntArg(arg, &(hintArgsState->row));
	}
	return false;
}


bool parseArgs(char* argsStr, void* argumentsStruct, int argsNum, commandArgsParser parser) {
	char* arg = NULL;
	int i = 0;

	UNUSED(argsStr); /* It seems bizarre not to have this parameter, so we keep it even if it isn't used explicitly (it's used implicitly by strtok) */

	for (i = 0; i < argsNum; i++) {
		arg = strtok(NULL, COMMAND_DELIMITERS);
		if (arg == NULL) {
			return false;
		}
		if (! parser(arg, argumentsStruct, i + 1)) {
			return false;
		}
	}

	return true;
}

void cleanupCommand(Command* command) {
	commandArgsCleaner cleaners[] = {NULL, NULL, NULL, NULL, NULL, NULL}; /* No cleanup is necessary for any command yet */

	if (command == NULL)
		return;

	if (cleaners[command->type] != NULL) {
		cleaners[command->type](command->arguments);
	}

 	if (command->arguments != NULL) {
		free(command->arguments);
	}
}

bool parseCommand(char* commandStr, Command* commandOut) {
	int argsNum = 0;
	commandArgsParser parser = NULL;

	char* type = strtok(commandStr, COMMAND_DELIMITERS);
	if (type == NULL) {
		commandOut->type = IGNORE;
		return true;
	} else if (strcmp(type, "set") == 0) {
		commandOut->type = SET;
		commandOut->arguments = calloc(1, sizeof(SetCommandArguments));
		argsNum = SET_COMMAND_ARGS_NUM;
		parser = &setArgsParser;
	} else if (strcmp(type, "hint") == 0) {
		commandOut->type = HINT;
		commandOut->arguments = calloc(1, sizeof(HintCommandArguments));
		argsNum = HINT_COMMAND_ARGS_NUM;
		parser = &hintArgsParser;
	} else if (strcmp(type, "validate") == 0) {
		commandOut->type = VALIDATE;
		commandOut->arguments = calloc(1, sizeof(ValidateCommandArguments));
		argsNum = VALIDATE_COMMAND_ARGS_NUM;
	} else if (strcmp(type, "restart") == 0) {
		commandOut->type = RESTART;
		commandOut->arguments = calloc(1, sizeof(RestartCommandArguments));
		argsNum = RESTART_COMMAND_ARGS_NUM;
	} else if (strcmp(type, "exit") == 0) {
		commandOut->type = EXIT;
		commandOut->arguments = calloc(1, sizeof(ExitCommandArguments));
		argsNum = EXIT_COMMAND_ARGS_NUM;
	} else {
		return false;
	}

	if (commandOut->arguments == NULL) {
		/* NOTE: for now we are allowed this behaviour: */
		printf("Error: parseCommand has failed\n");
		exit(EXIT_FAILURE);
		return false;
	}

	if (parseArgs(commandStr, commandOut->arguments, argsNum, parser)) {
		return true;
	}

	return false;
}
