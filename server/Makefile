# Makefile for CS50 Nugget Game server                  
# Final project                                                         
# Team Spruce May 27, 2021

L = ../grid
L2 = ../support
L3 = ../game
LIBS = -lm

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$(L2) -I$(L3)
CC = gcc
OBJS =  $L/file.o $(L2)/log.o $L/grid.o $(L2)/message.o $(L3)/mem.o $(L3)/game.o server.o
LIBS = -lm

MAKE = make

# for memory-leak tests                                                                                           
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

server: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(LLIBS) -o server  

server.o: $L/grid.h $(L2)/message.h $(L2)/log.h $(L3)/game.h $(L3)/mem.h
grid.o: $L/grid.h $L/file.h
game.o: $(L3)/game.h $L/grid.h $(L2)/message.h $(L2)/log.h $(L3)/mem.h
file.o: $L/file.h
mem.o: mem.h



.PHONY: test valgrind clean

test:
	./servertest &> testing.out

valgrind:
	valgrind --leak-check=full ./server ../maps/small.txt

clean:
	rm -rf *.dSYM  # MacOS debugger info                
	rm -f *~ *.o
	rm -f server
	rm -f core
	rm -f vgcore*
