#include "solver.h"

/* TODO: potentially these two functions (randomised vs. deterministic) could be merged somehow to reflect indeed how very similar they are */

/**
 * solvePuzzleRec is used to solve a given sudoku puzzle board by assingning a valid
 * value to a cell with given indices. The values are selected using the deterministic 
 * backtracking algorithm. 
 * 
 * @param solution		[in, out]  a pointer to a Board struct whose cell will be set						
 * @param curRow 		[in] row number of the cell currently being set
 * @param curCol 		[in] column number of the cell currently being set
 * @return true 		iff the halting condition was reached: the board is completely
 * 						filled
 * @return false 		iff there exists no valid value to set in the current cell, and
 * 						its value was set to EMPTY_CELL_VALUE.
 */
bool solvePuzzleRec(Board* solution, int curRow, int curCol) {
	int nextRow = 0, nextCol = 0;
	int value = 0;

	if (curRow == N_SQUARE)
		return true;

	if (curCol == N_SQUARE - 1) {
		nextRow = curRow + 1;
		nextCol = 0;
	} else {
		nextRow = curRow;
		nextCol = curCol + 1;
	}

	if (solution->cells[curRow][curCol].value != EMPTY_CELL_VALUE) {
		return solvePuzzleRec(solution, nextRow, nextCol);
	}

	for (value = 1; value <= N_SQUARE; value++) {
		if (isCellValueValid(solution, curRow, curCol, value)) {
			solution->cells[curRow][curCol].value = value;
			if (solvePuzzleRec(solution, nextRow, nextCol)) {
				return true;
			}
		}
	}
	solution->cells[curRow][curCol].value = EMPTY_CELL_VALUE;
	return false;
}

/**
 * solvePuzzle is a wrapper function solvePuzzleRec used to solve a given sudoku
 * puzzle board. In its recursive calls, generatePuzzleRec will fill the board, employing
 * the deterministic backtracking algorithm, cell by cell, left to right and top to bottom.
 * 
 * @param state			[in] current state of the game 
 * @param solutionOut 	[in, out] a pointer to a Board struct, to be assigned with a solution
 * 						for the given board
 * @return true 		iff the game in its current state was successfully solved
 * @return false 		iff solving the board has failed
 */
bool solvePuzzle(State* state, Board* solutionOut) {
	Board board;
	exportBoard(state, &board);

	if (solvePuzzleRec(&board, 0, 0)) {
		*solutionOut = board;
		return true;
	}
	return false;
}

/**
 * generatePuzzleRec is used to generate a sudoku puzzle board by assingning a valid
 * value to a cell with given indices. The values are selected using the randomized 
 * backtracking algorithm. 
 * 
 * @param board		[in, out] the board currently being filled 
 * @param curRow 	[in] row number of the cell currently being set
 * @param curCol 	[in] column number of the cell currently being set	
 * @return true		iff the halting condition was reached: the board is completely
 * 					filled
 * @return false 	iff there exists no valid value to set in the current cell, and
 * 					its value was set to EMPTY_CELL_VALUE.
 */
bool generatePuzzleRec(Board* board, int curRow, int curCol) {
	int nextRow = 0, nextCol = 0;
	int value = 0;
	int potentialValues[N_SQUARE] = {0};
	int numPotentialValues = 0;

	/* If the board is completely filled */
	if (curRow == N_SQUARE)
		return true;

	if (curCol == N_SQUARE - 1) {
		nextRow = curRow + 1;
		nextCol = 0;
	} else {
		nextRow = curRow;
		nextCol = curCol + 1;
	}

	if (board->cells[curRow][curCol].value != EMPTY_CELL_VALUE) {
		return generatePuzzleRec(board, nextRow, nextCol);
	}

	for (value = 1; value <= N_SQUARE; value++) /* NOTE: could improve complexity of this */
		if (isCellValueValid(board, curRow, curCol, value))
			potentialValues[numPotentialValues++] = value;

	while (numPotentialValues > 0) {
		int chosenIndex = 0;
		if (numPotentialValues > 1) {
			chosenIndex = rand() % numPotentialValues;
		}
		board->cells[curRow][curCol].value = potentialValues[chosenIndex];
		if (generatePuzzleRec(board, nextRow, nextCol)) {
			return true;
		} else {
			/* NOTE: this is the smart way (complexity-wise) of doing this -
			potentialValues[randomIndex] = potentialValues[numPotentialValues--]; */
			int i = 0;
			numPotentialValues--;
			for (i = chosenIndex; i < numPotentialValues; i++)
				potentialValues[i] = potentialValues[i + 1];
		}
	}

	board->cells[curRow][curCol].value = EMPTY_CELL_VALUE;
	return false;
}

/**
 * generatePuzzle is a wrapper function for generatePuzzleRec used to generate
 * a new sudoku puzzle board. In its recursive calls, generatePuzzleRec will fill
 * the board, employing the randomized backtracking algorithm, cell by cell, left
 * to right and top to bottom.
 * 
 * @param board		[in, out] a pointer to a board struct 
 * @return true 	iff generatePuzzleRec successfully 
 * @return false 
 */
bool generatePuzzle(Board* board) {
	return generatePuzzleRec(board, 0, 0);
}
