# Testing

## Grid

Below are the output and the type of testing done for the grid module. The tests were done whenever a new feature was added to verify that no existing functions were broken.

### gridtest.c

This file is a testing file for all the basic functionality for the grid module. Functions are invoked both with valid and invalid parameters to check if defensive programming is done correctly, as well as verify correct behavior. The results of this is in `testing.out`. The test can be invoked through `make test`.

### visibility.c

This file is another testing file but specifically for the visibility. The function `grid_addVisible` is invoked multiple times to check that the visibility is working correctly. The results of this is in `visible.out`. The test can be invoked through `make visible`.

### valgrind.out

The file `valgrind.out` holds the results of the `valgrind` output of both `gridtest` and `visibility`.

## Game

Below are the output and the type of testing done for the game module. The tests were done whenever a new feature was added to verify that no existing functions were broken.

### gametest.c

This file is a testing file for all the basic functionality for the game module. Functions are invoked both with valid and invalid parameters to check if defensive programming is done correctly, as well as verify correct behavior. The results of this is in `testing.out`. The test can be invoked through `make test`.
One thing to notice that for name, we pass a constant string as real name instead of malloc a string and strcpy like what we did in the server since that is easier to test the functions. The valgrind for the gametest.c involves no leak and no error.

### testing.out

The output file for running the gametest.c

## Server

Tested server.c with various commandline inputs and different players/spectator joining.
The test mainly took place by incorporating the player/client code provided by the professor. Funtionality testings were checked with valgrind to make sure there's no memeory leak.

### command testing
* test invalid directory path
* test non-existent map file path
* test no seed
* test with all valid inputs 

### functionality testing
* test with more than 26 players
* test with each map in the map directory including the challenge.txt
* test with long names longer than 50 char
* test with names with spaces
* test with spectator
* test with multiple spectators
* test with different seeds
