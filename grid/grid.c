/* 
 * grid.c - implementation of grid.h
 *
 * A *grid* maintains a 2d-array. This acts as a grid
 * where each coordinate holds a char
 *
 * So Amano, May 2021
 */


#include <stdio.h>
#include <stdbool.h>
#include "file.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/**************** global types ****************/
typedef struct grid{
    char** array;
    int x_size;
    int y_size;
}grid_t;

/**************** prototype static functions ****************/
static bool isVisible(grid_t* grid, int x_s, int y_s, int x_g, int y_g);
static void setGridEmpty(grid_t* originalGrid, grid_t* destGrid, int x, int y);
/**************** functions ******************/

/**************** grid_xsize ***************/
/*
 * See details in grid.h.
 */
int grid_xsize(grid_t* grid){
    if (grid == NULL){
        return 0;
    } else{
        return grid->x_size;
    }
}

/**************** grid_ysize ******************/
/*
 * See details in grid.h
 */
int grid_ysize(grid_t* grid){
    if (grid == NULL){
        return 0;
    } else{
        return grid->y_size;
    }
}
    
/**************** grid_new ****************/
/*
 * See details in grid.h.
 */
grid_t* grid_new(FILE* fp)
{
    if (fp == NULL){
        return NULL;
    }

    grid_t* grid = malloc(sizeof(grid_t));

    // Read file to find y size
    int r = file_numLines(fp);

    // Read file to find x size
    int c = 0;
    char curr;
    while ((curr = fgetc(fp)) != '\n'){
        c += 1;
    }
    // Allocate appropriate space
    grid->array = (char**)calloc(c+1, sizeof(char*));
    grid->x_size = c;
    grid->y_size = r;

    // Allocate space for inner arrays
    for (int i = 0; i < c; i++){
        char** outer = grid->array;
        char* inner = (char*)calloc(r+1, sizeof(char));
        outer[i] = inner;
    }
    rewind(fp);
    
    int i = 0;
    // Read and load in the map
    while (!feof(fp) && i < r){
        char** outer = grid->array;

        for (int j = 0; ; j++){

            char* inner = outer[j];
            if ((curr = fgetc(fp)) == '\n' || feof(fp)){ //Next row
                break;
            }
            inner[i] = curr;
        }
        i += 1;
    }    
    return grid;
}
/**************** grid_emptyNew ****************/
/*
 * See details in grid.h.
 */
grid_t* grid_emptyNew(int x_size, int y_size)
{
    if (x_size < 0 || y_size < 0){
        return NULL;
    }

    grid_t* grid = malloc(sizeof(grid_t));

    int r = y_size ;
    int c = x_size;
    // Allocate appropriate space
    grid->array = (char**)calloc(2*c, sizeof(char*));
    grid->x_size = c;
    grid->y_size = r;

    // Allocate space for inner arrays
    for (int i = 0; i < c; i++){
        char** outer = grid->array;
        char* inner = (char*)calloc(2*r, sizeof(char));
        outer[i] = inner;
    }
    int i = 0;
    while (i < r){
        char** outer = grid->array;

        for (int j = 0; ; j++){
            char* inner = outer[j];
            if (j >= c){ //Next row
                break;
            }
            inner[i] = ' ';
        }
        i += 1;
    }
    return grid;
}
/***************** grid_get ******************/
/*
 * See details in grid.h.
 */
char grid_get(grid_t* grid, int x, int y)
{
    if (grid == NULL || x < 0 || y < 0 || x >= grid->x_size || y >= grid->y_size){
        fprintf(stderr, "invalid input for grid_get (%d, %d)!\n", x, y);
        return ' ';
    }
    // Get coordinate and return item
    char** outer = grid->array;
    char* inner = outer[x];
    char item = inner[y];
    return item;
}

/**************** grid_set ****************/
/* 
 * See details in grid.h.
 */
bool grid_set(grid_t* grid, char item, int x, int y)
{
    if (grid == NULL || x < 0 || y < 0 || 
            x >= grid->x_size || y >= grid->y_size){
        fprintf(stderr, "invalid input for grid_set!\n");
        return false;
    }

    // Change item in x,y
    char** outer = grid->array;
    char* inner = outer[x];
    inner[y] = item;
    
    return true;
}

/**************** grid_print ****************/
/*
 * See details in grid.h.
 */
void grid_print(grid_t* grid, FILE* fp)
{
    if (grid == NULL || fp == NULL){
        fprintf(stderr, "invalid input for grid_print!\n");
        return;
    }

    //Print the grid out for each x, then for each y (as a grid)
    char** outer = grid->array;
    for (int i = 0; i < (grid->y_size); i++){

        for (int j = 0; j < (grid->x_size); j++){
            char* inner = outer[j];
            fprintf(fp, "%c", inner[i]);
        }
        fprintf(fp, "\n");
    }
}

/**************** grid_toString ****************/
/*
 * See details in grid.h.
 */
char* grid_toString(grid_t* grid)
{
    if(grid == NULL){
        return NULL;
    }
    char* string = calloc(2 * grid->x_size * (grid->y_size + 1), sizeof(char)) ;
    char** outer = grid->array;
    for (int i = 0; i < (grid->y_size); i++){

        for (int j = 0; j < (grid->x_size); j++){
            char* inner = outer[j];
            char* adding = calloc(2, sizeof(char));
            sprintf(adding, "%c", inner[i]);
            strcat(string, adding);

            if (adding != NULL){
                free(adding);
            }
        }
        strcat(string, "\n");
    }
    return string;
}

/**************** grid_move ****************/
/* 
 * See details in grid.h 
 */
void grid_move(grid_t* grid, int x_start, int y_start, int x_dest, int y_dest)
{
    if(grid == NULL || x_start < 0 || y_start < 0 || x_dest < 0 || y_dest < 0
            || x_start >= grid->x_size || y_start >= grid->y_size || 
            x_dest >= grid->x_size || y_dest >= grid->y_size){
        fprintf(stderr, "invalid input for grid_move!\n");
        return;
    }
    char** outer = grid->array;
    char* inner = outer[x_start];
    char item = inner[y_start];
    grid_set(grid, item, x_dest, y_dest); //Move item
    grid_set(grid, '.', x_start, y_start); //Empty prev space
}

/**************** grid_delete ****************/
/* 
 * See details for grid.h
 */
void grid_delete(grid_t* grid)
{
    if(grid == NULL){
        return;
    }

    char** outer = grid->array;
    for (int i = 0; i < grid->x_size; i++){
        free(outer[i]);
    }
    free(outer);
    free(grid);
}

/********************* grid_addVisible *************************/
/* 
 * See details for grid.h
 */
void grid_addVisible(grid_t* baseGrid, grid_t* originalGrid, grid_t* destGrid, int x_s, int y_s){

    if(baseGrid == NULL || destGrid == NULL || originalGrid == NULL || x_s < 0 || y_s < 0
            || x_s >= baseGrid->x_size || y_s >= baseGrid->y_size){
        return;
    }
    int x_size = grid_xsize(baseGrid);
    int y_size = grid_ysize(baseGrid);
    setGridEmpty(originalGrid, destGrid, x_s, y_s);
    // Iterate over all points
    for (int i = 0; i < x_size; i++){
        for (int j = 0; j < y_size; j++){
            if (isVisible(baseGrid, x_s, y_s, i, j)){ // If visible, update everything
                char item = grid_get(baseGrid, i, j);
                grid_set(destGrid, item, i, j);
            }
        }
    }
}

/************************* setGridEmpty ***************************/
/*
 * Helper function for grid_addVisible used to set the grid back to the original grid
 * Allows a player to "forget".
 */
static void
setGridEmpty(grid_t* originalGrid, grid_t* destGrid, int x, int y) 
{
    if (originalGrid == NULL || destGrid == NULL || x < 0 || y < 0
            || x >= originalGrid->x_size || y >= originalGrid->y_size){
        return;
    }

    int x_size = grid_xsize(originalGrid);
    int y_size = grid_ysize(originalGrid);
    for (int i = 0; i < x_size; i++) {
        for (int j = 0; j < y_size; j++) {
            if (x == i && y == j) {
                continue;
            }
            if (grid_get(destGrid, i, j) != ' ') {
                char current = grid_get(originalGrid, i, j);
                grid_set(destGrid, current, i, j);
            }
        }
    }
}


/*********************** isSeeThrough **********************/
/*
 * Helper function for isBlocked. 
 * Returns whether a character should be "see-through"
 */
static bool
isSeeThrough(char item)
{
    return (item == '.' || item == '*' || isalpha(item));
}


/*********************** isNotBlocked ************************/
/*
 * Helper function for isVisible. 
 * Checks whether the line of vision between two places is blocked
 *
 * x_c, y_c and x_g, y_g are the two "places"
 * x_step, y_step is the increments that are checked whether a wall exists and such
 */
static bool
isNotBlocked(grid_t* grid, double x_c, double y_c, int x_g, int y_g, double x_step, double y_step)
{
    if(grid == NULL || x_c < 0 || y_c < 0 || x_g < 0 || y_g < 0){
        return false;
    }

    while (abs(x_c-x_g) > 0.001 || abs(y_c-y_g) > 0.001){ // To account for 0.9999999
        if (y_c != floor(y_c) || y_c != ceil(y_c)){ // If non-integer
            char item1 = grid_get(grid, x_c, floor(y_c));
            char item2 = grid_get(grid, x_c, ceil(y_c));
            if (!isSeeThrough(item1) && !isSeeThrough(item2)) { //Check both
                return false;
            }
        }
        else if (x_c != floor(x_c) || x_c != ceil(x_c)){ // If non-integer for x
            char item1 = grid_get(grid, floor(x_c), y_c);
            char item2 = grid_get(grid, ceil(x_c), y_c);
            if (!isSeeThrough(item1) && !isSeeThrough(item2)){
                return false;
            }
        }
        else{
            char item = grid_get(grid, x_c, y_c);
            if (!isSeeThrough(item)){
                return false;
            }
        }
        x_c += x_step;
        y_c += y_step;
    }
    return true;
}


/********************** isVisible ***********************/
/*
 * Helper function for grid_addVisible
 * Simply returns whether a point should be visible from another.
 * x_s,y_s is the "start" point, and x_g, y_g is "goal" point.
 * The notion of "start" and "goal" is conceptual. There should be
 * no difference even if a user flips them.
 */
static bool 
isVisible(grid_t* grid, int x_s, int y_s, int x_g, int y_g)
{
    if (grid == NULL || x_s < 0 || y_s < 0 || x_g < 0 || y_g < 0
            || x_s >= grid->x_size || y_s >= grid->y_size
            || x_g >= grid->x_size || y_g >= grid->y_size){
        return false;
    }

    if (x_s == x_g && y_s == y_g){
        return false;
    }
    int x_d = x_s - x_g;
    int y_d = y_s - y_g;

    // First is loop with y_step = 1
    double x_step = 0;
    double y_step = 1; 
    if (y_d != 0){
        x_step = (double)abs(x_d)/abs(y_d);
    } 

    if (x_d > 0){ // Goal is left of start
        x_step *= -1;
    }
    if (y_d > 0){ // Goal is above start
        y_step *= -1;
    }

    double x_c = x_s + x_step; //Exclusive as per requirement spec
    double y_c = y_s + y_step;
    
    // y_d != 0 since that will be checked in second call of isNotBlocked
    if (y_d != 0 && !isNotBlocked(grid, x_c, y_c, x_g, y_g, x_step, y_step)){
        return false;
    }
    
    // Second pass is loop with x_step = 1
    x_step = 1;
    if (x_d != 0){ 
        y_step = (double)abs(y_d)/abs(x_d);
    }
    
    if (x_d > 0){
        x_step *= -1;
    }
    if (y_d > 0){
        y_step *= -1;
    }

    x_c = x_s + x_step; 
    y_c = y_s + y_step;    
   
    if (x_d != 0 && !isNotBlocked(grid, x_c, y_c, x_g, y_g, x_step, y_step)){
        return false;
    }
    return true;
}

