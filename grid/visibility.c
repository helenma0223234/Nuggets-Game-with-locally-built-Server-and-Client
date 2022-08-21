/*
 * visibility.c - testing file for grid, specifically the visibility
 */

#include<stdio.h>
#include<stdlib.h>
#include"grid.h"
#include"file.h"

/********************** main *********************/
int
main(int argc, char* argv[])
{
    printf("Loading in challenge.txt...\n");
    FILE* fp = fopen("../maps/challenge.txt", "r");
    grid_t* grid = grid_new(fp);
    fclose(fp);

    FILE* fp2 = fopen("../maps/challenge.txt", "r");
    grid_t* origGrid = grid_new(fp2);
    fclose(fp2);

    if (grid == NULL || origGrid == NULL){
        fprintf(stderr, "Loading failed!\n");
    }

    grid_t* playerGrid = grid_emptyNew(grid_xsize(grid), grid_ysize(grid));

    printf("Testing with NULL grids...\n");
    grid_addVisible(NULL, origGrid, playerGrid, 13, 5);
    grid_addVisible(grid, NULL, playerGrid, 13, 5);
    grid_addVisible(grid, origGrid, NULL, 13, 5);

    printf("Testing with invalid coordinates...\n");
    grid_addVisible(grid, origGrid, playerGrid, -13, 5);
    grid_addVisible(grid, origGrid, playerGrid, 13, -5);

    grid_addVisible(grid, origGrid, playerGrid, 2000, 5);
    grid_addVisible(grid, origGrid, playerGrid, 13, 5000);

    printf("showing visibility for (13, 5)...\n");
    grid_addVisible(grid, origGrid, playerGrid, 13, 5);
    grid_print(playerGrid, stdout);

    printf("simulating move left by one...\n");
    grid_move(grid, 13, 5, 12, 5);
    grid_addVisible(grid, origGrid, playerGrid, 12, 5);
    grid_print(playerGrid, stdout);
    
    printf("simulating move down by one...\n");
    grid_move(grid, 12, 5, 12, 6);
    grid_addVisible(grid, origGrid, playerGrid, 12, 6);
    grid_print(playerGrid, stdout);

    
    printf("Base map...\n");

    grid_print(grid, stdout);
    printf("\n");


    printf("deleting grid...\n");
    grid_delete(grid);
    grid_delete(origGrid);
    grid_delete(playerGrid);
    return 0;
}


