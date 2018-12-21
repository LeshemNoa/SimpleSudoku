#include "main_aux.h"

void printSeparatorLine() {
	int numDashes = N * (N * CELL_SIZE_IN_PRINT + BLOCK_OVERHEAD_SIZE_IN_PRINT) + LINE_OVERHEAD_SIZE_IN_PRINT;
	for (; numDashes > 0; numDashes--) {
		printf("-");
	}
	printf("\n");
}

void printLine(Board* board, int lineIndex) {
	int col = 0;

	printf("|");

	for (col = 0; col < N_SQUARE; col++) {
		printf(" ");
		printf("%c", board->cells[lineIndex][col].isFixed ? '.' : ' ');
		if (board->cells[lineIndex][col].value != EMPTY_CELL_VALUE) {
			printf("%d", board->cells[lineIndex][col].value);
		} else {
			printf(" ");
		}

		if ((col % N) == N - 1) {
			printf(" ");
			printf("|");
		}
	}

	printf("\n");
}

void printBoard(Board* board) {
	int row = 0;

	printSeparatorLine();

	for (row = 0; row < N_SQUARE; row++) {
		printLine(board, row);
		if ((row % N) == N - 1) {
			printSeparatorLine();
		}
	}
}

bool isNumCellsToFillValid(int numCellsToFill) {
	return (numCellsToFill >= 0) && (numCellsToFill <= (N_SQUARE * N_SQUARE - 1));
}

bool getNumCellsToFill(int* numCellsToFillOut) {
	while (true) {
		int scanfRetVal = 0;
		printf("Please enter the number of cells to fill [%d-%d]:\n", 0, N_SQUARE * N_SQUARE - 1);
		scanfRetVal = scanf("%d", numCellsToFillOut);
		if (feof(stdin) || (scanfRetVal == EOF)) {
			return false;
		}
		if (scanfRetVal == 1) {
			if (isNumCellsToFillValid(*numCellsToFillOut)) {
				return true;
			}
			else {
				printf("Error: invalid number of cells to fill (should be between %d and %d)\n", 0, N_SQUARE * N_SQUARE - 1);
			}
		} else {
			printf("Error: not a number\n");
			/* NOTE: for now we are allowed this behaviour: */
			printf("Error: getNumCellsToFill has failed\n");
			exit(EXIT_FAILURE);
		}
	}
}

bool initialStage(State** state) {
	Board board = {{{{0}}}};

	int numFixedCells = 0;

	if (!generatePuzzle(&board)) {
		return false;
	}

	if (!getNumCellsToFill(&numFixedCells)) {
		return false;
	}

	if (!initialise(numFixedCells, state, &board)) {
		return false;
	}

	exportBoard(*state, &board);
	printBoard(&board);

	return true;
}

bool getCommandString(char* commandStrOut, int commandMaxSize) {
	char* fgetsRes = fgets(commandStrOut, commandMaxSize, stdin);
	return fgetsRes != NULL;
}

void performSetCommand(State* state, SetCommandArguments* args) {
	SetErrorType error;
	if (!set(state, args->row - 1, args->col - 1, args->value, &error)) {
		switch (error) {
		case VALUE_FIXED:
			printf("Error: cell is fixed\n");
			break;
		case VALUE_INVALID:
			printf("Error: value is invalid\n");
			break;
		}
	} else {
		Board board = {{{{0}}}};
		exportBoard(state, &board);
		printBoard(&board);

		if (isGameWon(state)) {
			printf("Puzzle solved successfully\n");
		}
	}
}

void performHintCommand(State* state, HintCommandArguments* args) {
	printf("Hint: set cell to %d\n", hint(state, args->row - 1, args->col - 1));
}

void performValidateCommand(State* state) {
	Board solution = {{{{0}}}};
	if (solvePuzzle(state, &solution)) {
		printf("Validation passed: board is solvable\n");
	} else {
		printf("Validation failed: board is unsolvable\n");
	}
}

void performCommand(State* state, Command* command, bool* shouldRestart, bool* shouldExit) {
	switch (command->type) {
	case SET:
		performSetCommand(state, command->arguments);
		break;
	case HINT:
		performHintCommand(state, command->arguments);
		break;
	case VALIDATE:
		performValidateCommand(state);
		break;
	case RESTART:
		*shouldRestart = true;
		break;
	case EXIT:
		*shouldExit = true;
		break;
	case IGNORE:
		break;
	}
}

bool performCommandLoop(State* state) {
	bool shouldExit = false;
	while (true) {
		bool shouldRestart = false;
		Command command = {0};
		char commandStr[COMMAND_MAX_LENGTH + 1] = {0};

		if (!getCommandString(commandStr, COMMAND_MAX_LENGTH + 1)) {
			shouldExit = true;
			break;
		}

		if ((!parseCommand(commandStr, &command)) || (isGameWon(state) && command.type != RESTART && command.type != EXIT)){
			printf("Error: invalid command\n");
		} else {
			performCommand(state, &command, &shouldRestart, &shouldExit);
		}

		cleanupCommand(&command);

		if (shouldRestart || shouldExit) {
			break;
		}
	}
	return shouldExit;
}

bool runGame() {
	bool shouldExit = false;

	State* state = NULL;

	if (initialStage(&state)) {
		shouldExit = performCommandLoop(state);
		destruct(state);
	} else {
		shouldExit = true;
	}

	if (shouldExit)
		printf("Exiting...\n");

	return shouldExit;
}
