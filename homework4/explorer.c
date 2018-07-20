/**
  @file explorer.c
  @author Jesse Liddle (jaliddl2)

  This is the main function of the explorer program.
  This program calls the functions that are defined in
  the map.c function and uses them to generate the map
  for the game.  This function is responsible for checking
*/

#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include <string.h>
#include <ctype.h>

//Defines number of starting rows.
#define startingRows 3

//Counter for rows in the array.
int rows = 0;

//Starting point of the player.
int startingRow = 1;
int startingCol = 1;

//Int for the directions
#define FORWARD 0
#define LEFT -90
#define RIGHT 90
#define QUIT "quit"
#define FORWARDST "forward"
#define LEFTST "left"
#define RIGHTST "right"
#define RESET 0
#define FRONTZ 0
#define FRONTP 360
#define FRONTN -360
#define LEFTP 270
#define LEFTN -90
#define DOWNP 180
#define DOWNN -180
#define RIGHTP 90
#define RIGHTN -270

//Defines extra functions inside explorer.c
int commandLine( char **map, int *playerPos[2], int *rows );
int scriptInput(  char **map, FILE *input, int *playerPos[2], int *rows );
void updateMap( char **map, int *rows, int direction, int *pPos[2],
    char c1, char c2, char c3, int mov );
void playerMove( char **map, int *pPos[2], int dir, int *rows );
int checkMap( char **map, int x, int y, char c );

/**
  This function is called if there is no command line input entered when starting
  this program.  This function handles all the user input that come in through the
  console.
  
  @param **map Is the pointer to a pointer of the map.
*/
int commandLine( char **map, int *playerPos[2], int *rows )
{
  //Creates initial items to be used later
  char c1;
  char c2;
  char c3;
  char *command[20];
  int direction = FORWARD;//Starts player facing forward

  //Gets the first line of input and updates the map
  scanf( " %c %c %c", &c1, &c2, &c3 );
  updateMap( map, rows, direction, playerPos, c1, c2, c3, 0);
  
  //Loop for running rest of the map creation
  while ( strcmp( *command, QUIT) != 0 ) {
    scanf( " %s %c %c %c", *command, &c1, &c2, &c3 );
    
    if ( strcmp( *command, FORWARDST ) == 0 ) {
      direction += FORWARD;
      updateMap( map, rows, direction, playerPos, c1, c2, c3, 1 );
    } else if ( strcmp( *command, LEFTST ) == 0 ) {
      direction -= LEFT;
      updateMap( map, rows, direction, playerPos, c1, c2, c3, 0 );
    } else if ( strcmp( *command, RIGHTST ) == 0 ) {
      direction += RIGHT;
      updateMap( map, rows, direction, playerPos, c1, c2, c3, 0 );
    } else if ( strcmp( *command, QUIT ) == 0 ) {
      break;
    } else {
      fprintf( stderr, "Invalid Input." );
    }
    
  }
  
  freeMap( map, *rows );
  return 0;
}

/**
  This function is called if there is a file the the input is read from to generate
  the map for the dungeon.
*/
int scriptInput(  char **map, FILE *input, int *playerPos[2], int *rows )
{
  //Creates initial items to be used later
  char c1 = ' ';
  char c2 = ' ';
  char c3 = ' ';
  char command[20];
  int direction = FORWARD;//Starts player facing forward
  
  //Gets the first line of input and updates the map
  fscanf( input, " %c %c %c", &c1, &c2, &c3 );
  updateMap( map, rows, direction, playerPos, c1, c2, c3, 0);

  fscanf( input, "%19s %c %c %c", command, &c1, &c2, &c3 );

  //Loop for running rest of the map creation
  while ( strcmp( command, QUIT) != 0 ) {
  
    if ( strcmp( command, FORWARDST ) == 0 ) {
      direction += FORWARD;
      updateMap( map, rows, direction, playerPos, c1, c2, c3, 1 );
    } else if ( strcmp( command, LEFTST ) == 0 ) {
      direction += LEFT;
      updateMap( map, rows, direction, playerPos, c1, c2, c3, 0 );
    } else if ( strcmp( command, RIGHTST ) == 0 ) {
      direction += RIGHT;
      updateMap( map, rows, direction, playerPos, c1, c2, c3, 0 );
    } else if ( strcmp( command, QUIT ) == 0 ) {
      break;
    } else {
      fprintf( stderr, "Invalid command\n" );
      return 0;
    }
    
    
    fscanf( input, " %19s %c %c %c", command, &c1, &c2, &c3 );
  }

  freeMap( map, *rows );
  return 0;
}

/**
  This method is used to update the map with the new characters
  and the player movement.
  
  @param **map The current map that is being checked.
  @param *rows The current count of rows
  @param direction The direction the player is facing.
  @param *pPos[2] The current position of the player.
  @param c1 The first character to be added.
  @param c2 The second character to be added.
  @param c3 The third character to be added.
  @param mov 1 if player is moving forward or 0 if just turning.
*/
void updateMap( char **map, int *rows, int direction, int *pPos[2],
    char c1, char c2, char c3, int mov )
{  
  //Resets direction to 0 when it hits 360 or -360
  if ( direction == FRONTP || direction == FRONTN )
    direction = RESET;
  //Checks to see if player moved or not
  if ( mov == 1 ) {
    playerMove( map, pPos, direction, rows );
  }

  //Changes the characters on the map
  if ( direction == FRONTZ ) {
    //Checks input of c1
    if ( checkMap( map, (*pPos[0]-1), (*pPos[1]-1), c1 ) == 1 ) {
      //Checks input of c2
      if ( checkMap( map, (*pPos[0]), (*pPos[1]-1), c2 ) == 1 ) {
        //Checks input of c3
        if ( checkMap( map, (*pPos[0]+1), (*pPos[1]-1), c3 ) == 1 ) {
          map[ *pPos[1]-1 ][ *pPos[0]-1 ] = c1;
          map[ *pPos[1]-1 ][ *pPos[0] ] = c2;
          map[ *pPos[1]-1 ][ *pPos[0]+1 ] = c3;
        } else {
          fprintf( stderr, "Inconsistent map\n" );
        }
      } else {
        fprintf( stderr, "Inconsistent map\n" );
      }
    } else {
      fprintf( stderr, "Inconsistent map\n" );
    }  
  } else if ( direction == LEFTP || direction == LEFTN ) {
    //Checks input of c1
    if ( checkMap( map, (*pPos[0]-1), (*pPos[1]+1), c1 ) == 1 ) {
      //Checks input of c2
      if ( checkMap( map, (*pPos[0]-1), (*pPos[1]), c2 ) == 1 ) {
        //Checks input of c3
        if ( checkMap( map, (*pPos[0]-1), (*pPos[1]-1), c3 ) == 1 ) {
          map[ *pPos[1]+1 ][ *pPos[0]-1 ] = c1;
          map[ *pPos[1] ][ *pPos[0]-1 ] = c2;
          map[ *pPos[1]-1 ][ *pPos[0]-1 ] = c3;
        } else {
          fprintf( stderr, "Inconsistent map\n" );
        }
      } else {
        fprintf( stderr, "Inconsistent map\n" );
      }
    } else {
      fprintf( stderr, "Inconsistent map\n" );
    }
  } else if ( direction == DOWNP || direction == DOWNN ) {
    //Checks input of c1
    if ( checkMap( map, (*pPos[0]+1), (*pPos[1]+1), c1 ) == 1 ) {
      //Checks input of c2
      if ( checkMap( map, (*pPos[0]), (*pPos[1]+1), c2 ) == 1 ) {
        //Checks input of c3
        if ( checkMap( map, (*pPos[0]-1), (*pPos[1]+1), c3 ) == 1 ) {
          map[ *pPos[1]+1 ][ *pPos[0]+1 ] = c1;
          map[ *pPos[1]+1 ][ *pPos[0] ] = c2;
          map[ *pPos[1]+1 ][ *pPos[0]-1 ] = c3;
        } else {
          fprintf( stderr, "Inconsistent map\n" );
        }
      } else {
        fprintf( stderr, "Inconsistent map\n" );
      }
    } else {
      fprintf( stderr, "Inconsistent map\n" );
    }== RIGHTN ) {
    //Checks input of c1
    if ( checkMap( map, (*pPos[0]+1), (*pPos[1]+1), c1 ) == 1 ) {
      //Checks input of c2
      if ( checkMap( map, (*pPos[0]+1), (*pPos[1]), c2 ) == 1 ) {
        //Checks input of c3
        if ( checkMap( map, (*pPos[0]+1), (*pPos[1]+1), c3 ) == 1 ) {
          map[ *pPos[1]-1 ][ *pPos[0]+1 ] = c1;
          map[ *pPos[1] ][ *pPos[0]+1 ] = c2;
          map[ *pPos[1]+1 ][ *pPos[0]+1 ] = c3;
        } else {
          fprintf( stderr, "Inconsistent map\n" );
        }
      } else {
        fprintf( stderr, "Inconsistent map\n" );
      }
    } else {
      fprintf( stderr, "Inconsistent map\n" );
    }
  }
  
  showMap( map, *rows, *pPos[0], *pPos[1], direction );
}

/**
  This function is used to check the map's contents at a
  specific location so that it does not overwrite anything that
  is already there.
  
  @param **map The current map that is being checked.
  @param x The x position of the character being checked
  @param y The y position of the character being checked
  @param c The character being checked
*/
int checkMap( char **map, int x, int y, char c )
{
  char *p = &c;
  if ( map[y][x] == ' ' )
    return 1;
  else if ( map[y][x] == *p )
    return 1;
  else
    return 0;
}

/**
  This function actually moves the character forward depending
  on the direction the character is facing currently.
  
  @param **map The current map that is being checked.
  @param *pPos[2] The current position of the player.
  @param dir The direction the player is facing.
  @param *rows The current count of rows.
*/
void playerMove( char **map, int *pPos[2], int dir, int *rows )
{ 
  int temp = 0;
  //Checks the players direction and moves accordingly.
  
  if ( dir == FRONTZ ) {
    //Increases the y value of the player position
    temp = *pPos[1];
    *pPos[1] = temp + 1;
    //map = expandMap( map, rows, extraRows, extraCols, shiftRows, shiftCols );
    map = expandMap( map, rows, 0, 0, 1, 0 );
  } else if ( dir == LEFTP || dir == LEFTN ) {
    //Decreases the x value of the player position
    temp = *pPos[0];
    *pPos[0] = temp - 1;
    map = expandMap( map, rows, 0, 0, 0, 1 );
  } else if ( dir == RIGHTP || dir == RIGHTN ) {
    //Increases the x value of the player position
    temp = *pPos[0];
    *pPos[0] = temp + 1;
    map = expandMap( map, rows, 0, 1, 0, 0 );
  } else if ( dir == DOWNP || dir == DOWNN ) {
    //Decreases the y value of the player position
    temp = *pPos[1];
    *pPos[1] = temp - 1;
    map = expandMap( map, rows, 1, 0, 0, 0 );
  }
}

/**
  This is the main function of the program.  This is the first function
  called when starting the explorer program.  This function can be called
  with or without command line parameters, but only one.  The command line
  input that can be used is a script file for movement of the character.  If
  no file is entered then the user will enter input through standard input.
  
  @param argc The count of arguments passed in through the console
  @param argv The arguments passed in through the console as an array
*/
int main( int argc, char *argv[] )
{
  int returnV = 0;//Value of exit status
  int rows = 0;
  int *rowsP = &rows;
  //The 1D array of arrays to characters
  char **map = initMap( rowsP );
  int *playerPos[2];
  playerPos[ 0 ] = &startingRow;
  playerPos[ 1 ] = &startingCol;
  
  if ( argc <= 1 )
    returnV = commandLine( map, playerPos, rowsP );
  else if ( argc > 1 ) {
    FILE *inputF = fopen( argv[1], "r" );//Opens a file for reading

    if ( inputF == NULL ) {
      fprintf( stderr, "Cannot open file.\n" );
    }
    returnV = scriptInput( map, inputF, playerPos, rowsP );
  }
  else {
    fprintf( stderr, "usage: explorer [movement_script]" );
    returnV = -1;
  }
  
  return returnV;
}