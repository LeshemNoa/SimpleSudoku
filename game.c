#include "game.h"

#define UNUSED(x) (void)(x)

/**
 * State struct represents a sudoku game in its current state. It contains the board itself, a 
 * possible solution for it, and the number of cells left to fill in the board in its current
 * configuration. 
 */
struct State {
	Board puzzle;
	Board solution;
	int numNonSet;
};

/**
 * exportBoard assigns a copy (?) of the current Board struct of a given state to the provided
 * pointer.
 * 
 * @param state		[in] the State struct whose board is exported 
 * @param boardOut 	[in, out] a pointer to a Board struct to be assigned with a copy (??) of the 
 * 					state's current board
 */
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
		if (board->cells[row][col].value == value)
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
		if (board->cells[row][col].value == value)
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
			if (board->cells[rowOffset + row][colOffset + col].value == value)
				return true;
	return false;
}

/**
 * isCellValueValid checks the validity of a value assignment in a particular cell. A cell value
 * is valid if that value does nto already appear in the row, column or block of the cell in which
 * it's placed.
 * 
 * @param board		[in] pointer to the Board struct to be inspected		
 * @param row 		[in] number of row of the cell whose value's validity is checked
 * @param col 		[in] number of row of the cell whose value's validity is checked
 * @param value 	[in] value of that cell
 * @return true 	iff value is a valid non-zero integer and its placement in the cell is 
 * 					valid
 * @return false 	iff the value is 0 indicating that the cell is empty, or its placement
 * 					in the cell is invalid
 */
bool isCellValueValid(Board* board, int row, int col, int value) {
	return (getCellValue(board, row, col) == value) ||
		   (!rowContains(board, row, value) &&
		    !colContains(board, col, value) &&
		    !blockContains(board, whichBlock(row, col), value));
}

/**
 * setCellValue is used to set the cell with given indices in the given board with the 
 * given value.
 * 
 * @param board 	[in, out] pointer to the Board struct whose cell is set
 * @param row 		[in] the row number of that cell
 * @param col 		[in] the column number of that cell
 * @param value 	[in] the value to be set in that cell
 */
void setCellValue(Board* board, int row, int col, int value) {
	board->cells[row][col].value = value;
	/*UNUSED(board); TODO: try to deal with this */
}

/**
 * getCellValue returns the value of a particular cell in the sudoku board.
 * 
 * @param board 	[in] pointer to the Board struct containing desired cell
 * @param row 		[in] row number of that cell
 * @param col 		[in] column number of that cell
 * @return int 		the integer contained in that cell
 */
int getCellValue(Board* board, int row, int col) {
	return board->cells[row][col].value;
}

/**
 * fixCell is used during the generation of a new sudoku board to make a cell fixed, making it 
 * one of the sudoku board hints.  
 * 
 * @param board		[in] pointer to the Board struct whose cell is set to be fixed  
 * @param row 		[in] the row number of that cell
 * @param col 		[in] the column number of that cell
 */
void fixCell(Board* board, int row, int col) {
	board->cells[row][col].isFixed = true;
	/*UNUSED(board); TODO: try to deal with this */
}

/**
 * isCellFixed checks whether a particular cell of a sudoku board is a fixed cell or 
 * not. 
 * 
 * @param board		[in] pointer to the Board struct whose cell is checked 
 * @param row 		[in] row number of that cell
 * @param col 		[in] column number of that cell
 * @return true 	iff the cell with given indices is fixed
 * @return false 	iff the cell with given indices is not fixed
 */
bool isCellFixed(Board* board, int row, int col) {
	return board->cells[row][col].isFixed;
}

/**
 * isCellEmpty checks whether a particular cell of a sudoku board is empty or not.
 * 
 * @param board		[in] pointer to the Board struct whose cell is checked 
 * @param row 		[in] row number of that cell
 * @param col 		[in] column number of that cell
 * @return true 	iff the cell with given indices is empty
 * @return false 	iff the cell with given indices is not empty
 */
bool isCellEmpty(Board* board, int row, int col) {
	return getCellValue(board, row, col) == EMPTY_CELL_VALUE;
}

/**
 * emptyCell empties a particular cell in a sudoku board. 
 * 
 * @param board 	[in, out] pointer to the Board struct whose cell is being emptied
 * @param row 		[in] row number of that cell
 * @param col 		[in] column number of that cell
 */
void emptyCell(Board* board, int row, int col) {
	setCellValue(board, row, col, EMPTY_CELL_VALUE);
}

/**
 * TODO: is this function duplicate? It's never called by anyone and only setCellValue is used.
 * 
 * 
 * setPuzzleCell sets a value provided by the user to a cell in the sudoku board, in the 
 * current state of the game. 
 * 
 * @param state 
 * @param row 
 * @param col 
 * @param value 
 */
void setPuzzleCell(State* state, int row, int col, int value) {
	if (isCellEmpty(&(state->puzzle), row, col)) {
		state->numNonSet--;
	}
	setCellValue(&(state->puzzle), row, col, value);
}

/**
 * set is used to update the game after a 'set' command from the user. First, it checks if
 * the cell the user requested to set has a fixed value. If not, the validity of the given
 * value is checked for that cell. If it's valid, it's set in the cell. If it's invalid, an
 * error is assigned to the provided error pointer.
 * 
 * @param state 			[in, out] a pointer to the State struct containing the board  
 * 							whose cell is being set
 * @param row 				[in] row number of that cell
 * @param col 				[in] column number of that cell
 * @param value 			[in] value the user requested to set in that cell
 * @param errorTypeOut 		[in] a pointer to a SetErrorType object, to be assigned with
 * 							a VALUE_INVALID error in case the value the user provided is invalid
 * @return true 			iff the given value has been validated and set in the cel
 * @return false 			iff the cell the user requested to set if fixed, or if the value
 * 							they provided is invalid for that cell
 */
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
			if (isCellEmpty(&(state->puzzle), row, col)) {
				state->numNonSet--;
			}
			setCellValue(&(state->puzzle), row, col, value);
		}
	} else {
		if (!isCellEmpty(&(state->puzzle), row, col)) {
			state->numNonSet++;
		}
		emptyCell(&(state->puzzle), row, col);
	}

	return true;
}

/**
 * isGameWon checks if a sudoku game in its current state is finished by checking if 
 * all of its cells are filled.
 * 
 * @param state		[in] a pointer to a State struct whose numNonSet attribute will be
 * 					checked 
 * @return true 	iff there are no remaining cells to fill in the board
 * @return false 	iff there are still empty cells in the board
 */
bool isGameWon(State* state) {
	return state->numNonSet == 0;
}

/**
 * @brief 
 * 
 * @param state 
 * @param row 
 * @param col 
 * @return int 
 */
int hint(State* state, int row, int col) {
	return getCellValue(&(state->solution), row, col);
}

/**
 * @brief 
 * 
 * @param board 
 * @param numCellsToFix 
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
 * in the new board have been selected, clearNonFixed cells empties each cell which hasn't
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
				setCellValue(board, row, col, EMPTY_CELL_VALUE);
}

/**
 * 
 * 
 * @param numCellsToFill 
 * @param stateOut 
 * @param board 
 * @return true 
 * @return false 
 */
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

/**
 * destruct is used to free memomy of a State struct.
 * 
 * @param state 	[in] State struct to free memory of
 */
void destruct(State* state) {
	if (state != NULL) {
		free(state);
	}
}
