# CS50 Nuggets-spruce

## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
Here we focus on the core subset:
- Control flow (functions to be implemented)
- Major Data Structures and their functions
- Detailed function prototypes and their parameters
- Error Handling
- Visibility
- Testing plan

## Control flow

The Nugget game's server is implemented in one file 'server.c', with 10 functions

### Constants

Set up constant variables specified in the [Requirements Specification](REQUIREMENTS.md) as file-local global variables with static const int.

### main

The `main` function calls `parseArgs` and `setupGame`, message_loop`,then exits zero

```
Call parseArgs
Call setupGame
Call message_loop with the following parameters:
        arg = game, timeout = 0
        handleTimeout = NULL
        handleInput = NULL
        handleMessage = playGame
cleanup memory usages
```

### parseArgs

Given arguments from the command line, extract them into two function parameters; return only if successful.
* for `map.txt`, verify whether the file is a valid readable file 
* for optional `seed`, verify if it is an integer, pass to srand(seed); if not provided, initiate srand(getpid())
* if any trouble is found, print an error to stderr and exit non-zero.

### setupGame

Set up basics of the server for the game to start and clients/players to connect...

```
Initialize the message module for the server to a log file (message_init)
Initialize the 'game' with the map file
        Drop random number (between 'GoldMinNumPiles' and 'GoldMaxNumPiles') in Game
Log and announce port number
```

### playGame

The main driver of the same, read the messages and call corresponding handler functions; 
the function is called in message_loop

```
get the first word of the message
        compare if it is one of KEY/QUIT/Q/SPECTATE/PLAY
        if not:
                send "ERROR message "to the client
        if KEY:
                validate if the char following KEY is an executable key move
                if not: send "ERROR message"
                if all gold are found:
                        return true to message_loop
                        call endGame
        if QUIT:
                if followed by any letter/charaters: send "ERROR message"
                else quit the player
        if PLAY:
                call handlePlay with addr_t
                return false
        if is SPECTATE:
            call handleSpectate with addr_t
            return false


```

### handlePlay

handles PLAY message, add a new player to the game

```
parse out the user's "real name" following the PLAY chars
if we have max players already:
        respond with "QUIT Game is full: no more players can join"
        
if empty real name:
        respond with "QUIT Sorry - you must provide player's name"
        
else: //Add player to game
        Call normalizeName
        Assign a character 'c' to the player
        Let GAME know that new player joined and pass in 'c', real name, and addr_t
        send "OK c" to player
        send "GRID nrows ncols", nrows and ncols is size of grid
        send "GOLD 0 0 r", where r is remaining gold
        send "DISPLAY\nstring", to each client with player added //string is client's map
```

### handleKey

handle KEY input message, update the game with player's input new move:

```
if key is "Q":
        handleQuit
else:
        update the player's coordinates in Game
        if a gold is collected by the move:
                assign the player gold
                update the client with new status line
        calculate visibility in Game module
        send "DISPLAY\nstring" to all clients //string is client's map
```

### handleSpectate
handle SPECTATE input message, add new spectator (and remove the old):

```
if there's already a spectator:
        send "QUIT message" to old spectator //Replace spectator
send "GRID nrows ncols"
send "GOLD 0 0 r"
send "DISPLAY\nstring" of master map in Game
```

### handleQuit

handle Q input message, quit a client:

```
if player:
        send "QUIT message" to player
        Delete player from Game
        send "DISPLAY\nstring" without player for all clients
else:
        send "QUIT message" to spectator
```
### endGame

End the current game and summarize it:

```
print out summary of gold collections to the main server log file
send "QUIT GAME OVER" with summary to all clients

```

### parseRest

Parse the string pointer into real name's format

```
create new string
copy pase the old string to the new
loop each char:
    if is not graph and not black:
        replace with _
return the new string
```

## Major Data Structures

### Grid

The grid structure will be implemented using a 2d-array. The "outer" array will be the columns, and for each column there will be an array, where each index represents a row.
Function to simply return the horizontal and vertical size of the grid, respectively.

#### grid_new

Initializes the grid with the map given by a file pointer. We expect the file pointer to point to a valid map file.

#### grid_find

Finds and returns the character in the given (x,y) coordinate.

#### grid_replace

Replaces the item in the (x,y) coordinate of the grid.

#### grid_toString
Returns the grid as a string. Pseudocode:

```
for each row:
        for each column:
                concatenate the char to the string
        concatenate a new line character
return string
```

#### grid_move

Moves the character in the `(x_start, y_start)` coordinate to `(x_dest, y_dest)`. Start character will hold a space character.

```
get item k in (x_start, y_start)
Call grid_replace on (x_dest, y_dest) with k
Call grid_replace on (x_start, y_start) with space character
```

#### grid_delete

Frees the grid and its 2D array.

#### gird_addVisible

This implements the visibility part of the requirement spec. More details and its pseudocode is found in the below section named "Visibility".

### Game

### Game

The game data structure holds the following values: 

* An array of `player_t*` - `player_t` is an internal data structure. Details below.
* The main `grid` - The grid containing everything happening in the map from the players moving to the gold nuggets. This is what the spectator sees.
* The original `grid` - The grid of the original map. Only the walls, corridors, and empty spaces.
* number of gold piles
* number of gold nuggets left
* address of the current spectator

#### player_t data structure

This data structure is used internally in the game module to keep track of each player and their grids. Here is the list of data each `player_t` holds:
* The current x, y coordinates of the player
* The player's `grid` - what is sees and remembers
* The player's "real name"
* The number of gold nuggets collected
* The `addr_t` of the player
The way the server is expected to communicate with the game structure is to pass in the `addr_t` whenever it receives a message, and the game structure handles "mapping" the `addr_t` to the corresponding player.

#### game_new

Creates a new game struct by initializing a new grid using the map. Also initializes an array of `player` structs (internal data structure). 

#### game_gridxsize

Get the x_size of the main_grid in the game and return to the server.

#### game_gridysize

Get the y_size of the main_grid in the game and return to the server.

#### game_addSpectator

Add the new spectator to the game. If there is no spectator in the game, assign this new spectator to be the spectator for the game; if there is a spectator in the game, replace the current one with the new spectator.

```
get the current spectator's address
if the address is not NULL
        assign the new address to the address of the spectator in the game
        return the address of the current spectator
assign the new address to the address of the spectator in the game
```
#### game_addPlayer

Adds the player into the game by creating a `player_t` struct for it. 

```
get the index of the players array of where this new player should be place
create a new player struct according to its realname and address
assign the this player to the matched slot in the players array
update the main grid of the game with the player
update the grid for all the players
update the player's own grid with the '@' representation
```

#### game_quitPlayer

Quit a player by removing it from the game

```
delete the player in the players array
update the main grid
update the grid for each player in the game
```
#### game_quitSpectator

Quit a Spectator by removing it from the game

```
delete the old spectator if there was any
update the spector to new address
return the old addr_t
```

#### game_movePlayer

Moves the player in both the main grid and the player grid according to the key pressed. If visible for other players, update this for the other players as well.

```
calculate the destination point for the player that needs to move according to the given direction
if the destination is a nugget
        assign random nuggets to the player
        update the remaining nuggets and piles for the game
        move the player to the destination and update the player's x, y location
        update the main grid and grid for each player in the game accordingly
if the destination is empty space
        move the player to the destination and update the player's x, y location
        update the main grid and grid for each player in the game accordingly
if the destination is passage
        move the player to the destination and update the player's x, y location
        update the main grid and grid for each player in the game accordingly   
if the destination is a vertical boundary
        do nothing
if the destination is a horizontal boundary
        do nothing      
if the destination is a corner
        do nothing
if the destination is a solid rock
        do nothing
if the destination is a player
        switch the positions of two players
        update each player's x,y location
        update the mian grid and the grid for each player in the game accordingly
```

#### game_noNuggets

Check whether there are any nuggets left for the game, return true if there is no nuggets.

#### game_nuggetsRemain

Return numbers of nuggets remained uncollected in the current game grid.

### game_getAllAddr

Return an array of address of all the players in the game.

```
create an array of address
loop through each player in the game 
        add it to the array
```

#### game_getPlayerChar

Get the letter representation of the player in the game.

```
loop through all the players in the game
        check whether the player's address it equal to the given address
return the char name of the player found
```

#### game_getSpecAddr

Get the address of the current spectator in the game.

#### game_getPlayerNumber

Get the number of players currently active by looping through the players list.


#### game_getPlayersPurse

Returns the number of nuggets the player holds currently.

#### game_getPlayerGrid

Calls the grid_print function on the player's grid, returning what the player sees/remembers for their map.

#### game_getMainGrid

Calls the grid_print function on the player's grid, returning what the player sees/remembers for their map.

#### game_delete

Frees memory for the game, freeing all grids and such.

## Overall Structure

Server - Holds a global `game_t*` and `set` of (`char`, `addr_t*`) to keep track of address of each player.
Game - Holds an array of `grid_t*` and corresponding array of `player struct` (internal struct), the indices of the arrays match each `player` to their `grid_t*`.

## Function prototypes

### Server

```c
static void parseArgs(const int argc, char* argv[], char** mapAddr, int* seed);
static game_t* setupGame(char* mapAddr, int seed);
static bool playGame(void* arg, const addr_t from, const char* message);
static void handlePlay(game_t* game, char* real_name, const addr_t address, int currentPlayer);
static void handleSpectate (game_t* game, const addr_t address);
static void handleKey(game_t* game, const addr_t address, const char direction);
static void handleQuit(game_t* game, const addr_t address);
static void endGame(game_t* game);
static char* parseRest(const char* message);
```

### Grid

```c
int grid_xsize(grid_t* grid);
int grid_ysize(grid_t* grid);
int grid_ysize(grid_t* grid);
grid_t* grid_emptyNew(int x_size, int y_size);
char grid_get(grid_t* grid, int x, int y);
bool grid_set(grid_t* grid, char item, int x, int y);
bool grid_set(grid_t* grid, char item, int x, int y);
char* grid_toString(grid_t* grid);
void grid_delete(grid_t* grid);
void grid_addVisible(grid_t* baseGrid, grid_t* originalGrid, grid_t* destGrid, int x_s, int y_s);
```

### Game

```c
typedef struct game game_t;
game_t* game_new(const char* map_name, const int nuggets_count, const int num_piles);
bool game_addPlayer(game_t* game, const char* real_name, addr_t address);
int game_playerNugget(game_t* game, addr_t addr);
int game_nuggetsRemain(game_t* game);
bool game_quitPlayer(game_t* game, addr_t address);
bool game_movePlayer(game_t* game, addr_t address, const char direction);
bool game_noNuggets(game_t* game);
char* game_getPlayersPurse(game_t* game);
char* game_getPlayerGrid(game_t* game, addr_t address);
char* game_getMainGrid(game_t* game);
void game_delete(game_t* game);
addr_t* game_getAllAddr(game_t* game);
int game_getPlayerNumber(game_t* game);
char game_getPlayerChar(game_t* game, const addr_t address);
bool game_quitSpectator(game_t* game);
addr_t game_getSpecAddr(game_t* game);
int game_gridxsize(game_t* game);
int game_gridysize(game_t* game);
addr_t game_addSpectator(game_t* game, const addr_t address);
```

## Visibility

In this section we go into more detail of how the visible field of each player is found. When a player moves, `grid_addVisible` is called for that player to find the visible areas.  Do implement this, we will write a function called `isVisible` that returns true if and only if a point is visible from another. Here is how it works. Let (x_s, y_s) and (x_g, y_g) to be the two coordinates. We will "step" from (x_s, y_s) to (x_g, y_g) by incrementing. That is, we will check all the points that are in between (x_s, y_s) and (x_g, y_g). For every "step" between, we check according to the requirement spec: if the step happens to be on a specific coordinate, check that there is no wall there. If the step happens to be in between coordinates (non-integer coordinate), then check BOTH the coordinate above and below (or left and right) of the non-integer coordinate. For example, if the current coordinate that is being checked is (4, 4.5), then check [(4, 4) AND (4,5)]. The function returns true if and only if they pass these checks. 

Pseudocode:

```
Let x_s, y_s be coordinate of players
Let x_g, y_g be coordinate of the point seen
x_d = x_s - x_g
y_d = y_s - y_g
if x_d > y_d: //Every step is +1 to x, and +slope for y
        x_step = 1
        y_step = |y_d/x_d|
else: // And vice versa
        y_step = 1
        x_step = |x_d/y_d|
        
if x_d > 0: //point seen must be left of player
        x_step = -x_step 
        
if y_d > 0: //point seen must be above player
        y_step = -y_step
curr = (x_s + x_step, y_s + y_step) // Current coordinates
while curr != (x_g, y_g):
        if y_curr is not an integer:
                check at (x_curr, floor(y_curr)) AND (x_curr, ceiling(y_curr)) //Fine if only one is a wall
        if x_curr is not an integer:
                check at (floor(x_curr), y_curr) AND (ceiling(x_curr), y_curr)
        else:
                check at (x_curr, y_curr)
        curr = (x_curr + x_step, y_curr + y_step)
if at any point the checks encounter a non-empty space, return FALSE
Otherwise, return TRUE
```

This function is then called for all points in the grid in the function `grid_addVisibility`. For each visible point, we add into the existing grid if it doesn't contain that information yet. 

Pseudocode:
```
for each point in the baseGrid:
        if destination is empty there AND it should be visible //Call isVisible
                replace destGrid(point) with baseGrid(point)
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

The server won't shut down once initiated until all gold is collected, which means malformat messages from the client sides will be handled internally (they will be logged and get an explanation of what command/format to send correctly). 

## Testing plan

Here is the implementation-specific testing plan.

### Unit testing

Test each function in: Grid module, Game module, Player Module, server itself. Each test will include testing functionality and error handling. For example, the grid module will be tested with a series of "insert" and "move" commands as well as passing in NULL for arguments. The Game module will be tested by starting and ending the game by manually "collecting all remaining gold". The test for server messages will be done mainly in the integration testing.

### Regression testing

- We will write a series of moves inside a .c script for a player to conduct using in a given map
- We will also save messages we sent out from the server in a log file to compare it with what we expected

### Integration testing
Use the provided client to test the server. The following is the list of testing plans for the server:
- Test ./server with: invalid number of arguments and invalid arguments.
- Test with invalid messages from Players.
- Test with different numbers of player (max 26) and spectator (one old and one joins as new).
- Write a bash script `testing.sh` for the client side to simulate a player doing a series of valid and invalid "movements" and commands. Verify change in the state of the game on the server side.
- Valgrind to check for memory leaks on the server side.
