# Makefile for CS50 library
#   Builds and compiles all these utility functions into a library.
#
# David Kotz, April 2016, July 2017, April 2019, 2021
# updated by Xia Zhou, July 2016

# object files, and the target library
OBJS = $L/file.o $(L2)/log.o $L/grid.o $(L2)/message.o mem.o game.o gametest.o

L = ../grid
L2 = ../support
LIBS = -lm

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$(L2)
CC = gcc
MAKE = make

gametest: $(OBJS) $(LLIBS) 
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o gametest

# Dependencies: object files depend on header files
game.o: game.h $L/grid.h $(L2)/message.h mem.h
grid.o: $L/grid.h $L/file.h
file.o: $L/file.h
gametest.o: game.h mem.h
mem.o: mem.h

.PHONY: clean sourcelist test valgrind visible

# list all the sources and docs in this directory.
# (this rule is used only by the Professor in preparing the starter kit)
sourcelist: Makefile *.md *.c *.h
	@echo $^


test:
	./gametest &> testing.out

valgrind:
	valgrind --leak-check=full ./gametest

# clean up after our compilation
clean:
	rm -f core
	rm -f gametest
	rm -f $(LIB) *~ *.o
