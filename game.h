typedef int boolean;
#define DIM 9

typedef struct cell_t {
    int val;
    boolean isFixed;
} Cell;

typedef struct board_t {
    Cell cell[DIM][DIM];
} Board;

void printBoard(Board* b);
Board* createBoard();
Board* cloneBoard(Board* b);
void destroyBoard(Board* b);

void setCellValue(Board* b, int i, int j, int val);
void setCellFix(Board* b, int i, int j, boolean fix);
int getCellValue(Board* b, int i, int j);
boolean getCellFix(Board* b, int i, int j);

boolean nextEmptyCell(Board* b, int* row, int* col);

boolean validateRow(Board* b, int i);
boolean validateCol(Board* b, int j);
boolean validateBlock(Board* b, int blockNum);
boolean validateGame(Board* b);
int whichBlock(int i, int j);








