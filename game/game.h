
/* 
 * game.h - header file for CS50 final progject game module
 *
 * A *game* is a structure that contains everything we need to track the game status, number of players, players'name, 
 * where the players are, location of gold, map using the grid module, number of the gold left.
 *
 * Team Spruce, May, 2021
 */

#ifndef __GAME_H
#define __GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grid.h"
#include "mem.h"
#include "message.h"
#include <string.h>


/**************** file-local global variables ****************/
static const int MaxPlayers = 26;  


/**************** global types ****************/
typedef struct game game_t;

/**************** game_new ****************/
/* Create a new game structure that contains a grid,
 * an array of player, and the number of the to be dropped.
 *
 * Caller provides:
 *   number of golds to be dropped (must be > 0),
 *   number piles of nuggets the game has,
 *   a valid string to a map's name 
 *
 * We return:
 *   a pointer to a new game, return NULL if error.
 *
 * We guarantee:
 *   the array of player in the game is empty, and the grid is 
 *   initialized properly by calling game_buildMap function.
 *
 * Caller is responsible for:
 *   later calling game_delete.
 */
game_t* game_new(const char* map_name, const int nuggets_count, const int num_piles);

/**************** game_addPlayer ****************/
/* Add a new player to the game.
 *
 * Caller provides:
 *   valid pointer to the game, valid pointer to the player's real_name, 
 *   valid address for that player

 * We return:
 *   false, if the player is already in the game, any parameter is NULL, or error;
 *   true, if the new player is being added to the game.
 *
 */
bool game_addPlayer(game_t* game, const char* real_name, addr_t address);

/**************** game_playerNugget ****************/
/* Get the number of nuggets the given player has
 * 
 * Caller provides:
 *   valid pointer to the game, valid address of a player

 * We return:
 *   -1, if any parameter is NULL, or the player is not existed in the game
 *   else, return the number of nuggets the given player has
 */
int game_playerNugget(game_t* game, addr_t addr);

/**************** game_nuggetsRemain ****************/
/* Return the number of nuggets remains in the game
 * 
 * Caller provides:
 * valid pointer to the game
 *
 * We return:
 *  -1, if any parameters is NULL
 *  else, return the number of nuggets the game remains
 */
 int game_nuggetsRemain(game_t* game);

/**************** game_quitPlayer ****************/
/* Remove a new player from the game by setting its address to be noaddr
 *
 * Caller provides:
 *   valid pointer to game, valid char for the player's name 
 * We return:
 *   false, if the player is not in the game, any parameter is NULL, or error;
 *   true, if the player is being removed from the game.
 */
bool game_quitPlayer(game_t* game, addr_t address);

/**************** game_movePlayer ****************/
/* Update the game status according to player's key press.
 *
 * Caller provides:
 *   valid pointer to game, valid address for the player trying to move
 *   valid char for the key being pressed by the player
 *
 * We return:
 *   false, if any parameter is NULL, and the key is not valid
 *   otherwise, we update the game according to the destination 
 *   it reaches after moving towards the given direction.
 */
bool game_movePlayer(game_t* game, addr_t address, const char direction);

/**************** game_noNuggets ****************/
/* Check whether there are any Nuggets left in the game
 *
 * Caller provides:
 *   valid pointer to game
 *
 * We return:
 *   false, if any parameter is NULL, or the number of nuggets in the game is not zero,
 *   otherwise, we return true.
 */
bool game_noNuggets(game_t* game);


/**************** game_getPlayersPurse ****************/
/* Get the string contatining each player's nuggets and its real_name
 * 
 * Caller provides:
 *   valid pointer to game
 * 
 * We return:
 *   NULL, if any paramters is NULL
 *   a string containing each player's nuggets and its real_name
 */
char* game_getPlayersPurse(game_t* game);

/**************** game_getPlayerGrid ****************/
/* Get the given player's grid
 *
 * Caller provides:
 *   valid pointer to a game, and a valid address of a player
 * 
 * We return:
 *   NULL, if any parameters is NULL, or the player is not found
 *   else, the grid for the given player
 */
char* game_getPlayerGrid(game_t* game, addr_t address);

/**************** game_getMainGrid ****************/
/* Get the main grid for the game
 *
 * Caller provides:
 *   valid pointer to a game
 * 
 * We return:
 *   NULL, if any parameters is NULL
 *   else, the main grid of a game
 */
char* game_getMainGrid(game_t* game);

/**************** game_delete ****************/
/* Delete function for a game
 *
 * Caller provides:
 *   a valid pointer to a game
 *
 * We do:
 *   nothing, if any parameters is NULL
 *   free the memory of each element in the game, and free the game itself
 */
void game_delete(game_t* game);

/**************** game_getAllAddr ****************/
/* Get the array of addresses of all players in the game is the player exists
 * 
 * Caller provides:
 *   a valid pointer to a game
 *
 * We return:
 *   nothing, if any parameters is NULL
 *   an array of players' addresses if that player is not NULL
 */
addr_t* game_getAllAddr(game_t* game);


/**************** game_getPlayerNumber ****************/
/* Get the number of players already in the game
 *
 * Caller provides:
 *   a valid pointer to a game
 *
 * We return:
 *   0, if no player is in the game or any parameters is NULL
 *   otherwise, the number of players in the game
 */
int game_getPlayerNumber(game_t* game);

/**************** game_getPlayerChar ****************/
/* Get a player's char representation according to its address
 * 
 * Caller provides:
 *   a valid pointer to a game, and a valid address of a player
 *
 * We return:
 *   !, if any parameters is NULL, or the player with that address is not found
 *   the char representation of that player with the given address
 */ 
char game_getPlayerChar(game_t* game, const addr_t address);

/**************** game_quitSpectator ****************/
/* Quit the spectator of a game by setting the address of spectator to noaddr
 * 
 * Caller provides:
 *   a valid pointer to a game
 * 
 * We return:
 *   false, if the game is NULL
 *   true, if the spectator is being successfully quitted
 */
bool game_quitSpectator(game_t* game);

/**************** game_getSpecAddr ****************/
/* Get the address of the current spectator of the game
 *
 * Caller provides:
 *   a valid pointer to a game
 *  
 * We return:
 *   noAddr, if any parameters is NULL or there is no spectator in the game
 *   a valid address of the current spectator
 */
addr_t game_getSpecAddr(game_t* game);

/**************** game_gridxsize ****************/
/* Get the xsize of the main grid for the game
 * 
 * Caller provides:
 *   a valid pointer to a game
 * 
 * We return:
 *   the xsize of the main grid for the game
 */
int game_gridxsize(game_t* game);

/**************** game_gridysize ****************/
/* Get the ysize of the main grid for the game
 * 
 * Caller provides:
 *   a valid pointer to a game
 * 
 * We return:
 *   the ysize of the main grid for the game
 */
int game_gridysize(game_t* game);

/**************** game_addSpectator ****************/
/* Add a spectator to the game
 * 
 * Caller provides:
 *   a valid pointer to a game, a valid address of a player
 * 
 * We return:
 *   noAddr if any parameters is NULL, or there is no spectator right before the new spectator joins
 *   a valid address of the current spectator of the game
 */
addr_t game_addSpectator(game_t* game, const addr_t address)

#endif // __GAME_H
