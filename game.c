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

Board* createBoard() {
    Board* b = (Board*)calloc(1, sizeof(Board));
    return b;
}

void destroyBoard(Board* b) {
    free(b);
}

/* cloneBoard
Creates a deep copy of a given board and returns a pointer to it.
*/
Board* cloneBoard(Board* b) {
    int i, j;
    Board* clone = createBoard();
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

void setCellValue(Board* b, int i, int j, int val) {
    b->cell[i][j].val = val;
}

void setCellFix(Board* b, int i, int j, boolean fix) {
    b->cell[i][j].isFixed = fix;
}

int getCellValue(Board* b, int i, int j) {
    return b->cell[i][j].val;
}

boolean getCellFix(Board* b, int i, int j) {
    return b->cell[i][j].isFixed;
}

/* nextEmptyCell 
Returns true if exists an empty cell in the board, the indices of the 
first one found are updated in the given pointers. Returns false if the board
is full.  */
boolean nextEmptyCell(Board* b, int* row, int* col) {
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
    return 0;   
}

/* validateRegion
Checks thst no digit is repeated in the region b[i][j] where rStart <= i <= rEnd
and cStart <= j <= cEnd */

boolean validateRegion(Board* b, int rStart, int rEnd, int cStart, int cEnd) {
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

boolean validateRow(Board* b, int i) {
    return validateRegion(b, i, i, 0, DIM - 1);    
}

boolean validateCol(Board* b, int j) {
    return validateRegion(b, 0, DIM - 1, j, j);    
}

/* validateBlock

Gets the number of a block in the boar (0-9) and calculates (i,j) index of 
the upper left cell in the block, using the linear mapping 
blockNum -> ( 3*floor(blockNum/3), 3*(blockNum % 3). 

Using this index, it calculates the other 2 missing parameters for validateRegion.
*/
boolean validateBlock(Board* b, int blockNum) {
    int rStart = 3 * (blockNum / 3), cStart = 3 * (blockNum % 3);
    return validateRegion(b, rStart, rStart+2, cStart, cStart+2);
}

/* whichBlock 
We number the block of the board 0-9, right to left and top to bottom.
Given an index (i,j) in the board, we find which block it belongs to using the linear
mapping (i,j) -> floor(j/3) + 3*floor(i/3). 
*/
int whichBlock(int i, int j) {
    return (j/3) + (i/3)*3;
}

boolean validateGame(Board* b) {
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




