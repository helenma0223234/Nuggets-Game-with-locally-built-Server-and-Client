/*
 * gametest.c - testing file for gameer, loads in an game
 *               and outputs the same game into a file.
 *
 * Usage: gameer oldIndexFilename newIndexFilename
 */

#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "mem.h"

/********************** main *********************/
int
main(int argc, char* argv[])
{
    printf("Loading in big.txt...\n with 25 gold\n");
    game_t* game = game_new("../maps/narrow.txt", 25, 8);
    if (game == NULL){
        fprintf(stderr, "Loading failed, exceeding the number of piles");
    }

    printf("Testing game_addPlayer...\n");
    printf("With NULL game...\n");
    addr_t add = message_noAddr();
    addr_t add2 = message_noAddr(); 
    message_setAddr("80.229.166.8", "40996", &add2);

    if (!game_addPlayer(NULL, "food", add)){
        printf("Correctly returned false!\n");
    }
    printf("printing the main grid...\n");
    char* maingrid = game_getMainGrid(game);
    printf("%s", maingrid);
    mem_free(maingrid);
    
     
    printf("add player With valid character, valid game...\n");
    if (game_addPlayer(game, "WHOA", add)){
        printf("Player added!\n");
    }
    printf("printing the grid for the player..\n");
    char* playergrid = game_getPlayerGrid(game, add);
    printf("%s", playergrid);
    mem_free(playergrid);

    printf("printing the grid for the player just being added...\n");
    char* string = game_getPlayerGrid(game, add);
    printf("%s", string);
    mem_free(string);
    
    printf("\nmove to right....\n");
    game_movePlayer(game, add, 'l');
    printf("Printing player's updated grid, suppose to go right by one slot...\n");
    
    string = game_getPlayerGrid(game, add);
    printf("%s", string);
    mem_free(string);

    printf("Printing the update main grid..., the player A should go right by one slot\n");
    char* mainGrid = game_getMainGrid(game);
    printf("%s", mainGrid);
    mem_free(mainGrid);

    printf("\nmove up by 6....\n");
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    printf("Printing player's map...\n");
    
    string = game_getPlayerGrid(game, add);
    printf("%s", string);
    mem_free(string);
    printf("Printing the update main grid..., the player A should go right by one slot\n");
    mainGrid = game_getMainGrid(game);
    printf("%s", mainGrid);
    mem_free(mainGrid);


    printf("\nmove down by 3...\n");
    game_movePlayer(game, add, 'k');
    game_movePlayer(game, add, 'k');
    game_movePlayer(game, add, 'k');
    printf("Printing player's map...\n");
    
    string = game_getPlayerGrid(game, add);
    printf("%s", string);
    mem_free(string);


    printf("\nmove right by 4..\n");
    game_movePlayer(game, add, 'l');
    game_movePlayer(game, add, 'l');
    game_movePlayer(game, add, 'l');
    game_movePlayer(game, add, 'l');
    printf("Printing player's map...\n");
    

    
    string = game_getPlayerGrid(game, add);
    printf("%s", string);
    mem_free(string);

    printf("\nmoving right by 8..\n");
    game_movePlayer(game, add, 'l');
    game_movePlayer(game, add, 'l');
    game_movePlayer(game, add, 'l');
    game_movePlayer(game, add, 'l');
    game_movePlayer(game, add, 'l');
    game_movePlayer(game, add, 'l');
    game_movePlayer(game, add, 'l');
    game_movePlayer(game, add, 'l');
    
    printf("Printing player's map...\n"); 
    string = game_getPlayerGrid(game, add);
    printf("%s", string);
    mem_free(string);

    printf("\nmoving left by 13..\n");
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    
    printf("Printing player's map...\n"); 
    string = game_getPlayerGrid(game, add);
    printf("%s", string);
    mem_free(string);

    printf("\nmoving left by 10..\n");
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
    game_movePlayer(game, add, 'j');
       
    printf("Printing player's map...\n"); 
    string = game_getPlayerGrid(game, add);
    printf("%s", string);
    mem_free(string);

    printf("\nmoving left by 30..\n");
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');
    game_movePlayer(game, add, 'h');

    
    printf("Printing player's map...\n"); 
    string = game_getPlayerGrid(game, add);
    printf("%s", string);
    mem_free(string);
    
    printf("\nmoving down by 10..\n");
    game_movePlayer(game, add, 'k');
    game_movePlayer(game, add, 'k');
    game_movePlayer(game, add, 'k');
    game_movePlayer(game, add, 'k');
    game_movePlayer(game, add, 'k');
    game_movePlayer(game, add, 'k');
    game_movePlayer(game, add, 'k');
    game_movePlayer(game, add, 'k');
    game_movePlayer(game, add, 'k');
    game_movePlayer(game, add, 'k');


    printf("Printing player's map...\n"); 
    string = game_getPlayerGrid(game, add);
    printf("%s", string);
    mem_free(string);

    printf("Running up...\n");
    game_movePlayer(game, add, 'J');
    string = game_getPlayerGrid(game, add);
    printf("%s", string);
    mem_free(string);

    game_addPlayer(game, "ffod", add2);

    char* nuggets = game_getPlayersPurse(game);
    printf("%s\n", nuggets);
    mem_free(nuggets);

    
    
    game_quitPlayer(game, add);
    printf("Printing main grid...\n");
    mainGrid = game_getMainGrid(game);
    printf("%s", mainGrid);
    mem_free(mainGrid);

    printf("deleting game...\n");
    game_delete(game);
    return 0;
}
