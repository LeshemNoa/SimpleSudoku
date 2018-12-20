#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>

#define N (3)
#define N_SQUARE (N*N)

typedef struct {
	int value;
    bool isFixed;} Cell; /* VISIBILE: probably not */ /* Also move to solver */

typedef struct {
	Cell cells[N_SQUARE][N_SQUARE];} Board; /* TODO: this will be part of the State struct
											 VISIBILE: no
											 move to solver */
typedef struct State State;

void exportBoard(State* state, int board[N_SQUARE][N_SQUARE]);

typedef enum setErrorType {
	VALUE_FIXED,
	VALUE_INVALID} SetErrorType;

bool set(State* state, int row, int col, int value, SetErrorType* errorTypeOut);

int hint(State* state, int row, int col);

/*bool validate(State* state);

bool restart(State* state);

bool exit(State* state); // TODO: choose better (not overriding) name

bool initialise(int numCellsToFill, State* stateOut);

bool destruct(State* state); // TODO: check if needs bool or void is good enough */

#endif /* GAME_H_ */
