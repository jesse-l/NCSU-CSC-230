/**
  @file map.c
  @author Jesse Liddle (jaliddl2)

  This function is responsible for handling the parts of
  the program that deal with the map of this program.
*/

#define INITIAL_MAP_SIZE 3

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "map.h"

/**
  This function is used to make the original 3x3
  map for the start of the game.
  
  @param rows      This is the number of rows that are
                     in the map.
*/
char **initMap( int *rows ) {
  // Allocate enough space to store a pointer for every row.
  *rows = INITIAL_MAP_SIZE;
  char **map = (char **) malloc( *rows * sizeof( char * ) );
  // Fill in the rows with 3-character strings of spaces.
  for ( int i = 0; i < *rows; i++ ) {
    map[ i ] = (char *)malloc( INITIAL_MAP_SIZE + 1 );
    strcpy( map[ i ], "   " );
  }
  // Return the new map.
  return map;
}

/**
  This function is used to free the dynamically
  allocated memory used to create the map.  This
  function is only called on a successful exit.
  
  @param **map     This is the pointer that points to
                     the map in memory.
  @param rows      This is the number of rows that are
                     in the map.
*/
void freeMap( char **map, int rows )
{
  for ( int i = 0; i < rows; i++ )
    free( map[i] );
  free( map );
}

/**
  This function is used to show the map that the
  player has explored.  This function will print
  to standard output. This will include showing
  where the player is and his/her direction as
  <,^,>,v.
  
  @param **map     This is the pointer that points to
                     the map in memory.
  @param rows      This is the number of rows that are
                     in the map.
  @param rowPos    This is the row position that the
                     player is currently in.
  @param colPos    This is the column position that the
                     player is currently in.
  @param dir       This is the direction the player is
                     currently going in.
*/
void showMap( char **map, int rows, int rowPos,
    int colPos, int dir )
{
  char player;
  //Finds players direction.
  if ( dir == 0 )
    player = '^';
  else if ( dir == 90 || dir ==-270 )
    player = '>';
  else if ( dir == 180 || dir == -180 )
    player = 'V';
  else if ( dir == 270 || dir == -90 )
    player = '<';
  //Prints top of map
  printf( "+" );
  for ( int i = 0; map[ 0 ][ i ]; i++ )
    printf( "-" );
  printf( "+\n" );

  //Prints middle part of map
  for ( int k = 0; k < rows; k++ ) {
    printf( "|" );
    for ( int i = 0; map[ k ][ i ]; i++ ) {
      if ( k == rowPos && i == colPos )
        printf( "%c", player );
      else
        printf( "%c", map[ k ][ i ] ); 
    }
    printf( "|\n" );
  }
  
  //Prints bottom of map
  printf( "+" );
  for ( int i = 0; map[ 0 ][ i ]; i++ )
    printf( "-" );
  printf( "+\n" );
}

/**
  This function provides the ability to expand the map
  as the player explores the world.  It is passed the 
  current map array and expands it.

  Behavior is undefined if number of extraCols or extraRows
  is negative or if shiftCols or shiftRows is greater than
  extraCols or extraRows.************************
  
  @param **map     This is the pointer that points to
                     the map in memory.
  @param rows      This is the number of rows that are
                     in the map.
  @param extraRows This is for the number of extra rows
                     that are being added into the map
  @param extraCols This is for the number of extra columns
                     that are being added into the map
  @param shiftRows This is for the number of rows going
                     on the top of the map
  @param shiftCols This is for the number of columns going
                     on the left side of the map.
*/
char **expandMap( char **map, int *rows, int extraRows,
    int extraCols, int shiftRows, int shiftCols )
{

  int counter = 0;
  //Finds number of col in the map
  while ( map[ 0 ][ counter ] != '\0' ) {
    counter++;
  }
  
  int newRows = *rows + extraRows + shiftRows + 1;
  int newCols = counter + extraCols + shiftCols + 1;
  
  char **newMap = (char **)malloc( newRows * sizeof(char * ));
  
  //Creates the new map and fills with spaces.
  for ( int i = 0; i < newRows; i++ ) {
    newMap[ i ] = (char *)malloc( newCols * sizeof(char *) );
    for ( int k = 0; k < newCols; k++ ) {
      newMap[ i ][ k ] = ' ';
    }
    
    if ( i == newRows - 1 )
      break;
  }
  
  //Loop for copying the one array to the other with offset
  for ( int i = 0; i < *rows; i++ ) {
    for ( int k = 0; k < counter; k++ ) {
      newMap[ k + shiftCols ][ i + shiftRows ] = map[ i ][ k ];
    }
  }
  
  //Free memory from old map
  freeMap( map, *rows );

  rows = &newRows;
  
  //Sets old map pointer to the newMap address
  map = newMap;
  
  freeMap( newMap, *rows );
  //Returns the map pointer
  return map;
}
