# Makefile for CS50 library
# 
# So Amano, May 2021 

# object files, and the target library
OBJS = grid.o file.o gridtest.o

LIBS = -lm

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

gridtest: $(OBJS) $(LLIBS) 
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) $(LIBS) -o gridtest

# Dependencies: object files depend on header files
file.o: file.h
grid.o: file.h grid.h
gridtest.o: grid.h


.PHONY: clean sourcelist all test valgrind visible

all:
	make
	$(CC) $(CFLAGS) visibility.c grid.o file.o $(LIBS) -o visibility

visible:
	$(CC) $(CFLAGS) visibility.c grid.o file.o $(LIBS) -o visibility
	./visibility &> visible.out

visibility.o: grid.h

test:
	./gridtest &> testing.out

valgrind:
	valgrind --leak-check=full ./gridtest &> valgrind.out
	valgrind --leak-check=full ./visibility &>> valgrind.out

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
	rm -f gridtest
	rm -f visibility
