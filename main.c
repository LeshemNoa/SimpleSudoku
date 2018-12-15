#include <stdio.h>
#include "game.h"



int main(int argc, char** argv) {
    Board* b = createBoard();
    /*b->cell[0][0].val = 1;
    b->cell[0][0].isFixed = 1;*/
    printBoard(b);
    destroyBoard(b);

    if (argc == 2) {
        char* seed = argv[1];
        printf("seed is %s\n", seed);
    } else {
        printf("usage: %s <seed>\n", argv[0]);
        return -1;
    }
    return 0;
}
