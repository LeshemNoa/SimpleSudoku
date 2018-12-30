#include "parser.h"

#define UNUSED(x) (void)(x)

#define COMMAND_DELIMITERS " \t\r\n"

/* function pointer to a concrete command type's ArgParser. There currently are 2 of these:
* setArgsParser and hintArgsParser.	
* @params arg			[in] the string containing the specific argument currently
 * 						being parsed
* @params argState 		[in, out] a pointer to the concrete command argument struct
* @params argNo		 	[in] the parsed argument's index
*/
typedef bool (*commandArgsParser)(char* arg, void* argsState, int argNo);

/* 
* Reserved for future use. 
*
* function pointer to a concrete implementation of a command cleaner of a specific
* command type.
* 
* @params argState	a generic pointer to the concrete command argument struct
* 
*
*/
typedef void (*commandArgsCleaner)(void* argsState);

/**
 * parseIntArg parses a string representing an integer into an integer, then
 * assigns it to the provided integer pointer. 
 * 
 * @param arg		[in] the string to be parsed 
 * @param dst 		[in, out] a pointer to an integer to be assigned with
 * 					the parse result
 * @return true		iff parsing was successful 
 * @return false 	iff provided input was not entirely composed of digits
 */
bool parseIntArg(char* arg, int* dst) {
	char* p;

	/* validate that arg is all digits */
	for (p = arg; *p != '\0'; p++)
		if (*p < '0' || '9' < *p)
			return false;

	*dst = atoi(arg);
	return true;
}

/**
 * setArgsParser concretely implements an argument parser for the 'set' command. 
 * 
 * @param arg			[in] the string containing the specific argument currently
 * 						being parsed and assigned to the appropriate attribute 
 * @param argsState		[in, out] a generic pointer to a command argument struct, casted
 * 						to be a SetCommandArgument struct containing the arguments	 
 * @param argNo 		[in] the parsed argument's index: argument 1 is the number of
 * 						column of the cell for which the user requested a hint, argument 
 * 						2 is the number of the row, and argument 3 is the value to be set
 * 						in the cell with those indices
 * @return true 		iff parseIntArg successfully parsed and set a valid integer
 * @return false 		iff the parsing failed
 */
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

/**
 * hintArgsParser concretely implements an argument parser for the 'hint' command. 
 * 
 * @param arg	 		[in] the string containing the specific argument currently
 * 						being parsed and assigned to the appropriate attribute
 * @param argsState		[in, out] a generic pointer to a command argument struct, casted
 * 						to be a HintCommandArgument struct containing the arguments	
 * @param argNo 		[in] the parsed argument's index: argument 1 is the number of
 * 						column of the cell for which the user requested a hint, and
 * 						argument 2 is the number of the row
 * @return true			iff parseIntArg successfully parsed and set a valid integer 		
 * @return false 		iff the parsing failed
 */
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

/**
 * Called by parseCommand, parseArgs continutes processing the user input strings to finish 
 * the initialization of the Command struct, started by the caller. parseArgs provides
 * an appropriate parser for the type of command currently being processed. parseArgs uses the
 * parser to create a struct of the command's arguments according to its type. 
 * 
 * @param argsStr				[in] the user input string containing the command arguments 
 * @param argumentsStruct 		[in, out] a generic pointer to be assigned the command struct
 * 								with appropriate arguments after parsing is finished
 * @param argsNum 				[in] the expected number of command arguments according to 
 * 								the command type
 * @param parser				[in] the appropriate parser for the given type of command 
 * @return true 				iff the user provided a sufficient number of valid arguments
 * @return false 				iff the user provided an insufficient number of arguments, 
 * 								too many arguments, or invalid arguments values.
 */
bool parseArgs(char* argsStr, void* argumentsStruct, int argsNum, commandArgsParser parser) {
	char* arg = NULL;
	int i = 0;

	UNUSED(argsStr); /* It seems bizarre not to have this parameter, so we keep it even 
	if it isn't used explicitly (it's used implicitly by strtok) */

	for (i = 0; i < argsNum; i++) {
		arg = strtok(NULL, COMMAND_DELIMITERS);
		if (arg == NULL) {
			/* no remaining tokens in input can happen if expected arg count isn't reached */
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

	if (command == NULL || command->arguments == NULL) {
		return;
	}

	if (cleaners[command->type] != NULL) {
		cleaners[command->type](command->arguments);
	}

	free(command->arguments);
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

	/* check if arguments allocation failed */
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
