/**
  @file map.h
  @author Jesse Liddle (jaliddl2)

  This is the header file for the map.c
  file and allows the explorer.c function
  to access everything in the map.c function.
*/

/**
  This function is used to make the original 3x3
  map for the start of the game.
  
  @param rows      This is the number of rows that are
                     in the map.
*/
char **initMap( int *rows );

/**
  This function is used to free the dynamically
  allocated memory used to create the map.  This
  function is only called on a successful exit.
  
  @param **map     This is the pointer that points to
                     the map in memory.
  @param rows      This is the number of rows that are
                     in the map.
*/
void freeMap( char **map, int rows );

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
    int colPos, int dir );

/**
  This function provides the ability to expand the map
  as the player explores the world.  It is passed the
  current map array and expands it.
  
  Behavior is undefined if number of extraCols or extraRows
  is negative or if shiftCols or shiftRows is greater than
  extraCols or extraRows.
  
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
    int extraCols, int shiftRows, int shiftCols );
