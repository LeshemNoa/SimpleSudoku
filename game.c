#include "game.h"

#define UNUSED(x) (void)(x)

struct State {
	Board puzzle;
	Board solution;
	int numNonSet;
};

void exportBoard(State* state, Board* boardOut) {
	*boardOut = state->puzzle;
}

bool rowContains(Board* board, int row, int value) {
	int col = 0;
	for (col = 0; col < N_SQUARE; col++)
		if (board->cells[row][col].value == value)
			return true;
	return false;
}

bool colContains(Board* board, int col, int value) {
	int row = 0;
	for (row = 0; row < N_SQUARE; row++)
		if (board->cells[row][col].value == value)
			return true;
	return false;
}

int whichBlock(int row, int col) {
	return ((row / N) * N) + (col / N);
}

bool blockContains(Board* board, int block, int value) {
	int rowOffset = (block / N) * N;
	int colOffset = (block % N) * N;

	int row = 0, col = 0;
	for (row = 0; row < N; row++)
		for (col = 0; col < N; col++)
			if (board->cells[rowOffset + row][colOffset + col].value == value)
				return true;
	return false;
}

bool isCellValueValid(Board* board, int row, int col, int value) {
	return !rowContains(board, row, value) &&
		   !colContains(board, col, value) &&
		   !blockContains(board, whichBlock(row, col), value);
}

void setCellValue(Board* board, int row, int col, int value) {
	board->cells[row][col].value = value;
	/*UNUSED(board); TODO: try to deal with this */
}

int getCellValue(Board* board, int row, int col) {
	return board->cells[row][col].value;
}

void fixCell(Board* board, int row, int col) {
	board->cells[row][col].isFixed = true;
	/*UNUSED(board); TODO: try to deal with this */
}
bool isCellFixed(Board* board, int row, int col) {
	return board->cells[row][col].isFixed;
}

bool set(State* state, int row, int col, int value, SetErrorType* errorTypeOut) {
	if (isCellFixed(&(state->puzzle), row, col)) {
		*errorTypeOut = VALUE_FIXED;
		return false;
	}
	if (! isCellValueValid(&(state->puzzle), row, col, value)) {
		*errorTypeOut = VALUE_INVALID;
		return false;
	}

	setCellValue(&(state->puzzle), row, col, value);
	state->numNonSet--;

	return true;
}

bool isGameWon(State* state) {
	return state->numNonSet == 0;
}

int hint(State* state, int row, int col) {
	return getCellValue(&(state->solution), row, col);
}

void randomlyFixCells(Board* board, int numCellsToFix) {
	int fixCount = 0;
	while (fixCount < numCellsToFix) {
		int row = rand() % N_SQUARE;
		int col = rand() % N_SQUARE;

		if (! isCellFixed(board, row, col)) {
			fixCell(board, row, col);
			fixCount++;
		}
	}
}

void clearNonFixedCells(Board* board) {
	int row = 0, col = 0;
	for (row = 0; row < N_SQUARE; row++)
		for (col = 0; col < N_SQUARE; col++)
			if (! isCellFixed(board, row, col))
				setCellValue(board, row, col, EMPTY_CELL_VALUE);
}

bool initialise(int numCellsToFill, State** stateOut, Board* board) {
	*stateOut = calloc(1, sizeof(State));
	if (*stateOut == NULL) {
		return false;
	}

	(*stateOut)->puzzle = *board;

	randomlyFixCells(&((*stateOut)->puzzle), numCellsToFill);

	(*stateOut)->solution = (*stateOut)->puzzle;

	clearNonFixedCells(&((*stateOut)->puzzle));

	(*stateOut)->numNonSet = N_SQUARE * N_SQUARE - numCellsToFill;

	return true;
}

void destruct(State* state) {
	if (state != NULL) {
		free(state);
	}
}

/*bool getNextNonSetCell(State* state, int* row, int* col);  TODO: should be in solver */

/*bool validateRow(State* state, int row); TODO: should be in solver */
/*bool validateCol(State* state, int col);  TODO: should be in solver */
/*bool validateBlock(State* state, int block);  TODO: should be in solver */

/*bool validateGame(State* state);  TODO: should activate a validation function from solver */
