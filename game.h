#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define N (3)
#define N_SQUARE (N*N)

#define EMPTY_CELL_VALUE (0)

typedef struct {
	int value;
    bool isFixed;} Cell;

typedef struct {
	Cell cells[N_SQUARE][N_SQUARE];} Board;
typedef struct State State;

void exportBoard(State* state, Board* boardOut);

typedef enum setErrorType {
	VALUE_FIXED,
	VALUE_INVALID} SetErrorType;

bool set(State* state, int row, int col, int value, SetErrorType* errorTypeOut);

bool isGameWon(State* state);

int hint(State* state, int row, int col);

bool validate(State* state);

bool initialise(int numCellsToFill, State** stateOut, Board* board);

void destruct(State* state);

bool isCellValueValid(Board* board, int row, int col, int value);

void setCellValue(Board* board, int row, int col, int value);

int getCellValue(Board* board, int row, int col);

void fixCell(Board* board, int row, int col);

bool isCellFixed(Board* board, int row, int col);

#endif /* GAME_H_ */
