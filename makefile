CC = gcc
OBJS = game.o solver.o main_aux.o parser.o main.o
EXEC = sudoku
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

main.o: main.c main_aux.h SPBufferset.h
	$(CC) $(COMP_FLAG) -c $*.c
main_aux.o: main_aux.c main_aux.h parser.h game.h solver.h
	$(CC) $(COMP_FLAG) -c $*.c
parser.o: parser.c parser.h
	$(CC) $(COMP_FLAG) -c $*.c
game.o: game.c game.h
	$(CC) $(COMP_FLAG) -c $*.c
solver.o: solver.h solver.c game.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
