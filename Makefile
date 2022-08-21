# Makefile for CS50 Tiny Search Engine
#
# David Kotz - April 2016, 2017, 2021

.PHONY: all clean

############## default: make all libs and programs ##########
# If libcs50 contains set.c, we build a fresh libcs50.a;
# otherwise we use the pre-built library provided by instructor.
all: 
	make -C grid
	make -C game
	make -C server

############### TAGS for emacs users ##########
TAGS:  Makefile */Makefile */*.c */*.h */*.md */*.sh
	etags $^

############## clean  ##########
clean:
	rm -f *~
	rm -f TAGS
	make -C grid clean
	make -C game clean
	make -C server clean
