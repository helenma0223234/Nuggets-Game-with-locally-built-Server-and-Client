# CS50 Final Project Nugget Game

Written by: Kewen H.'23, Helen M.'23, So A.'23

## Design Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and focus on the implementation-independent design decisions.

Note: As a team of three, we are only designing and implementing the server side of the game.

Here we focus on the core subset:

- User interface
- Inputs and outputs
- Functional decomposition into modules/functions
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan
 
## User interface

As described in the [Requirements Spec](REQUIREMENTS.md), the nugget game's only interface with the user is on the command-line; it must always have three arguments.
users can be:
1. server host
2. players
3. spectator

server host:
```bash
$ ./server map.txt [seed]
```
`map.txt` is a provided text file. `seed` is a seed for a random map.

*example*
``` bash
$ ./server maps/small.txt 80000
```
player
```bash
$ ./player hostname port [playername]
```
*example*
```bash 
$ ./player spruceteam 80080 spruce
```

spectator
```bash 
$ ./player hostname port
```
*example*
```bash 
$ ./player spruceteam 80080 
```

## Inputs and outputs

*Input:* there are no file inputs (other than map.txt); there are command-line parameters described above.

*Output:*
Per the requirements spec:
server port number in stdout, messages sent to the players, the map in stdout, log files per client and for the server

## Functional decomposition into modules

We anticipate the following functions:

 1. *main*, which parses arguments and initializes other modules

 2. *parseArgs*, which parse in the input command line, validate the input, then parse out two variables: map file's path and seed (if exist)

 3. *parseMessage*, which validates the client sides messages, check the message's format and contents; if malformed, log it and send ERROR with explanation to this client

 4. *handleMessage*,  which handles messages from parseMessage, including: a) joining (initializing, storing) of new player; messaging this player that if the connection is success/fail; b) for current players in the game: handle their key input; c) for spectator: receives players location; paints whole map

 5. *setupGame*, which initializes modules, empty major data structures, and start the server port, the server logging file; set up the server to be "listening"
 6. *updateGame*, which updates a player's position in the grid map
 7. *endGame*, which makes the server prepare a tabular summary according to the format specified in the requirement spec, send QUIT to all clients along with a game summary, print the summary to log file, cleanup memory usages, then exit.
 8. *findVisibility*, which takes in the position of a player and calculate the visible field that the player should see

## Pseudo code for logic/algorithmic flow

The nugget game will run as follows:

```
parse arguments and validate input (seed and map.txt);
load map depending on whether seed is provided
Initialize modules (grid, message, etc.)
Initialize game with the map + grid;
Initialize server network (the port);
Output port number;
While (gold nuggets remaining):
    parse the message (parseMessage);
    handle the message (handleMessage);
    update the game according to player's new move (upon messages receiving);
    send corresponding message back to client;
send summary and Quit message to clients (endGame);
print summary;
```

## Major data structures

- *grid*
- *player*
- *game* 

### grid
A data structure that holds what character (and therefore a game object such as a wall or player) in what coordinate. Can be thought of as a discrete two dimensional coordinate system. Maps rows and columns to items/empty slot.
### player
Contains information (strings) including hostname, port number, unique address, player name, coordinates in the grid map, number of golds found, game status (PLAY or SPECTATE or quitted).
### game
Tracks the state of the game: number of players, players' name, where the players are, location of gold, map using the grid module, number of the gold left.

## Testing plan
### Unit testing
Test each function in: Grid module, Game module, Player Module, server itself. Each test will include testing functionality and error handling. For example, the grid module will be tested with a series of "insert" and "move" commands as well as passing in NULL for arguments. The Game module will be tested by starting and ending the game by manually "collecting all remaining gold". The test for server messages will be done mainly in the integration testing.

### Regression testing
- We will save game states to a file to compare it with behaviors we expected to happen
- We will also save messages we sent out from the server in a log file to compare it with what we expected

### Integration testing
Use the provided client to test the server. The following is the list of testing plans for the server:
- Test ./server with: invalid number of arguments and invalid arguments.
- Test with invalid messages from Players.
- Write a bash script `testing.sh` for the client side to simulate a player doing a series of valid and invalid "movements" and commands. Verify change in the state of the game on the server side.
- Valgrind to check for memory leaks on the server side.

