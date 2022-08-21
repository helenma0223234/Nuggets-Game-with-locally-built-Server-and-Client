/* 
 * grid.h - header file for grid module
 *
 * A grid maintains a 2d-array where each coordinate
 * holds a char.
 *
 * So Amano, May 2021
 */

#ifndef __GRID_H
#define __GRID_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file.h"
#include <string.h>
#include <ctype.h>
#include <math.h>

/**************** global types ****************/
typedef struct grid grid_t;  // opaque to users of the module

/**************** functions ****************/

/****************** grid_xsize *****************/
/*
 * Getter function for the horizontal width of the grid
 *
 * Caller provides:
 *  Valid pointer to a grid
 * We return:
 *  0 if the grid is NULL or on error
 *  the horizontal size otherwise.
 */
int grid_xsize(grid_t* grid);

/****************** grid_ysize *****************/
/*
 * Getter function for the vertical size of the grid
 *
 * Caller provides:
 *  Valid pointer to a grid
 * We return:
 *  0 if the grid is NULL or on error
 *  the vertical size otherwise.
 */
int grid_ysize(grid_t* grid);

/**************** grid_new ****************/
/* Create a new grid.
 * 
 * Caller provides:
 *  the file pointer to the map.txt
 * We return:
 *   pointer to a new grid, or NULL if error.
 * We guarentee:
 *   where there are spaces in the map, there will be a space
 *   character, not NULL.
 * Caller is responsible for:
 *   later calling grid_delete.
 */
grid_t* grid_new(FILE* fp);

/**************** grid_emptyNew ******************/
/* Create a new empty grid.
 *
 * Caller provides:
 *  the horizontal and vertical size of the grid; both positive numbers
 * We return:
 *  pointer to a new empty grid, NULL if error
 * We guarentee:
 *  When we say "empty" we mean square has a space character, not NULL.
 * Caller is responsible for:
 *  Later calling grid_delete.
 */
grid_t* grid_emptyNew(int x_size, int y_size);

/***************** grid_get ******************/
/*
 * Find and return the character that the coordinate has/
 *
 * Caller provides:
 *  A valid grid and a valid x and y.
 * We return:
 *  the char that is in that coordinate
 *  NULL if grid is NULL, 0 > x,y, x,y > x_size, y_size or error.
 */
char grid_get(grid_t* grid, int x, int y);


/**************** grid_set ****************/
/*
 * Replaces whatever that is in that coordinate with item
 * Caller provides:
 *   valid grid pointer, valid char, 0 < x,y < x_size, y_size
 * We return:
 *  false if error, or if any of the input is NULL or invalid
 *  true on success
 * 
 */
bool grid_set(grid_t* grid, char item, int x, int y);

/**************** grid_print ****************/
/* Print the whole grid
 *
 * Caller provides:
 *   valid grid pointer,
 *   FILE open for writing,
 * We print:
 *   Print (null) if grid is NULL..
 *   print the grid in the following format if not NULL:
 *      [1st line]\n[2nd line]\n...\n[final line]
 * Notes:
 *   This is the function we use to send to clients.
 *   Useful for debugging as well
 */
void grid_print(grid_t* grid, FILE* fp);

/***************** grid_toString *****************/
/* Returns the whole grid as a string.
 *
 * Caller provides:
 *  valid grid pointer.
 * We return:
 *  Null if grid is NULL or on error.
 * Notes:
 *  The caller is expected to free the given string.
 */
char* grid_toString(grid_t* grid);


/**************** grid_delete ****************/
/* Delete grid, frees memory
 *
 * Caller provides:
 *   valid grid pointer,
 * We do:
 *   if grid==NULL, do nothing.
 */
void grid_delete(grid_t* grid);

/**************** grid_addVisible ****************/
/* Add visible part of the baseGrid to the destGrid. 
 *
 * Caller provides:
 *   valid grid pointers, x_s, y_s > 0
 * We do:
 *   Nothing on error or invalid inputs
 *   Add in parts of the baseGrid that should be visible from (x_s, y_s) into destGrid.
 * Notes:
 *   An example of using this to move a player in an actual game is as follows:
 *   call grid_set on the main grid to move player.
 *   call grid_set on the player grid to move player.
 *   call addVisible for the two grids
 *   Note that it WILL NOT update the given x_s, y_s coordinate, to adhere to the
 *   requirement of using @ for displaying the player's self-position
 */
void grid_addVisible(grid_t* baseGrid, grid_t* originalGrid, grid_t* destGrid, int x_s, int y_s);


#endif // __GRID_H
