/* server.c - c file for nugget game server
* implement the server side of the nugget game.
* see implementation spec for details of functional decomposition and functions.
* 
* Team Spruce
* CS50 Spring 2021 Dartmouth
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mem.h"
#include "log.h"
#include "message.h"
#include "game.h"
#include "grid.h"
#include  <unistd.h>

/* ************ local global vars ************ */
const static int MaxNameLength = 50;   // max number of chars in playerName
//const static int MaxPlayers = 26;      // maximum number of players
const static int GoldTotal = 250;      // amount of gold in the game
const static int GoldMinNumPiles = 10; // minimum number of gold piles
const static int GoldMaxNumPiles = 30; // maximum number of gold piles

/* ************ private function prototypes ************ */
static void parseArgs(const int argc, char* argv[], char** mapAddr, int* seed);
static game_t* setupGame(char* mapAddr, int seed);
static bool playGame(void* arg, const addr_t from, const char* message);
static void handlePlay(game_t* game, char* real_name, const addr_t address, int currentPlayer);
static void handleSpectate (game_t* game, const addr_t address);
static void handleKey(game_t* game, const addr_t address, const char direction);
static void handleQuit(game_t* game, const addr_t address);
static void endGame(game_t* game);
static char* parseRest(const char* message);

/* ****************** main ****************** */
int
main(const int argc, char* argv[])
{
	char* mapAddr;
	int seed = 0;
	parseArgs(argc, argv, &mapAddr, &seed);

    // initialize the logging module
	log_init(stderr);
	// initialize the message module
	int serverPort = message_init(stderr);
  	if (serverPort == 0) {
    	return 1; // failure to initialize message module
  	}

	game_t* game = setupGame(mapAddr, seed);
	// let the server starts listenting and dealinng with clients side
	message_loop(game, 0, NULL, NULL, playGame);

	return 0;
}

/* ******************* parseArgs ******************* */
/* Handle the messy business of parsing command-line arguments, so main() is cleaner and clearer. 
 * Extract seedURL, destination directory and crawler's depth from input arguments
 * and then 1) normalize URL, 2) initialize crawler destination directory
 *
 * We return 'void' when successful; 
 * Otherwise, print error or usage message to stderr and exit non-zero.
 * Note: following codes are sourced and adapted from CS50 examples/parseArgs.c
 */
static void
parseArgs(const int argc, char* argv[], char** mapAddr, int* seed)
{
	const char* progName = argv[0]; // name of this program
	if (argc ==1 || argc > 3) {
		fprintf(stderr, "usage: %s map.txt [seed]\n", progName);
        exit(2);
	}

	*mapAddr = argv[1];

	if (argc == 2) {
		// system generates a seed num if optional seed param is missing
		*seed = 0;
	} else {
		const char* seedString = argv[2];  // local holder of the string input of seed num
		*seed = 0;
		char excess;

		// if the argument is invalid, sscanf fails and print error
	    if (sscanf(seedString, "%d%c", seed, &excess) != 1) {
	        fprintf(stderr, "%s: '%s' invalid integer\n", progName, seedString);
	        exit(3);
	    }
	}
}

/**************** setupGame ****************/
/* initialize the game module by passing parameters accordingly,
* including generating a randome number for gold piles and nugget numbers.
*
* the user is responsible to free the game later.
*/
static game_t*
setupGame(char* mapfileName, int seed) 
{
	if (mapfileName == NULL || seed < 0) {
		fprintf(stderr, "setupGame called with NULL arg");
		return NULL;
	}
	if (seed == 0) {
		srand(getpid());
	} else { 
		srand(seed);
	}

	// specify ranges!
	int numpile = (rand() % (GoldMaxNumPiles + 1 - GoldMinNumPiles)) + GoldMinNumPiles;

	// takes two random numbers
	game_t* game = game_new(mapfileName, GoldTotal, numpile);

	if (game == NULL) {
		log_v("failed to set up a new game");
		exit(5);
	}

	return game;
}

/**************** playGame ****************/
/* parse the client input message, checks if the command is valid,
* then call handler functions according to the command key type.
* when the command is invalid, send message with ERROR "explanation"
*
* the user is responsible to free the game later.
*/
static bool
playGame(void* arg, const addr_t from, const char* message)
{
	if (arg == NULL || !message_isAddr(from)) {
		log_v("playGame called with NULL arg");
		return true;
	}
    if (message == NULL){    
        log_v("playGame tried to read NULL input message");
		return false;
    }

	game_t* game = (game_t*) arg;

	if (strncmp(message, "PLAY ", strlen("PLAY "))==0) {
		// get all characters after "PLAY" to the end
        const char* content = message + strlen("PLAY ");
		if (strlen(content) > MaxNameLength) {
			message_send(from, "ERROR player name cann't be longer than 50 characters");
		} else if (strlen(content) == 0) {
			message_send(from, "QUIT Sorry - you must provide player's name");
		}
        char* realName = parseRest(content);
        handlePlay(game, realName, from, game_getPlayerNumber(game));
		}

	else if (strncmp(message, "KEY ", strlen("KEY "))==0) {
        const char* content = message + strlen("KEY ");
        //rest = parseMessage(message);
        if (strlen(content) != 1) {
            message_send(from, "ERROR KEY command must be followed by a letter locomotion");
        }
        // check if the direction is valid kestroke
        if (content[0] == 'Q') {
            handleQuit(game, from);
        } else {
        	handleKey(game, from, content[0]);
        }	
    	// check if there's gold left
    	if(game_noNuggets(game)) {		// end game if no gold left
        	endGame(game);
            //free(result);
            //free(rest);
        	return true;
    	}
        //free(rest);
    
	} else if (strncmp(message, "SPECTATE", strlen("SPECTATE"))==0) {
        const char* content = message + strlen("SPECTATE");
        //rest = parseMessage(message);
        if (strlen(content) != 0) {
            message_send(from, "ERROR SPECTATE cannot be followed by any other letter");
        } else {
		handleSpectate(game, from);
        }
    } else {
		message_send(from, "ERROR the command has to start with PLAY/QUIT/SPECTATE/KEY");
	}
    //free(rest);
    //free(result);
    return false;
}

static char*
parseRest(const char* message)
{
    //int pos = 0;
    if (message ==NULL ) {
        return NULL;
    }

    char* new = mem_calloc(MaxNameLength+1, sizeof(char));
    int i = 0;
    char let;
    strcpy(new, message);
    while (new[i] != '\0'){
        //let = new[i];
        if (!isgraph(new[i]) && !isblank(new[i])) {
            let = '_';
            new[i] = let;
        }
        
        i++;
    }

    return new;
}

/**************** handlePlay ****************/
/* Handles PLAY messages
* If the PLAY message is valid, add the player into the game and update
* its windows accordingly with map, GOLD infor; update all player's 
* (and spectator's) windows with the newly added player
* return a new string pointer that points to that rest of string section.
* 
* log error if NULL input.
*/ 
static void
handlePlay(game_t* game, char* real_name, const addr_t address, int currentPlayer)
{
	if (real_name == NULL || game == NULL || !message_isAddr(address)) {
		log_v("handlePlay called with NULL arg");
		return;
	}
	else if (currentPlayer >= MaxPlayers) {
		free(real_name);
        message_send(address, "QUIT Game is full: no more players can join");
		return;
	}
    printf("realname:%s\n", real_name);
	if (game_addPlayer(game, real_name, address) == false) {
		log_v("adding a new player failed");
	}
	// update this player
    char message[50];
    sprintf(message, "OK %c", game_getPlayerChar(game, address));
    message_send(address, message);
    sprintf(message, "GRID %d %d", game_gridysize(game), game_gridxsize(game));
    message_send(address, message);
	// send GOLD n,p,r
    sprintf(message, "GOLD 0 0 %d", game_nuggetsRemain(game));
    message_send(address, message);

	//update all player
	addr_t* addresses = game_getAllAddr(game);
    int mapStrSize = game_gridxsize(game)*(game_gridysize(game)+1) + strlen("DISPLAY\n") + 100;
    char mapStr[mapStrSize];
    char* playerGrid;
   
	for (int i = 0; i < MaxPlayers; i++) {
        if (message_isAddr(addresses[i])) {
            // send map
        	playerGrid = game_getPlayerGrid(game, addresses[i]);
            sprintf(mapStr,"DISPLAY\n%s", playerGrid);
            message_send(addresses[i], mapStr);
            mem_free(playerGrid);
            // send GOLD
            sprintf(message, "GOLD 0 %d %d", game_playerNugget(game, addresses[i]),game_nuggetsRemain(game));
           	message_send(addresses[i], message);       
        }
    }
    mem_free(addresses);
	//update spectator
	addr_t spect = game_getSpecAddr(game);
	if (message_isAddr(spect)) {
        char* mainGrid = game_getMainGrid(game);
        sprintf(mapStr, "DISPLAY\n%s", mainGrid);
		message_send(spect, mapStr);
    }
	
}

/**************** handleSpectate ****************/
/* Handles SPECTATE messages
* add the Spectator into the current game and update
* spectator's address in the game module;
* update the spectator's window with main original map.
* if there already exists a spectator, QUIT the old spectator.
*
* log error if NULL input.
*/
static void 
handleSpectate(game_t* game, const addr_t address)
{
	if (game == NULL || !message_isAddr(address)) {
		log_v("handleSpectate called with NULL arg");
		return;
	}
	
	// Quit the old spectator
	addr_t old = game_addSpectator(game, address);
	if (message_isAddr(old)) {
    	message_send(old, "QUIT You have been replaced by a new spectator.");
	}

    char message[50];
    sprintf(message, "GRID %d %d", game_gridysize(game), game_gridxsize(game));
    message_send(address, message);
    sprintf(message, "GOLD 0 0 %d", game_nuggetsRemain(game));
    message_send(address, message);
    
    int mapStrSize = game_gridxsize(game)*game_gridysize(game) + strlen("DISPLAY\n") + 100;
    char mapStr[mapStrSize];
    char* mainGrid = game_getMainGrid(game);
    sprintf(mapStr, "DISPLAY\n%s", mainGrid);
	message_send(address, mapStr);
    mem_free(mainGrid); 
}

/**************** handleKey ****************/
/* Handles KEY messages
* If the locomotive command is valid, move the player, update all clients'
* windows accordingly with map, GOLD info (tells if the player's new move has
* collected new nuggets or not);
*
* log error if NULL input.
*/
static void 
handleKey(game_t* game, const addr_t address, const char direction)
{
	if (game == NULL || !message_isAddr(address)) {
		log_v("handleKey called with NULL arg");
		return;
	}
	// record and calculate gold collected
	int before = game_playerNugget(game, address);
	if (!game_movePlayer(game, address, direction)){
        log_v("Error on moving player");
        message_send(address, "ERROR invalid direction to move\n");
    }
	int after = game_playerNugget(game, address);

	// loop through player list and send new grid map
	addr_t* addresses = game_getAllAddr(game);
    char message[50];
	int mapStrSize = game_gridxsize(game)*(game_gridysize(game)+1) + strlen("DISPLAY\n") + 100;
    char mapStr[mapStrSize];
    char* playerGrid;
	for (int i = 0; i < MaxPlayers; i++) {
        if (message_isAddr(addresses[i])) {
        	// send map first
            playerGrid = game_getPlayerGrid(game, addresses[i]);
    		sprintf(mapStr,"DISPLAY\n%s", playerGrid);
        	message_send(addresses[i], mapStr);
        	mem_free(playerGrid);
            
            // then send gold message accordingly
        	// if no gold collected by the new move, collected == 0, same as message sent to other players
        	if (message_eqAddr(address, addresses[i])) {
        		int collected = after-before;        	
        		sprintf(message, "GOLD %d %d %d", collected, game_playerNugget(game, addresses[i]),game_nuggetsRemain(game));
        		message_send(address, message);
        	} else {
        		sprintf(message, "GOLD 0 %d %d", game_playerNugget(game, addresses[i]),game_nuggetsRemain(game));
        		message_send(addresses[i], message);
        	}
        }
    }
    mem_free(addresses);
	// update spectator if there's one
	addr_t spect = game_getSpecAddr(game);
	if (message_isAddr(spect)) {
		// send map
		char* mainGrid = game_getMainGrid(game);
		sprintf(mapStr, "DISPLAY\n%s", mainGrid);
		message_send(spect, mapStr);
		mem_free(mainGrid);
		// send GOLD info
		sprintf(message,"GOLD 0 0 %d", game_nuggetsRemain(game));
		message_send(spect, message);
	}
}

/**************** handleQuit ****************/
/* Handles QUIT messages
* First validate if the message is sent from a spectator, if yes
* quit that spectator;
* if not, quit that player;
* then update all clients' windows accordingly (the quit player's letter
* should be gone.)
*
* log error if NULL input.
*/
static void 
handleQuit(game_t* game, const addr_t address)
{
	if (game == NULL || !message_isAddr(address)) {
		log_v("handleQuit called with NULL arg");
		return;
	}
    // quit for the spectator
    // TO DO: IS THIS WORKING?
    addr_t spec = game_getSpecAddr(game);
    if (message_eqAddr(spec, address)) {
        printf("inloop");
        message_send(address, "QUIT Thanks for watching!");
        if (!game_quitSpectator(game)) {
            log_v("Quit spectator failed");
        }     
        return;
    }
    // If not the spectator quit, quit this player
	if (game_quitPlayer(game, address) == false) {
		log_c("quitting player %c failed", game_getPlayerChar(game, address));
	}

    char message[50];
    sprintf(message, "QUIT Thanks for playing!");
    message_send(address, message);
    int mapStrSize = game_gridxsize(game)*(game_gridysize(game)+1) + strlen("DISPLAY\n") + 1000;
    char mapStr[mapStrSize];
    char* playerGrid;

	// update every player's map
	addr_t* addresses = game_getAllAddr(game);
	for (int i = 0; i < MaxPlayers; i++) {

        if (message_isAddr(addresses[i])) {
        	playerGrid = game_getPlayerGrid(game, addresses[i]);
            sprintf(mapStr,"DISPLAY\n%s", playerGrid);
            message_send(addresses[i], mapStr);
            mem_free(playerGrid);
        }
    }
    mem_free(addresses);
	//update spectator
	addr_t spect = game_getSpecAddr(game);
	if (message_isAddr(spect)) {
        char* mainGrid = game_getMainGrid(game);
        sprintf(mapStr, "DISPLAY\n%s", mainGrid);
		message_send(spect, mapStr);
        mem_free(mainGrid);
	}
}

/**************** endGmae ****************/
/* Called when no nuggets left, end the entire game.
* Print game summary to all current alive clients
* Delete the game and grid modules
* shut done Message and Log modules
*
* log error if NULL input.
*/ 

static void 
endGame(game_t* game) 
{
	if (game == NULL) {
		log_v("endGame called with NULL arg");
	return;
	}

	//print summary to all clients
	addr_t* addresses = game_getAllAddr(game);
	int maxSumLen = 20 * MaxNameLength * MaxPlayers + sizeof(char)*(strlen("QUIT GAME OVER:\n") + 1);
	char message[maxSumLen];
    char* nuggetSumm;
    nuggetSumm = game_getPlayersPurse(game);
	for (int i = 0; i < MaxPlayers; i++) {
        if (message_isAddr(addresses[i])) {
        	sprintf(message, "QUIT GAME OVER:\n%s", nuggetSumm);
        	message_send(addresses[i], message);
        }
    }
    mem_free(addresses);

    addr_t spect = game_getSpecAddr(game);
	if (message_isAddr(spect)) {

		message_send(spect, message);
	}
	mem_free(nuggetSumm);
	// end the game and modules in use
	game_delete(game);
	message_done(); 
	log_done();
}

