/*
 * gridtest.c - testing file for grider, loads in an grid
 *
 */

#include<stdio.h>
#include<stdlib.h>
#include"grid.h"
#include"file.h"

/********************** main *********************/
int
main(int argc, char* argv[])
{
    printf("Loading in big.txt...\n");
    FILE* fp = fopen("../maps/big.txt", "r");
    grid_t* grid = grid_new(fp);
    if (grid == NULL){
        fprintf(stderr, "Loading failed!\n");
    }
    printf("Here is the map:\n");
    grid_print(grid, stdout);
    printf("\n");
    printf("Size of map is x: %d, y: %d\n", grid_xsize(grid), grid_ysize(grid));

    printf("Testing grid_get...\n");
    printf("With NULL grid...\n");
    char item = grid_get(NULL, 10, 20);
    printf("returned %c\n", item);
    
    printf("With negative x...\n");
    item = grid_get(grid, -4, 20);
    printf("returned %c\n", item);

    printf("With large x...\n");
    item = grid_get(grid, 20000, 20);
    printf("returned %c\n", item);

    printf("With negative y...\n");
    item = grid_get(grid, 5, -10);
    printf("returned %c\n", item);

    printf("With large y...\n");
    item = grid_get(grid, 4, 20000);
    printf("returned %c\n", item);

    printf("Valid request, should return '#'...\n");
    item = grid_get(grid, 11, 2);
    printf("returned %c\n", item);

    printf("Testing grid_set next...\n");
    printf("With NULL grid...\n");
    grid_set(NULL, '@', 3, 1);

    printf("With negative x...\n");
    grid_set(grid, '@', -4, 20);

    printf("With large x...\n");
    grid_set(grid, '@', 20000, 20);

    printf("With negative y...\n");
    grid_set(grid, '@', 5, -10);

    printf("With large y...\n");
    grid_set(grid, '@', 4, 20000);

    printf("Valid request...\n");
    grid_set(grid, '@', 3, 1);

    printf("printing map, should see an @ mark at (3,1)...\n");
    grid_print(grid, stdout);
    printf("\n");

    printf("test grid_toString as well...\n");
    char* string = grid_toString(grid);
    printf("%s\n", string);
    free(string);

    printf("deleting grid...\n");
    grid_delete(grid);
    fclose(fp);
    return 0;
}


