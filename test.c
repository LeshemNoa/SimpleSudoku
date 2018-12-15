#include <stdio.h>
#include "solver.h"

void testValidateRow() {
    int i;
    Board* b = createBoard();
    for (i = 0; i < DIM; i++) {
        b->cell[0][i].val = i+1;
        if (! validateRow(b, 0)) {
           printf("Unexpected invalid row\n");
           printBoard(b);
           break;
        }
    }
    destroyBoard(b);

    b = createBoard();
    b->cell[0][1].val = 1;
    b->cell[0][8].val = 1;
    if (validateRow(b, 0)) {
        printf("Unexpected valid row\n");
        printBoard(b);
    }
    destroyBoard(b);    
}

void testValidateCol() {
    int i;
    Board* b = createBoard();
    for (i = 0; i < DIM; i++) {
        b->cell[i][0].val = i+1;
        if (! validateCol(b, 0)) {
           printf("Unexpected invalid col\n");
           printBoard(b);
           break;
        }
    }
    destroyBoard(b);

    b = createBoard();
    b->cell[1][0].val = 1;
    b->cell[8][0].val = 1;
    if (validateCol(b, 0)) {
        printf("Unexpected valid col\n");
        printBoard(b);
    }
    destroyBoard(b);    
}

void testValidateBlock() {
    int i,j;
    Board* b = createBoard();
    for (i=0; i<DIM; i++) {
        for (j=0; j<DIM; j++) {
            b->cell[i][j].val = 1;
        }
    }
    b->cell[0][0].val = 1;
    b->cell[0][1].val = 2;
    b->cell[0][2].val = 3;

    b->cell[1][0].val = 4;
    b->cell[1][1].val = 5;
    b->cell[1][2].val = 6;

    b->cell[2][0].val = 7;
    b->cell[2][1].val = 8;
    b->cell[2][2].val = 9;

    if (!validateBlock(b, 0)) {
        printf("Unexpected invalid block\n");
        printBoard(b);
    }
    destroyBoard(b);

    /********/
    b = createBoard();
    for (i=0; i<DIM; i++) {
        for (j=0; j<DIM; j++) {
            b->cell[i][j].val = 1;
        }
    }
    b->cell[6][6].val = 1;
    b->cell[6][7].val = 2;
    b->cell[6][8].val = 3;

    b->cell[7][6].val = 4;
    b->cell[7][7].val = 5;
    b->cell[7][8].val = 1;

    b->cell[8][6].val = 7;
    b->cell[8][7].val = 2;
    b->cell[8][8].val = 9;

    if (validateBlock(b, 8)) {
        printf("Unexpected valid block\n");
        printBoard(b);
    }
    destroyBoard(b);
}

void testSolveBoard() {
    Board *b, *solved;

    b = createBoard();
    solved = solveBoard(b);
    if (solved != NULL) {
        printBoard(solved);
        destroyBoard(solved);
    }
    else {
        printf("Solve failed");
    }
    destroyBoard(b);
}


int main() {
    testValidateCol();
    testValidateRow();
    testValidateBlock();
    testSolveBoard();
    

    return 0;
}


