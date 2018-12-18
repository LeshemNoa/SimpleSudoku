#include "parser.h"

bool parseSetCommand(char* commandStr, Command* commandOut) {
	char* strtokRes = NULL;

	SetCommandArguments args = {0};

	UNUSED(commandStr);

	strtokRes = strtok(NULL, COMMAND_DELIMITERS);
	if (strtokRes == NULL) {
		return false;
	}
	args.col = atoi(strtokRes);

	strtokRes = strtok(NULL, COMMAND_DELIMITERS);
	if (strtokRes == NULL) {
		return false;
	}
	args.row = atoi(strtokRes);

	strtokRes = strtok(NULL, COMMAND_DELIMITERS);
	if (strtokRes == NULL) {
		return false;
	}
	args.value = atoi(strtokRes);

	memcpy(commandOut->arguments, &args, sizeof(args));
	return true;
}

bool parseHintCommand(char* commandStr, Command* commandOut) {
	char* strtokRes = NULL;

	HintCommandArguments args = {0};

	UNUSED(commandStr);

	strtokRes = strtok(NULL, COMMAND_DELIMITERS);
	if (strtokRes == NULL) {
		return false;
	}
	args.col = atoi(strtokRes);

	strtokRes = strtok(NULL, COMMAND_DELIMITERS);
	if (strtokRes == NULL) {
		return false;
	}
	args.row = atoi(strtokRes);

	memcpy(commandOut->arguments, &args, sizeof(args));
	return true;
}

bool parseValidateCommand(char* commandStr, Command* commandOut) {
	ValidateCommandArguments args = {0};

	UNUSED(commandStr);

	memcpy(commandOut->arguments, &args, sizeof(args));
	return true;
}

bool parseRestartCommand(char* commandStr, Command* commandOut) {
	RestartCommandArguments args = {0};

	UNUSED(commandStr);

	memcpy(commandOut->arguments, &args, sizeof(args));
	return true;
}

bool parseExitCommand(char* commandStr, Command* commandOut) {
	ExitCommandArguments args = {0};

	UNUSED(commandStr);

	memcpy(commandOut->arguments, &args, sizeof(args));
	return true;
}

bool parseCommand(char* commandStr, Command* commandOut) {
	char* type = strtok(commandStr, COMMAND_DELIMITERS);
	if (type == NULL) {
		commandOut->type = IGNORE;
		return true;
	}
	if (strcmp(type, "set") == 0) {
		commandOut->type = SET;
		return parseSetCommand(commandStr, commandOut);
	}
	if (strcmp(type, "hint") == 0) {
		commandOut->type = HINT;
		return parseHintCommand(commandStr, commandOut);
	}
	if (strcmp(type, "validate") == 0) {
		commandOut->type = VALIDATE;
		return parseValidateCommand(commandStr, commandOut);
	}
	if (strcmp(type, "restart") == 0) {
		commandOut->type = RESTART;
		return parseRestartCommand(commandStr, commandOut);
	}
	if (strcmp(type, "exit") == 0) {
		commandOut->type = EXIT;
		return parseExitCommand(commandStr, commandOut);
	}

	return false;
}
