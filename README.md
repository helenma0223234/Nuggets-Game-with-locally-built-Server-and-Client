# Nuggets

This repository contains the code for the CS50 "Nuggets" game, in which  players explore a set of rooms and passageways in search of gold  nuggets. The rooms and passages are defined by a *map* loaded by the server at the start of the game. The gold nuggets are randomly distributed in *piles* within the rooms. Up to 26 players, and one spectator, may play a given game. Each player is randomly dropped into a room when joining the game. Players move about, collecting nuggets when they move onto a pile. When all gold nuggets are collected, the game ends and a summary is printed.

## Authors

Helen '23, Coco '23, So '23

## grid

The grid directory contains the grid module. The header file is `grid.h` and the implementation is `grid.c`. This grid implements a grid data structure used for the Nuggets game. The grid will hold the players, walls, nuggets and such in the map. More details are in the directory `README.md`.

## game

The game directory contains the game module. The header file is `game.h` and the implementation is `game.c`. The game modules implements the actual game using the `grid` module. For example, it will handle players moving, quitting, joining, keeping track of visibility of each player, etc. More details are in the directory `README.md`.

## server

The server directory contains the `server.c` file. The server handles the messages that come in from the players. These messages are then processed and "passed into" the game for the game to act appropriately. More details are in the directory `README.md`.

## maps

This directory contains the various maps that were provided as well as the one our group has made.