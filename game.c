#include "game.h"

#define UNUSED(x) (void)(x)

struct State {
	Board puzzle;
	Board solution;
	int numNonSet;
};

void exportBoard(State* state, int board[N_SQUARE][N_SQUARE]) {
	int row = 0, col = 0;

	for (row = 0; row < N_SQUARE; row++)
		for (col = 0; col < N_SQUARE; col++)
			board[row][col] = state->puzzle.cells[row][col].value;
}

bool rowContains(Board board, int row, int value) {
	int col = 0;
	for (col = 0; col < N_SQUARE; col++)
		if (board.cells[row][col].value == value)
			return true;
	return false;
}

bool colContains(Board board, int col, int value) {
	int row = 0;
	for (row = 0; row < N_SQUARE; row++)
		if (board.cells[row][col].value == value)
			return true;
	return false;
}

int whichBlock(int row, int col) {
	return ((row / N) * N) + (col / N);
}

bool blockContains(Board board, int block, int value) {
	int rowOffset = (block / N) * N;
	int colOffset = (block % N) * N;

	int row = 0, col = 0;
	for (row = 0; row < N; row++)
		for (col = 0; col < N; col++)
			if (board.cells[rowOffset + row][colOffset + col].value == value)
				return true;
	return false;

}

bool isCellValueValid(Board board, int row, int col, int value) {
	return !rowContains(board, row, value) &&
		   !colContains(board, col, value) &&
		   !blockContains(board, whichBlock(row, col), value);
}

void setCellValue(Board board, int row, int col, int value) {
	board.cells[row][col].value = value;
	UNUSED(board); /* TODO: try to deal with this */
}

int getCellValue(Board board, int row, int col) {
	return board.cells[row][col].value;
}

void setCellFix(Board board, int row, int col, bool isFixed) { /* TODO: what is this good for?, perhaps we want a fixCell function, but surely not here - it'll be in solver, which is entrusted with creating a board from scratch */
	board.cells[row][col].isFixed = isFixed;
	UNUSED(board); /* TODO: try to deal with this */
}
bool isCellFixed(Board board, int row, int col) {
	return board.cells[row][col].isFixed;
}

bool set(State* state, int row, int col, int value, SetErrorType* errorTypeOut) {
	if (isCellFixed(state->puzzle, row, col)) {
		*errorTypeOut = VALUE_FIXED;
		return false;
	}
	if (! isCellValueValid(state->puzzle, row, col, value)) {
		*errorTypeOut = VALUE_INVALID;
		return false;
	}

	setCellValue(state->puzzle, row, col, value);
	return true;
}

int hint(State* state, int row, int col) {
	return getCellValue(state->puzzle, row, col);
}

bool initialise(int numCellsToFill, State* stateOut) {
	UNUSED(numCellsToFill);
	UNUSED(stateOut);
	return true; /*TODO*/
}

/*bool getNextNonSetCell(State* state, int* row, int* col);  TODO: should be in solver */

/*bool validateRow(State* state, int row); TODO: should be in solver */
/*bool validateCol(State* state, int col);  TODO: should be in solver */
/*bool validateBlock(State* state, int block);  TODO: should be in solver */

/*bool validateGame(State* state);  TODO: should activate a validation function from solver */
