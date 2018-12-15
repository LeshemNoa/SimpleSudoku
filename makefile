CC = gcc
OBJS = game.o solver.o
# TODO: decide on final name
EXEC = hw3
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) main.o $(OBJS) -o $@
test: test.o $(OBJS)
	$(CC) test.o $(OBJS) -o test

main.o: main.c game.h
	$(CC) $(COMP_FLAG) -c $*.c
game.o: game.c game.h
	$(CC) $(COMP_FLAG) -c $*.c
solver.o: game.h solver.h solver.c
	$(CC) $(COMP_FLAG) -c $*.c
test.o: test.c game.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
