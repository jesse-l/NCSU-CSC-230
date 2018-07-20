/**
  @file drawing.c
  @author Jesse Liddle (jaliddl2)

  This is the main part to the drawing program.
  This is the part the controls the whole program
  and is called first.
*/

#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
  This is the main function of the program and is
  called to run as soon as the program is executed.
*/
int main( int argc, char **argv )
{
  char type;//Type of object being drawn
  int match = 0;//Matches for the fscanf
  int x1;//x1 for line or cx for circle
  int y1;//y1 for line or cy for circle
  int x2;//x2 for line not used for circle
  int y2;//y2 for line not used for circle
  int rad;//radius for circle not used for line
  int color;//color for both circle and line
  int maxColor = 255;
  unsigned char picture[ WIDTH ][ HEIGHT ];//The picture this is being drawn to
  unsigned char clearColor = 255;
  int matchLine = 5;
  int matchCircle = 4;
  
  //Sets all pixels to white before drawing on them
  clearImage( picture, clearColor );
  
  //Opens up input and output files
  FILE *inputF = fopen( argv[1], "r" );//Opens a file for reading
  FILE *outputF = fopen( argv[2], "w" );//Opens a file for writing
  
  if ( argc != 3 ) {
    fprintf(stderr, "usage: drawing <script_file> <image_file>\n");
    return -1;
  } else if ( inputF == NULL ) {
    fprintf(stderr, "Can't open file: %s\n", argv[1]);
    fprintf(stderr, "usage: drawing <script_file> <image_file>\n");
    return -1;
  } else {
  //Loop for reading the file
  while ( fscanf( inputF, " %c", &type ) == 1 ){
    if ( type == 'l' || type == 'L' ){
      match = fscanf( inputF, " %d %d %d %d %d", &x1, &y1, &x2, &y2, &color );
      
      //Checks to see matchs the output of the fscanf
      if ( match == matchLine  && color >= 0 && color <= maxColor )
        drawLine( picture, x1, y1, x2, y2, color );
      else
        goto ERROR;
    } else if ( type == 'c' || type == 'C' ) {
      match = fscanf( inputF, " %d %d %d %d", &x1, &y1, &rad, &color );
      
      //Checks to see matchs the output of the fscanf
      if ( match == matchCircle  && color >= 0 && color <= maxColor )
        drawCircle( picture, x1, y1, rad, color );
      else
        goto ERROR;
    } else {
ERROR:
      //Error if line does not start with l or c
      fprintf(stderr, "Invalid script file\n");
      return -1;
      break;
    }//End of if-else statement
    
    //match = fscanf( inputF, "\n%c", &type );
    
  }//End of while loop

    saveImage( picture, outputF );//Saves file
    fclose(inputF);//Closes the inputF file
    fclose(outputF);//Closes the outputF file
    return EXIT_SUCCESS;
  }
}
