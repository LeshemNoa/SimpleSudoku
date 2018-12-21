#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

#include <stdbool.h>
#include <stdio.h>

#include "game.h"
#include "parser.h"
#include "solver.h"

#define COMMAND_MAX_LENGTH (1024)

#define CELL_SIZE_IN_PRINT (3)
#define BLOCK_OVERHEAD_SIZE_IN_PRINT (2)
#define LINE_OVERHEAD_SIZE_IN_PRINT (1)

void printSeparatorLine();

void printLine(Board* board, int lineIndex);

void printBoard(Board* board);

bool isNumCellsToFillValid(int numCellsToFill);

bool getNumCellsToFill(int* numCellsToFillOut);

bool initialStage(State** state);

bool getCommandString(char* commandStrOut, int commandMaxSize);

void performSetCommand(State* state, SetCommandArguments* args);

void performHintCommand(State* state, HintCommandArguments* args);

void performValidateCommand(State* state);

void performCommand(State* state, Command* command, bool* shouldRestart, bool* shouldExit);

bool performCommandLoop(State* state);

bool runGame();

#endif /* MAIN_AUX_H_ */
