#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h> // TODO: removed your 'boolean' macro and replaced it with functionality from this library.

// TODO: check which of the elements here should be visible to an outsider

#define DIM (9) // TODO: consider changing to N (3)
				// VISIBILE: yes

// TODO: we probably need a STATE struct
// TODO: it may be useful to keep here the number of slots that need solving
// TODO: we need to keep a copy of the solved board

typedef struct {int val;
    			bool isFixed;} Cell; // VISIBILE: probably not

typedef struct {Cell cell[DIM][DIM];} Board; // TODO: this will be part of the State struct
											 // VISIBILE: no

void printBoard(Board* b); // TODO: printing is done via main_aux. We would need a function to export the state of the board.
Board* createBoard(); // TODO: rename 'initialise'
					  // VISIBILE: yes
Board* cloneBoard(Board* b); //TODO: what is the need for this?

void destroyBoard(Board* b); // TODO: rename 'destruct'
							 // VISIBILE: YES

void setCellValue(Board* b, int i, int j, int val); // VISIBILE: yesS (should probably be invisible, but be used by a visible set function)
void setCellFix(Board* b, int i, int j, bool fix); // VISIBLE: no
int getCellValue(Board* b, int i, int j); // VISIBILE: yes (should probably be invisible, but be used by a visible getHint function)
bool getCellFix(Board* b, int i, int j); // VISIBLE: no

bool nextEmptyCell(Board* b, int* row, int* col); // VISIBLE: no

bool validateRow(Board* b, int i); // VISIBLE: no
bool validateCol(Board* b, int j); // VISIBLE: no
bool validateBlock(Board* b, int blockNum); // VISIBLE: no
bool validateGame(Board* b); // VISIBLE: yes
int whichBlock(int i, int j); // VISIBLE: no


#endif /* GAME_H_ */
