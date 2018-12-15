#include "solver.h"
#include <stdlib.h>

/* solveBoard
Gets a configuration of a sudoku board and uses the deterministic backtracking
algoritm to generate a solution if one exists. The solution is returned if it exists,
otherwise null. */
Board* solveBoard(Board* b) {
    Board* clone;
    int try;
    int currRow, currCol;

    clone = cloneBoard(b);
    if (clone == NULL) {
        return NULL;
    }

    if (! nextEmptyCell(clone, &currRow, &currCol)) {
        if (validateGame(clone)) {
            return clone;
        }
        else {
            destroyBoard(clone);
            return NULL;
        }
    }

    for (try = 1; try <= 9; try ++) {
        Board* solved;
        setCellValue(clone, currRow, currCol, try);
        if (!validateGame(clone)) {
            continue;
        }

        solved = solveBoard(clone);
        if (solved != NULL) {
            destroyBoard(clone);
            return solved;
        }
    }

    destroyBoard(clone);
    return NULL;
}


