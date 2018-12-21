#include "solver.h"

/* TODO: potentially these two functions (randomised vs. deterministic) could be merged somehow to reflect indeed how very similar they are */

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

bool solvePuzzle(State* state, Board* solutionOut) {
	Board board;
	exportBoard(state, &board);

	if (solvePuzzleRec(&board, 0, 0)) {
		*solutionOut = board;
		return true;
	}
	return false;
}

bool generatePuzzleRec(Board* board, int curRow, int curCol) {
	int nextRow = 0, nextCol = 0;
	int value = 0;
	int potentialValues[N_SQUARE] = {0};
	int numPotentialValues = 0;

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
		return solvePuzzleRec(board, nextRow, nextCol);
	}

	for (value = 1; value <= N_SQUARE; value++) /* NOTE: could improve complexity of this */
		if (isCellValueValid(board, curRow, curCol, value))
			potentialValues[numPotentialValues++] = value;

	while (numPotentialValues > 0) {
		int randomIndex = rand() % numPotentialValues;
		board->cells[curRow][curCol].value = potentialValues[randomIndex];
		if (solvePuzzleRec(board, nextRow, nextCol)) {
			return true;
		}
		potentialValues[randomIndex] = potentialValues[numPotentialValues--];
	}

	board->cells[curRow][curCol].value = EMPTY_CELL_VALUE;
	return false;
}

bool generatePuzzle(Board* board) {
	return generatePuzzleRec(board, 0, 0);
}
