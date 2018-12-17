#include "game.h"
#include <stdio.h>
#include <stdlib.h>

#define SEP_LINE "----------------------------------\n"
/* SET_BIT
Uses bitwise operations to set the bit of the provided index in the integer field.
*/
#define SET_BIT(field, index) (field |= (1 << (index)))
/* IS_BIT_SET
Uses bitwise opeartions to check whether the bit of the provided index in field is set.
*/
#define IS_BIT_SET(field, index) ((field >> (index)) & 1)

// TODO: this should be relegated to main_aux. main should retrieve the board from game (note main shouldn't have access to the internal structure of the game) and pass it on to main_aux for printing.
// TODO: split to functions (printRow(rowNo) printSeparatorRow())
void printBoard(Board* b) {
    int i, j;
    for (i = 0; i < DIM; i++) {

        if (i % 3 == 0) {
            printf(SEP_LINE);
        }

        for (j = 0; j < DIM; j++) {

            Cell* cell = &b->cell[i][j];

            /* Print left column delimiter */
            if (j % 3 == 0) {
                printf("| ");
            }

            /* Print cell contents */
            printf(cell->isFixed ? "." : " ");

            if (cell->val == 0) {
                printf(" ");
            } else {
                printf("%d", cell->val);
            }

            printf(" ");
        }
        /* End row with right delimiter */
        printf("|\n");

    }
    printf(SEP_LINE);
}

Board* createBoard() { // TODO: this will need to initialise the entire state (a member of which should of course be the board).
    Board* b = (Board*)calloc(1, sizeof(Board)); // TODO: always check the returned value of a function.
    											 // TODO: it would perhaps be better to return the pointer via an argument, and use the returned value as an indicator for success (as a rule!). This approach usually greatly relieves the user (here, main.h) from unnecessary burden.
    return b;
}

void destroyBoard(Board* b) { // TODO: what if b is NULL?
    free(b);
}

/* cloneBoard
Creates a deep copy of a given board and returns a pointer to it.
*/
Board* cloneBoard(Board* b) {
    int i, j;
    Board* clone = createBoard(); // TODO: supposedly the user is to use createBoard (soon to be: initialise) just ONCE. I don't see why it should be used from the inside. If the idea is to copy the board before "cleaning" it, then it should be done through a wholly-internal function.
    if (clone == NULL) {
        return NULL;
    }
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            clone->cell[i][j].val = b->cell[i][j].val;
            clone->cell[i][j].isFixed = b->cell[i][j].isFixed;
        }
    } return clone;
}

void setCellValue(Board* b, int i, int j, int val) { // TODO: in general, it's better to have the arguments' names reflect best their meanings. here 'i' should be renamed 'row', and 'j' - 'column' (or the abbreviated form 'col')
    b->cell[i][j].val = val;
}

void setCellFix(Board* b, int i, int j, bool fix) {
    b->cell[i][j].isFixed = fix;
}

int getCellValue(Board* b, int i, int j) {
    return b->cell[i][j].val;
}

bool getCellFix(Board* b, int i, int j) {
    return b->cell[i][j].isFixed;
}

// TODO: we should establish some formal way of documenting our functions.

/* nextEmptyCell
Returns true if exists an empty cell in the board, the indices of the
first one found are updated in the given pointers. Returns false if the board
is full.  */
bool nextEmptyCell(Board* b, int* row, int* col) {
    int i, j;
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            if (b->cell[i][j].val == 0) {
                *row = i;
                *col = j;
                return 1;
            }
        }
    }
    return 0; // TODO: now that we have stdbool.h, we can use the constant false here (and true, where it's appropriate).
}

/* validateRegion
Checks that no digit is repeated in the region b[i][j] where rStart <= i <= rEnd
and cStart <= j <= cEnd */

bool validateRegion(Board* b, int rStart, int rEnd, int cStart, int cEnd) { // TODO: using bit operations feels like somewhat of an overkill to me. We have no memory shortage (especially given that each checked block will be of a very much constant size). It is a very neat idea though, and we can keep it if you so desire.
																			//		 a slight problem arises when one considers that this system only allows regions of sizes up to 32/64 (based on sizeof(int)). This seems like an assumption we cannot make, since the size of the board is reportedly up to the user. So after due consideration, I think we should change the implementation of this function.
    int bitField = 0;
    int i, j;

    for (i = rStart; i <= rEnd; i++) {
        for (j = cStart; j <= cEnd; j++) {
            Cell* cell = &b->cell[i][j];
            if (cell->val != 0) {
                if (IS_BIT_SET(bitField, cell->val -1)) {
                    return 0;
                }
                SET_BIT(bitField, cell->val -1);
            }
        }
    }
    return 1;
}

bool validateRow(Board* b, int i) {
    return validateRegion(b, i, i, 0, DIM - 1);
}

bool validateCol(Board* b, int j) {
    return validateRegion(b, 0, DIM - 1, j, j);
}

/* validateBlock

Gets the number of a block in the boar (0-9) and calculates (i,j) index of
the upper left cell in the block, using the linear mapping
blockNum -> ( 3*floor(blockNum/3), 3*(blockNum % 3).

Using this index, it calculates the other 2 missing parameters for validateRegion.
*/
bool validateBlock(Board* b, int blockNum) { // TODO: this MUST use the DIM (or N) constant!!! (and for now, I didn't check the correctness of the calculations)
    int rStart = 3 * (blockNum / 3), cStart = 3 * (blockNum % 3);
    return validateRegion(b, rStart, rStart+2, cStart, cStart+2);
}

/* whichBlock
We number the block of the board 0-9, right to left and top to bottom.
Given an index (i,j) in the board, we find which block it belongs to using the linear
mapping (i,j) -> floor(j/3) + 3*floor(i/3).
*/
int whichBlock(int i, int j) { // TODO: this MUST use the DIM (or N) constant!!!
    return (j/3) + (i/3)*3;
}

bool validateGame(Board* b) {
    int i, j, k;
    for(i = 0; i < DIM; i++) {
        if (!validateRow(b, i)){
            return 0;
        }
    }
    for(j = 0; j < DIM; j++) {
        if (!validateCol(b, j)){
            return 0;
        }
    }
    for(k = 0; k < DIM; k++) {
        if (!validateBlock(b, k)) {
            return 0;
        }
    }
    return 1;
}




