#ifndef SOLVER_H_
#define SOLVER_H_

#include "game.h"

/**
 * solvePuzzle is used to solve a given sudoku puzzle board by assigning valid
 * values to its cells, one at a time. The values are selected using the deterministic
 * backtracking algorithm.
 *
 * @param state			[in] current state of the game
 * @param solutionOut 	[in, out] a pointer to a Board struct, to be assigned with a solution
 * 						for the given board
 * @return true 		iff the game in its current state was successfully solved
 * @return false 		iff solving the board has failed
 */
bool solvePuzzle(State* state, Board* solution);

/**
 * generatePuzzle is used to generate a sudoku puzzle board by assingning valid
 * values to its cells, one at a time. The values are selected using the randomized
 * backtracking algorithm.
 *
 * @param board		[in, out] a pointer to a board struct
 * @return true 	iff a board was generated successfully
 * @return false 	iff a board could not be generated
 */
bool generatePuzzle(Board* board);

#endif /* SOLVER_H_ */
