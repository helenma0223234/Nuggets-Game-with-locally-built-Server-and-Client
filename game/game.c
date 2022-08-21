/* 
 * game.c - Source code for CS50 final progject game module 
 * 
 * see game.h for more information.
 *
 * Team Spruce, May, 2021
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grid.h"
#include "mem.h"
#include "message.h"
#include <string.h>


   
/**************** file-local global variables ****************/
static const int MaxPlayers = 26;  

/**************** local types ****************/
typedef struct player {
    int x;
    int y;
    grid_t* grid;
    char* real_name;
    int num_nuggets;
    addr_t address;
} player_t;


/**************** global types ****************/
typedef struct game {
    player_t** players;
    grid_t* main_grid;
    grid_t* original_grid;
    int num_piles;
    int num_nuggets;
    addr_t specAddr;
} game_t;


/***************** prototype declarations *****************/
static int charToInt(const char playerLetter);
static char intToChar(const int playerNum);
static void player_delete(player_t* player);
static player_t* player_new(game_t* game, char* real_name, addr_t addr);
static player_t* getPlayer(game_t* game, addr_t address);
static void game_dropGolds(game_t* game, const int num_piles);
static void switchPlayers(game_t* game, player_t* player1, player_t* player2);
static void movePlayer(game_t* game, player_t* player, int x_dest, int y_dest);    
static void update_visible(game_t* game);
static int getTotalSteps(game_t* game, player_t* player, int x_step, int y_step);


/********************** functions ***********************/

/********************** game_new **********************/
/*
 * See game.h for details.
 */
game_t* 
game_new(const char* map_name, const int nuggets_count, const int num_piles)
{
    // defensive code check
    if (map_name == NULL || nuggets_count < 0 || num_piles < 0) {
        return NULL;
    }


    game_t* game = mem_malloc(sizeof(game_t));
    mem_assert(game, "Error: failed to initialize a game structure.\n");

    if (game != NULL) {
        // create an array of players with each slot empty
        player_t** players = mem_calloc(MaxPlayers + 1, sizeof(player_t*));
        mem_assert(players, "Error: failed to initialize an array of player.\n");

        FILE* fp = fopen(map_name, "r");
        // build the grids for the game
        grid_t* grid = grid_new(fp);
        grid_t* baseGrid = grid_new(fp);
        game->players = players;
        game->main_grid = grid;
        game->original_grid = baseGrid;
        game->num_piles = num_piles;
        game->num_nuggets = nuggets_count;
        // dropping nuggets for the game
        game_dropGolds(game, num_piles);
        game->specAddr = message_noAddr();
        fclose(fp);
    }

    return game;
}

/********************** game_gridxsize **********************/
/*
 * See game.h for details.
 */
int
game_gridxsize(game_t* game)
{
    return grid_xsize(game->main_grid);
}


/********************** game_gridysize **********************/
/*
 * See game.h for details.
 */
int
game_gridysize(game_t* game)
{
    return grid_ysize(game->main_grid);
}


/********************** game_addPlayer **********************/
/*
 * See game.h for details.
 */
bool 
game_addPlayer(game_t* game, char* real_name, addr_t address) 
{
    // defensive code check
    if (game == NULL || real_name == NULL || !message_isAddr(address)) {
        return false;
    }
    
    player_t** players = game->players;
    int index;

    //Loop for all existing players
    for (index = 0; index < MaxPlayers; index++) {         
        if (players[index] == NULL) {
            break;
        }
        if (message_eqAddr(players[index]->address, address)) {
            return false; //Player already here
        }
    }  
    // if there is no slot
    if(index == MaxPlayers){
        return false;
    }

    // get the char representation of the player
    char name = intToChar(index);
    printf("gameaddplayer: %s\n", real_name);

    // create a new player
    player_t* player = player_new(game, real_name, address);
    players[index] = player;
    
    // update the main_grid
    int x = player->x;
    int y = player->y;
    grid_set(game->main_grid, name, x, y);
    
    // update the visibility for the whole day
    update_visible(game);
    grid_set(player->grid, '@', x, y);

    return true;
}


/********************** game_nuggetsRemain **********************/
/*
 * See game.h for details.
 */
int
game_nuggetsRemain(game_t* game)
{   
    if (game == NULL) {
        return -1;
    }

    return game->num_nuggets;
}


/********************** game_playerNugget **********************/
/*
 * See game.h for details.
 */
int
game_playerNugget(game_t* game, addr_t addr)
{
    if (game == NULL) {
        return -1;
    }
    // get the player of the given address
    player_t* player = getPlayer(game, addr);
    if (player != NULL) {
        return player->num_nuggets;
    } else {
        return -1;
    }
}


/********************** addr_t **********************/
/*
 * See game.h for details.
 */
addr_t
game_addSpectator(game_t* game, const addr_t address)
{   
    // defensive code check
    if (game == NULL || !message_isAddr(address)){
        return message_noAddr();
    }
    // get the address of the current spectator for the game
    addr_t specAddr = game->specAddr;
    // if there is a spectator
    if (message_isAddr(address)){
        game->specAddr = address;
        return specAddr;
    }
    game->specAddr = address;
    return message_noAddr();
}


/********************** game_quitPlayer **********************/
/*
 * See game.h for details.
 */
bool 
game_quitPlayer(game_t* game, addr_t address)
{
    // defensive code check
    if (game == NULL || !message_isAddr(address)) {
        return false;
    }
    // find the player according to the given address
    player_t* player = getPlayer(game, address);
    

    if (player == NULL){
        return false;
    }

    int x = player->x;
    int y = player->y;
    char name = grid_get(game->main_grid, x, y);
    int index = charToInt(name);

    // set the quitted player's address to noAddr
    player_t** players = game->players;
    players[index]->address = message_noAddr();
    char current = grid_get(game->original_grid, x, y);
    grid_set(game->main_grid, current, x, y);

    // update the visibility for the game
    update_visible(game);    

    return true;
}


/********************** game_movePlayer **********************/
/*
 * See game.h for details.
 */
bool 
game_movePlayer(game_t* game, addr_t address, const char direction)
{
    // defensive code check
    if (game == NULL || !message_isAddr(address)) {
        return false;
    }
    // get the player
    player_t** players = game->players;
    player_t* player = getPlayer(game, address);
    if (player == NULL) {
       return false;
    } 

    int curr_x = player->x;
    int curr_y = player->y;
    int x_step = 0;
    int y_step = 0;
    int total_steps = 1;
    bool capital = false;

    // calculate the destination based on the given key
    switch (direction) {
        case 'h': 
            x_step = -1;
            break;
        case 'H':
            x_step = -1;
            capital = true;
            break; 
        case 'l':
            x_step = 1;
            break;
        case 'L':
            x_step = 1;
            capital = true;
            break;
        case 'j':
            y_step = 1;
            break;
        case 'J':
            capital = true;
            y_step = 1;
            break;
        case 'k':
            y_step = -1;
            break;
        case 'K':
            capital = true;
            y_step = -1;
            break;
        case 'y':
            x_step = -1;
            y_step = -1;
            break;
        case 'Y':
            capital = true;
            x_step = -1;
            y_step = -1;       
            break;
        case 'u':
            x_step = 1;
            y_step = -1;
            break;
        case 'U':
            capital = true;
            x_step = 1;
            y_step = -1;
            break;
        case 'b':
            x_step = -1;
            y_step = +1;
            break;
        case 'B':
            capital = true;
            x_step = -1;
            y_step = +1;
            break;
        case 'n':
            x_step = 1;
            y_step = +1;
            break;
        case 'N':
            capital = true;
            x_step = 1;
            y_step = +1;
            break;
        default:
            return false;       
    }
    
    // check whether the key is capital
    if (capital){
        total_steps = getTotalSteps(game, player, x_step, y_step);
    }

    for (int i = 0; i < total_steps; i++){
        int dest_x = curr_x + x_step;
        int dest_y = curr_y + y_step;
        int nuggets;
        int other; 
        char dest = grid_get(game->main_grid, dest_x, dest_y);
        // check what is in the destination
        switch (dest) {
            case '*':
                // Update nuggets
                if (game->num_piles == 1){
                    nuggets = game->num_nuggets;
                }
                else{
                    nuggets = 1 + rand() % (1 + 2 * game->num_nuggets/game->num_piles);
                    if (nuggets == game->num_nuggets){
                        nuggets -= game->num_piles;
                    }
                }

                game->num_nuggets = game->num_nuggets - nuggets;
                game->num_piles = game->num_piles - 1;
                player->num_nuggets += nuggets;

                movePlayer(game, player, dest_x, dest_y);
                break;
            case '.':
                movePlayer(game, player, dest_x, dest_y);
                break;
            case '#':
                movePlayer(game, player, dest_x, dest_y);
                break;
            case '|':
                break;
            case '+':
                break;
            case '-':
                break;
            case ' ':
                break;
            default:
                // Switch players
                other = charToInt(dest);
                switchPlayers(game, players[other], player);
                break;
        }
        curr_x += x_step;
        curr_y += y_step;
        update_visible(game);
    }
    return true;
}



/********************** game_quitSpectator **********************/
/*
 * See game.h for details.
 */
bool
game_quitSpectator(game_t* game)
{
    if (game == NULL){
        return false;
    }
    game->specAddr = message_noAddr();
    return true;
}


/********************** game_noNuggets **********************/
/*
 * See game.h for details.
 */
bool 
game_noNuggets(game_t* game) 
{
    if (game == NULL) {
        return false;
    }
    // check whether there are any nuggets remain
    if (game->num_piles == 0 && game->num_nuggets == 0) {
        return true;
    } else {
        return false;
    }
}


/********************** game_printPlayerNugget **********************/
/*
 * See game.h for details.
 */
char* 
game_getPlayersPurse(game_t* game) {
    if (game == NULL) {
        return NULL;
    }
    char* playerSum = mem_calloc(MaxPlayers*300+26, sizeof(char));
    player_t** players = game->players;
    // loop through each player in the game
    for (int i = 0; i < MaxPlayers; i++) {
        if (players[i] != NULL) {
            char playerNugget[300];
            char name = intToChar(i);
            // construct a string for each player's nuggets and realname
            sprintf(playerNugget, "%c %11d %-50s\n", name, players[i]->num_nuggets, players[i]->real_name);
            strcat(playerSum, playerNugget);
        }
    }
    return playerSum;

}


/********************** game_getAllAddr **********************/
/*
 * See game.h for details.
 */
addr_t*
game_getAllAddr(game_t* game)
{
    // defensive code check
    if (game == NULL) {
        return NULL;
    }
    player_t** players = game->players;
    addr_t* array = calloc(MaxPlayers, sizeof(addr_t));
    // loop through each player in the game
    for (int i = 0; i < MaxPlayers; i++){
        if (players[i] != NULL){
            array[i] = players[i]->address;
        }
    }
    return array;
}



/********************** game_getPlayerChar **********************/
/*
 * See game.h for details.
 *
 * Return '!' if no such player with that address is found.
 */
char
game_getPlayerChar(game_t* game, const addr_t address)
{
    // defensive code check
    if (game == NULL || !message_isAddr(address)) {
        return '!';
    }

    player_t** players = game->players;
    int index;
    bool found = false;
    // loop through each player
    for (index = 0; index < MaxPlayers; index++) {
        if (players[index] != NULL) {
            if (message_eqAddr(players[index]->address, address)) {
                found = true;
                break;
            }
        }
    }
    if (!found) {
       return '!'; //Return non-alphabet if nothing is found
    } else {
        char let = intToChar(index);
        return let;
    }
}


/********************** game_getPlayerGrid **********************/
/*
 * See game.h for details.
 */
char*
game_getPlayerGrid(game_t* game, addr_t address) 
{
    if (game == NULL) {
        return NULL;
    }
    // get the player
    player_t* player = getPlayer(game, address);
    if (player == NULL) {
       return NULL;
    } else {
        char* grid = grid_toString(player->grid);
        return grid;
    } 
}


/********************** game_getMainGrid **********************/
/*
 * See game.h for details.
 */
char*
game_getMainGrid(game_t* game) {
    if (game == NULL) {
        return NULL;
    }
    char* grid = grid_toString(game->main_grid);
    return grid;
}


/********************** game_getSpecAddr **********************/
/*
 * See game.h for details.
 */
addr_t
game_getSpecAddr(game_t* game)
{
    if (game == NULL){
        return message_noAddr();
    }
    return game->specAddr;
}

/********************** game_getPlayerNumber **********************/
/*
 * See game.h for details.
 */
int
game_getPlayerNumber(game_t* game)
{
    if (game == NULL){
        return 0;
    }
    int num = 0;
    player_t** players = game->players;
    // loop through each player in the game to see if it is NULL
    for (int i = 0; i < MaxPlayers; i++){
        if (players[i] != NULL){
            num += 1;
        }
    }
    return num;
}



/********************** game_delete **********************/
/*
 * See game.h for details.
 */
void game_delete(game_t* game) 
{
    if (game == NULL) {
        return;
    }
    player_t** players = game->players;
    for (int i = 0; i < MaxPlayers; i++) {
        if (players[i] != NULL) {
            player_delete(players[i]);
        }
    }
    mem_free(players);
    grid_delete(game->original_grid);
    grid_delete(game->main_grid);
    mem_free(game);
}

        
/********************** game_dropGolds **********************/
/*
 * Helper function called for game_new().
 * Randomly drops gold into the map.
 */
static void 
game_dropGolds(game_t* game, const int num_piles)
{
    if (game == NULL || num_piles <= 0) {
        return;
    }
    // for each pile, find a right place to drop
    for (int i = 0; i < num_piles; i++) {
        grid_t* grid = game->main_grid;
        int x = grid_xsize(grid);
        int y = grid_ysize(grid);
        int gold_x = rand() % (x - 1);
        int gold_y = rand() % (y - 1);
        // find a place that is empty spot
        while (grid_get(grid, gold_x, gold_y) != '.') {
            gold_x = rand() % (x - 1);
            gold_y = rand() % (y - 1);
        }

        grid_set(grid, '*', gold_x, gold_y);
    }
}


/********************** getPlayer **********************/
/*
 * Helper function called throughout the module.
 * Find the player according to it's address.
 */
static player_t*
getPlayer(game_t* game, addr_t address)
{
    player_t** players = game->players;
    for (int i = 0; i < MaxPlayers; i++) {
        if (players[i] != NULL) {
            if (message_eqAddr(players[i]->address, address)) {
                return players[i];
            }
        }
    }
    return NULL;
}


/********************** movePlayer **********************/
/*
 * Helper function called by game_movePlayer.
 * Moves the player by manipulating the grid.
 */
static void
movePlayer(game_t* game, player_t* player, int x_dest, int y_dest)
{
    int x_curr = player->x;
    int y_curr = player->y;
    
    player->x = x_dest;
    player->y = y_dest;
    
    char c1 = grid_get(game->main_grid, x_curr, y_curr); //curr place
    grid_set(game->main_grid, c1, x_dest, y_dest);

    char c2 = grid_get(game->original_grid, x_curr, y_curr); //What should be there after
    grid_set(game->main_grid, c2, x_curr, y_curr);
    
    grid_t* playerGrid = player->grid;
    grid_set(playerGrid, '@', x_dest, y_dest);
    grid_set(playerGrid, c2, x_curr, y_curr);
}


/********************** switchPlayers **********************/
/*
 * Helper function called by game_movePLayer.
 * Switches the position between two players.
 */
static void
switchPlayers(game_t* game, player_t* player1, player_t* player2)
{
    int x1 = player1->x;
    int y1 = player1->y;
    int x2 = player2->x;
    int y2 = player2->y;

    grid_t* grid = game->main_grid;
    
    // get the content in the players' locations
    char c1 = grid_get(grid, x1, y1);
    char c2 = grid_get(grid, x2, y2);

    // switch the contends
    char c1 = grid_get(grid, x1, y1);
    char c2 = grid_get(grid, x2, y2);

    grid_set(grid, c2, x1, y1);
    grid_set(grid, c1, x2, y2);

    player1->x = x2;
    player1->y = y2;
    player2->x = x1;
    player2->y = y1;

    grid_t* playerGrid1 = player1->grid;
    grid_t* playerGrid2 = player2->grid;    

    grid_set(playerGrid1, '@', x2, y2);
    grid_set(playerGrid2, '@', x1, y1);

}


/********************** update_visible **********************/
/*
 * Helper function called throughout the module.
 * Updates all player's vision.
 */
static void 
update_visible(game_t* game)
{
    player_t** players = game->players;
    grid_t* main_grid = game->main_grid;
    // loop through each player in the game, call addVisible on its grid
    for (int i = 0; i < MaxPlayers; i++){
        player_t* player = players[i];
        if (player != NULL){
            int x = player->x;
            int y = player->y;
            grid_t* playerGrid = player->grid;
            grid_addVisible(main_grid, game->original_grid, playerGrid, x, y);
        }
    }
}


/************************ getTotalSteps ***********************/
/*
 * Helper function called by game_movePlayer.
 * Gets the total number of steps the player should move when they run.
 */
static int
getTotalSteps(game_t* game, player_t* player, int x_step, int y_step)
{
    grid_t* grid = game->original_grid;
    
    int x = player->x;
    int y = player->y;

    int steps = 1;
    char dest = grid_get(grid, x, y);
    while (dest != '-' && dest != '|' && dest != '+' && dest != ' '){
        dest = grid_get(grid, x + (x_step * steps), y + (y_step * steps));
        steps += 1;
    }
    return steps - 1;
}


/************************ PLAYER **************************/

/********************** player_new **********************/
/*
 * Creates a player struct.
 */
static player_t*
player_new(game_t* game, char* real_name,  addr_t addr)
{
    if (game == NULL || real_name == NULL){ //|| !message_isAddr(addr)) {
        return NULL;
    }
    player_t* player = mem_malloc(sizeof(player_t));
    mem_assert(player, "Error: failed to initialize a player structure.\n");
        
    grid_t* grid = game->main_grid;
    int x = grid_xsize(grid);
    int y = grid_ysize(grid);
    int player_x = rand() % (x - 1);
    int player_y = rand() % (y - 1);

    // Randomly find a place to land until valid place
    while (grid_get(grid, player_x, player_y) != '.') {
        player_x = rand() % (x - 1);
        player_y = rand() % (y - 1);
    }

    player->real_name = real_name;
    player->x = player_x;
    player->y = player_y;
    player->grid = grid_emptyNew(x, y);

    player->num_nuggets = 0;
    player->address = addr;

    return player;
}


/********************** player_delete **********************/
/*
 * Frees all memory taken up by the player.
 */
static void
player_delete(player_t* player)
{
    if (player == NULL) {
        return;
    }

    grid_delete(player->grid);
    mem_free(player->real_name);
    mem_free(player);
}


/********************** intToChar **********************/
/*
 * Helper function used throughout the module.
 * Converts an int to a char.
 */
static char 
intToChar(const int playerNum) 
{
    char playerLetter = 'A' + playerNum;
    return playerLetter;
}


/********************** charToInt **********************/
/*
 * Helper function used throughout the module.
 * Converts a char to an int.
 */
static int
charToInt(const char playerLetter)
{
    int playerNum = playerLetter - 'A';
    return playerNum;
}
