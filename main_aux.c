#include "main_aux.h"

#define COMMAND_MAX_LENGTH (1024)

#define CELL_SIZE_IN_PRINT (3)
#define BLOCK_OVERHEAD_SIZE_IN_PRINT (2)
#define LINE_OVERHEAD_SIZE_IN_PRINT (1)

/**
 * printSeparatorLine calculates the number of dashes in a line separator of a sudoku board
 * of size N, and prints the separating line.
 * 
 */
void printSeparatorLine() {
	int numDashes = N * (N * CELL_SIZE_IN_PRINT + BLOCK_OVERHEAD_SIZE_IN_PRINT) + LINE_OVERHEAD_SIZE_IN_PRINT;
	for (; numDashes > 0; numDashes--) {
		printf("-");
	}
	printf("\n");
}

/**
 * printLine prints an individual line of a given sudoku board, according to the format.
 * 
 * @param board			[in] the board whose line will be printed 
 * @param lineIndex 	[in] the index of the line to be printed
 */
void printLine(Board* board, int lineIndex) {
	int col = 0;

	printf("|");

	for (col = 0; col < N_SQUARE; col++) {
		printf(" ");
		printf("%c", isCellFixed(board, lineIndex, col) ? '.' : ' ');
		if (! isCellEmpty(board, lineIndex, col)) {
			printf("%d", getCellValue(board, lineIndex, col));
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

/**
 * printBoard prints out the current state of the game board according to the format.
 * 
 * @param board		[in] a pointer to the sudoku board to be printed out 
 */
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

/**
 * isNumCellsToFillValid verifies that the number of cells to fill is adequate:
 * i.e., that it is non-negative, and not greater than the total number of cells minus 1
 * in the board.
 *
 * @param numCellsToFill	[in] the number of cells to fill, which is to be checked
 * @return true				iff numCellsToFill is appropriate (within the range [0, N*N - 1])
 * @return false			iff numCellsToFill is not appropriate
 */
bool isNumCellsToFillValid(int numCellsToFill) {
	return (numCellsToFill >= 0) && (numCellsToFill <= (N_SQUARE * N_SQUARE - 1));
}

/**
 * getNumCellsToFill is called during game initialization. It prompts the user to provided the
 * desired number of fixed cells in the sudoku board they're about to play. If the number of cells
 * the user requested to fix is not in the appropriate range, an error message is printed out. If
 * the input the user provided is not an integer, the entire process is terminated.
 * 
 * @param numCellsToFillOut		[in, out] a pointer to an integer, assigned with the number of fixed
 * 								cells in the sudoku board initialized 
 * @return true					iff the user provided a valid number of cells to fix
 * @return false 				iff stdin reached EOF
 */
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

/**
 * initialStage initializes the state of the game when it begins. It takes a pointer to an 
 * at first null pointer, allocates it and writes the new sudoku board generated to that pointer.
 * Afterwards, the new sudoku board is printed to the user. 
 * 
 * @param state		[in, out] a pointer to a null pointer, to be allocated and assigned by
 * 					initialStage 
 * @return true		iff the game has been successfully initialized 
 * @return false 	iff either getting the number of fixed board cells, puzzle generation
 * 					or game initialization has failed
 */
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

/**
 * getCommandString reads a command string from stdin, and writes the input
 * into the provided string pointer.
 * 
 * @param commandStrOut		[out] a pointer to a string. 
 * @param commandMaxSize	[in] the maximum input size in bytes that should be read stdin
 * @return true 			iff a string was successfully fetched
 * @return false 			iff there had been an error and the input fetch had failed
 */
bool getCommandString(char* commandStrOut, int commandMaxSize) {
	char* fgetsRes = fgets(commandStrOut, commandMaxSize, stdin);
	return fgetsRes != NULL;
}

/**
 * performSetCommand executes a given 'set' command from the user.
 * It attempts to set the board entry according to the user's command. If the command
 * could not be completed, an appropriate error message is displayed to the user. If 
 * the command was successfully executed, the updated sudoku board is printed. If the
 * game has been finished afterwards, a win message is printed. 
 * 
 * @param state		[in, out] current state of the game 
 * @param args 		[in] a pointer to the arguments of the user's set command
 */
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

/**
 * performHintCommand executes a given 'hint' command from the user.
 * It prints out the pre-generated hint for the particular index the user has provided.
 * The hint is then fetched by a second auxiliary function.
 * 
 * @param state		[in] current state of the game 
 * @param args		[in] a pointer to the command arguments of the user's hint command
 */
void performHintCommand(State* state, HintCommandArguments* args) {
	printf("Hint: set cell to %d\n", hint(state, args->row - 1, args->col - 1));
}

/**
 * performValidateCommand executes a given 'validate' command from the user.
 * It uses the backtracking algorithm to solve the sudoku in its current state.
 * An appropriate message is presented to the user, indicating whether the game
 * can or cannot be solved in its current condition. 
 * 
 * Additionally, if the current game is found to be solvable, the solution produced
 * during the validation process is set to be the new solution of the game.
 * 
 * @param state		[in] current state of the game 
 */
void performValidateCommand(State* state) {
	Board solution = {{{{0}}}};
	if (solvePuzzle(state, &solution)) {
		printf("Validation passed: board is solvable\n");
		setPuzzleSolution(state, &solution);
	} else {
		printf("Validation failed: board is unsolvable\n");
	}
}

/**
 * performCommand uses a switch statement to select how to update the game's state according to
 * the type of the command provided as a parameter. It either calls an executing function 
 * matching that command type, or updates attributes of the game's state.
 * 
 * @param state				[in] the current state of the game 
 * @param command 			[in] a pointer to the user's command
 * @param shouldRestart		[in, out] a pointer to the game's state shouldRestart attribute, 
 * 							allowing its update 
 * @param shouldExit 		[in, out] a pointer to the game's state shouldRestart attribute, 
 * 							allowing its update
 */
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

/**
 * performCommandLoop manages the user interface of the game. It takes commands from the user and
 * validates them, displaying an error message when the command is found invalid. The commands are
 * then performed and the game is updated accordingly. After each user turn it checks if the game
 * should be terminated, then it finishes.
 * 
 * @param state		[in, out] a pointer to the current state of the game
 * @return true 	iff the game should be terminated
 */
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

		if ((!parseCommand(commandStr, &command)) || (isGameWon(state) && command.type != RESTART && command.type != EXIT && command.type != IGNORE)){
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

/**
 * runGame starts by initializing the sudoku board and runs the game, exiting when it
 * is finished.
 * 
 * @return true 	iff the game is exited (Rather than: restarted)
 */
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
