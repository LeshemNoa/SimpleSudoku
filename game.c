#include "game.h"

/**
 * State struct represents a sudoku game in its current state. It contains the board itself, a 
 * possible solution for it, and the number of cells left to fill in the board in its current
 * configuration.
 * Note: the implementation of this struct is meant to be hidden from the user.
 */
struct State {
	Board puzzle;
	Board solution;
	int numNonSet;
};

void exportBoard(State* state, Board* boardOut) {
	*boardOut = state->puzzle;
}

/**
 * rowContains checks whether a particular row in the provided board contains a particular
 * value or not.
 * 
 * @param board		[in] pointer to the Board struct to be inspected
 * @param row 		[in] the number of row to check 
 * @param value 	[in] value being checked
 * @return true 	iff row contains value
 * @return false 	iff row does not contain value
 */
bool rowContains(Board* board, int row, int value) {
	int col = 0;
	for (col = 0; col < N_SQUARE; col++)
		if (getCellValue(board, row, col) == value)
			return true;
	return false;
}

/**
 * colContains checks whether a particular column in the provided board contains a particular
 * value or not. 
 * 
 * @param board 	[in] pointer to the Board struct to be inspected
 * @param col 		[in] the number of column to check
 * @param value 	[in] value being checked
 * @return true 	iff column contains value
 * @return false 	iff column does not contain value
 */
bool colContains(Board* board, int col, int value) {
	int row = 0;
	for (row = 0; row < N_SQUARE; row++)
		if (getCellValue(board, row, col) == value)
			return true;
	return false;
}

/**
 * whichBlock uses a linear mapping to find which block in the board a cell with given
 * indices belongs to. The board blocks are of size N*N and there are N of them in total,
 * numbered [0 - N-1], from left to right and top to bottom.
 * 
 * @param row		[in] the number of row the cell is in
 * @param col 		[in] the number of column the cell is in
 * @return int		the number of block the cell is in 
 */
int whichBlock(int row, int col) {
	return ((row / N) * N) + (col / N);
}

/**
 * blockContains checks whether a particular block in the provided board contains a particular
 * value or not. 
 * 
 * @param board 	[in] pointer to the Board struct to be inspected
 * @param block 	[in] the number of block to check
 * @param value 	[in] value being checked
 * @return true 	iff block contains value
 * @return false 	iff block does not contain value
 */
bool blockContains(Board* board, int block, int value) {
	int rowOffset = (block / N) * N;
	int colOffset = (block % N) * N;

	int row = 0, col = 0;
	for (row = 0; row < N; row++)
		for (col = 0; col < N; col++)
			if (getCellValue(board, rowOffset + row, colOffset + col) == value)
				return true;
	return false;
}

int getCellValue(Board* board, int row, int col) {
	return board->cells[row][col].value;
}

bool isCellValueValid(Board* board, int row, int col, int value) {
	return (getCellValue(board, row, col) == value) ||
		   (!rowContains(board, row, value) &&
		    !colContains(board, col, value) &&
		    !blockContains(board, whichBlock(row, col), value));
}

void setCellValue(Board* board, int row, int col, int value) {
	board->cells[row][col].value = value;
}

/**
 * fixCell is used during the generation of a new sudoku board to make a cell fixed.
 * 
 * @param board		[in] pointer to the Board struct whose cell is set to be fixed  
 * @param row 		[in] the row number of that cell
 * @param col 		[in] the column number of that cell
 */
void fixCell(Board* board, int row, int col) {
	board->cells[row][col].isFixed = true;
}

bool isCellFixed(Board* board, int row, int col) {
	return board->cells[row][col].isFixed;
}

bool isCellEmpty(Board* board, int row, int col) {
	return getCellValue(board, row, col) == EMPTY_CELL_VALUE;
}

void emptyCell(Board* board, int row, int col) {
	setCellValue(board, row, col, EMPTY_CELL_VALUE);
}

/**
 * setPuzzleCell sets a value provided by the user to a cell in the sudoku board, in the 
 * current state of the game. Number of cells to be set is updated if need be.
 *
 * @param state 	[in] pointer to the Board struct whose cell is set to be set
 * @param row 		[in] the row number of that cell
 * @param col 		[in] the column number of that cell
 * @param value 	[in] the value to be set to that cell
 */
void setPuzzleCell(State* state, int row, int col, int value) {
	if (isCellEmpty(&(state->puzzle), row, col)) {
		state->numNonSet--;
	}
	setCellValue(&(state->puzzle), row, col, value);
}

/**
 * emptyPuzzleCell empties a cell in the sudoku board, in the current state of the game.
 * Number of cells to be set is updated if need be.
 *
 * @param state 	[in] pointer to the Board struct whose cell is set to be emptied
 * @param row 		[in] the row number of that cell
 * @param col 		[in] the column number of that cell
 */
void emptyPuzzleCell(State* state, int row, int col) {
	if (!isCellEmpty(&(state->puzzle), row, col)) {
		state->numNonSet++;
	}
	emptyCell(&(state->puzzle), row, col);
}

bool set(State* state, int row, int col, int value, SetErrorType* errorTypeOut) {
	if (isCellFixed(&(state->puzzle), row, col)) {
		*errorTypeOut = VALUE_FIXED;
		return false;
	}
	if (value != EMPTY_CELL_VALUE) {
		if (! isCellValueValid(&(state->puzzle), row, col, value)) {
			*errorTypeOut = VALUE_INVALID;
			return false;
		} else {
			setPuzzleCell(state, row, col, value);
		}
	} else {
		emptyPuzzleCell(state, row, col);
	}

	return true;
}

bool isGameWon(State* state) {
	return state->numNonSet == 0;
}

int hint(State* state, int row, int col) {
	return getCellValue(&(state->solution), row, col);
}

/**
 * randomlyFixCells is to be used in ordered to randomly mark a certain
 * amount of cells in a given board as fixed.
 * Canonical use of this function: generation of a puzzle board.
 * 
 * @param board				[in, out] the board whose cells are to be fixed
 * @param numCellsToFix 	[in] the number of cells to fix (should be not more
 * 							than the number of cells in board)
 */
void randomlyFixCells(Board* board, int numCellsToFix) {
	int fixCount = 0;
	while (fixCount < numCellsToFix) {
		int col = rand() % N_SQUARE;
		int row = rand() % N_SQUARE;

		if (! isCellFixed(board, row, col)) {
			fixCell(board, row, col);
			fixCount++;
		}
	}
}

/**
 * clearNonFixedCells is used during the puzzle generation process. After the fixed cells
 * in the new board have been selected, clearNonFixed cells empties each cell which haven't
 * been fixed to create the puzzle. 
 * 
 * @param board		[in, out] a pointer to the Board struct of which the non fixed cells
 * 					are to be cleared 
 */
void clearNonFixedCells(Board* board) {
	int row = 0, col = 0;
	for (row = 0; row < N_SQUARE; row++)
		for (col = 0; col < N_SQUARE; col++)
			if (! isCellFixed(board, row, col))
				emptyCell(board, row, col);
}

bool initialise(int numCellsToFill, State** stateOut, Board* board) {
	*stateOut = calloc(1, sizeof(State));
	if (*stateOut == NULL) {
		/* NOTE: for now we are allowed this behaviour: */
		printf("Error: initialise has failed\n");
		exit(EXIT_FAILURE);
		return false;
	}

	(*stateOut)->puzzle = *board;

	randomlyFixCells(&((*stateOut)->puzzle), numCellsToFill);

	(*stateOut)->solution = (*stateOut)->puzzle;

	clearNonFixedCells(&((*stateOut)->puzzle));

	(*stateOut)->numNonSet = N_SQUARE * N_SQUARE - numCellsToFill;

	return true;
}

void setPuzzleSolution(State* state, Board* solution) {
	state->solution = *solution;
}

void destruct(State* state) {
	if (state != NULL) {
		free(state);
	}
}
