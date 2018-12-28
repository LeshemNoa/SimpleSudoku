#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * The dimension of the sudoku board are determined by these constants.
 */
#define N (3)
#define N_SQUARE (N*N)

/**
 * The value defined in this constant would mark an empty cell in the sudoku board.
 */
#define EMPTY_CELL_VALUE (0)

/**
 * Cell struct represents one cell of a sudoku puzzle board. It contains the value
 * in the cell, and a marker determining whether the cell is fixed (immutable) or not.
 */
typedef struct {
	int value;
    bool isFixed;} Cell;

/**
 * Board struct represents a sudoku board.
 */
typedef struct {
	Cell cells[N_SQUARE][N_SQUARE];} Board;
	

/**
 *  State struct represents a sudoku game in its current state.
 *  In order to run a game of sudoku, one must acquire (through the initialise
 *  function) such a struct, and then use it to bring about the actual game.
 */
typedef struct State State;

/**
 * exportBoard given a state struct of a given sudoku game, a representation of the
 * current state of the board game is exported via boardOut.
 *
 * @param state		[in] the State struct of a sudoku game, the current state of the board
 * 					of which is to be exported
 * @param boardOut 	[in, out] a pointer to a Board struct to be assigned with a representation
 * 					of the current state of the board game
 */
void exportBoard(State* state, Board* boardOut);

/**
 * getCellValue returns the value of a particular cell in the sudoku board.
 *
 * @param board 	[in] pointer to the Board struct containing desired cell
 * @param row 		[in] row number of that cell
 * @param col 		[in] column number of that cell
 * @return int 		the integer contained in that cell
 */
int getCellValue(Board* board, int row, int col);

/**
 * isCellValueValid checks the validity of a value assignment in a particular cell. A cell value
 * is valid if that value does not already appear in the row, column or block of the cell in which
 * it's placed OR if it's the exact same the cell already contains.
 *
 * @param board		[in] pointer to the Board struct to be inspected
 * @param row 		[in] number of row of the cell whose value's validity is checked
 * @param col 		[in] number of row of the cell whose value's validity is checked
 * @param value 	[in] value of that cell (which should be within the range [1, N*N])
 * @return true 	iff placement of value in the cell is valid
 * @return false 	iff placement of value in the cell is invalid
 */
bool isCellValueValid(Board* board, int row, int col, int value);

/**
 * setCellValue is used to set the cell with given indices in the given board with the
 * given value.
 *
 * @param board 	[in, out] pointer to the Board struct whose cell is set
 * @param row 		[in] the row number of that cell
 * @param col 		[in] the column number of that cell
 * @param value 	[in] the value to be set in that cell
 */
void setCellValue(Board* board, int row, int col, int value);

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
bool isCellFixed(Board* board, int row, int col);

/**
 * isCellEmpty checks whether a particular cell of a sudoku board is empty or not.
 *
 * @param board		[in] pointer to the Board struct whose cell is checked
 * @param row 		[in] row number of that cell
 * @param col 		[in] column number of that cell
 * @return true 	iff the cell with given indices is empty
 * @return false 	iff the cell with given indices is not empty
 */
bool isCellEmpty(Board* board, int row, int col);

/**
 * emptyCell empties a particular cell in a sudoku board.
 *
 * @param board 	[in, out] pointer to the Board struct whose cell is being emptied
 * @param row 		[in] row number of that cell
 * @param col 		[in] column number of that cell
 */
void emptyCell(Board* board, int row, int col);

/**
 * setErrorType is an enum used to indicate the kinds of errors that could
 * arise from using the function set (below).
 */
typedef enum setErrorType {
	VALUE_FIXED,
	VALUE_INVALID} SetErrorType;


/**
 * set is used to update the game after a 'set' command from the user. First, it checks if
 * the cell the user requested to set has a fixed value. If not, the validity of the given
 * value is checked for that cell. If it's valid, it's set in the cell. If it's invalid, an
 * error is assigned to the provided error pointer (see SetErrorType).
 *
 * @param state 			[in, out] a pointer to the State struct containing the board
 * 							whose cell is being set
 * @param row 				[in] row number of that cell
 * @param col 				[in] column number of that cell
 * @param value 			[in] value the user requested to set in that cell
 * @param errorTypeOut 		[in] a pointer to a SetErrorType variable, to be assigned with a value
 * 							indicating the kind of error that has occurred (in the case that the
 * 							function failed)
 * @return true 			iff the given value has been validated and set in the cel
 * @return false 			iff the cell the user requested to set if fixed, or if the value
 * 							they provided is invalid for that cell
 */
bool set(State* state, int row, int col, int value, SetErrorType* errorTypeOut);

/**
 * isGameWon checks if a sudoku game in its current state is finished by checking if
 * all of its cells are filled.
 *
 * @param state		[in] a pointer to a State struct whose numNonSet attribute will be
 * 					checked
 * @return true 	iff there are no remaining cells to fill in the board
 * @return false 	iff there are still empty cells in the board
 */
bool isGameWon(State* state);

/**
 * hint is used to after a 'hint' command from the user. It returns the stored
 * solution's value of the cell the user has asked for a hint of.
 *
 * @param state 	[in] a pointer to the State struct containing the board
 * 					whose cell is being probed
 * @param row 		[in] row number of that cell
 * @param col 		[in] column number of that cell
 * @return int 		the stored solution's value of the selected cell
 */
int hint(State* state, int row, int col);

/**
 * initialise is used in order to initialise a sudoku game.
 *
 * @param numCellsToFill 	[in] the number of cells which should be fixed
 * @param stateOut 			[in, out] a pointer to a State struct pointer. This
 * 							will be used to pass a valid pointer to a sudoku game
 * 							struct, through which the game being initialised can be handled.
 * @param board 			[in] the puzzle which will be the initial state of the game
 * @return true 			iff the initialisation succeeded
 * @return false 			iff the initialisation failed
 *
 * @note	if initialise succeeded, you must later call destruct with the pointer
 * 			returned through stateOut.
 */
bool initialise(int numCellsToFill, State** stateOut, Board* board);

/**
 * setPuzzleSolution is used to set the stored solution of a sudoku game
 * to some desired board.
 * Canonical use: after a new solution is found to the puzzle.
 * @param state		[in, out] a pointer to the State struct of the game whose stored
 * 					solution is to be set
 * @param solution	[in] a pointer to a new solution for the puzzle
 */
void setPuzzleSolution(State* state, Board* solution);

/**
 * destruct is used to free resourced needed for the game.
 *
 * @param state 	[in] State struct previously acquired through initialise
 */
void destruct(State* state);

#endif /* GAME_H_ */
