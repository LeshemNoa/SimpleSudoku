#include "solver.h"

/**
 * solvePuzzleRec is a recursive function (to be called by solvePuzzle). It is used to
 * solve a given sudoku puzzle board. In its recursive calls, generatePuzzleRec will
 * fill the board, employing the deterministic backtracking algorithm, cell by cell,
 * left to right and top to bottom.
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

	if (! isCellEmpty(solution, curRow, curCol)) {
		return solvePuzzleRec(solution, nextRow, nextCol);
	}

	for (value = 1; value <= N_SQUARE; value++) {
		if (isCellValueValid(solution, curRow, curCol, value)) {
			setCellValue(solution, curRow, curCol, value);
			if (solvePuzzleRec(solution, nextRow, nextCol)) {
				return true;
			}
		}
	}
	emptyCell(solution, curRow, curCol);
	return false;
}

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
 * generatePuzzleRec is a recursive function (to be called by generatePuzzle).
 * It is used to generate a new sudoku puzzle board. In its recursive calls,
 * generatePuzzleRec will fill the board, employing the randomized backtracking
 * algorithm, cell by cell, left to right and top to bottom.
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

	if (! isCellEmpty(board, curRow, curCol)) {
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
		setCellValue(board, curRow, curCol, potentialValues[chosenIndex]);
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

	emptyCell(board, curRow, curCol);
	return false;
}

bool generatePuzzle(Board* board) {
	return generatePuzzleRec(board, 0, 0);
}

/* Note: potentially those two functions (randomised vs. deterministic) could be
 * merged somehow to reflect indeed how very similar they are */
