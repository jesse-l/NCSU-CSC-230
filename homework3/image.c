/**
  @file image.c
  @author Jesse Liddle (jaliddl2)

  This part of the drawing program provides some of
  the main functionality for the drawing.c file.
*/
#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define COORDS 2

/**
  This function is used to clear the image.  It sets all the pixels to one
  specific color that is passed to this function.
  
  @param image The image that is being edited.
  @param color The color that is being put on all pixels.
*/
void clearImage( unsigned char image [ WIDTH ] [ HEIGHT ], unsigned char color )
{
  for ( int k = 0; k < HEIGHT; k++ ) {
    for ( int i = 0; i < WIDTH; i++ ) {
      image[i][k] = color;
    }
  }
}

/**
  This function is used to save the image to the output file
  that was entered by the user.
  
  @param image The image that is being saved.
  @param outputFile The file where data is being wrote to.
*/
void saveImage( unsigned char image [ WIDTH ] [ HEIGHT ], FILE *outputFile )
{
  //Header stuff for pgm file type
  fprintf( outputFile, "P2\n" );
  fprintf( outputFile, "%d %d\n255\n", WIDTH - 1, HEIGHT - 1 );
  
  //Loops to traverse the array of the picture
  for ( int k = 0; k <= HEIGHT - 2; k++ ) {
    for ( int i = 0; i <= WIDTH - 2; i++ ) {
    
      //Determines correct spacing for the file
      if ( (int)image[i][k] < 10 ) {
        fprintf( outputFile, "  %d", (int)image[i][k] );
      } else if ( (int)image[i][k] >= 10 && (int)image[i][k] < 100 ) {
        fprintf( outputFile, " %d", (int)image[i][k] );
      } else {
        fprintf( outputFile, "%d", (int)image[i][k] );
      }
      
      //Stops spacing at end of line
      if ( i < WIDTH - 1 )
        fprintf( outputFile, " " );
    }
  
  fprintf( outputFile, "\n" );
  }
  
  //Closes the outputFile
  //fclose(outputFile);
}

/**
  This function is used to draw a line on the picture based on
  what is read from the file.
  
  @param image The image that is being drawn on.
  @param x1 This is the first x value of the line.
  @param y1 This is the first y value of the line.
  @param x2 This is the second x value of the line.
  @param y2 This is the second y value of the line.
  @param color This is the color that the line will be.
*/
void drawLine( unsigned char image [ WIDTH ] [ HEIGHT ], int x1,
    int y1, int x2, int y2, unsigned char color )
{
  int dx = x2 - x1;//Change in x
  int dy = y2 - y1;//Change in y
  
  //Determine the largest change
  int larger = 0;
  if ( abs( dx ) > abs( dy ) ) {
    larger = abs( dx ) + 1;
  } else {
    larger = abs( dy ) + 1;
  }
  
  //Determines the change in each test point
  double xdiff = (x2 - x1) / (double)(larger - 1);
  double ydiff = (y2 - y1) / (double)(larger - 1);
  
  //Point x & y to plot colors too
  double x;
  double y;
  
  for ( int i = 0; i < larger; i++ ){
    //finds the points x and y
    x = (double)x1 + ( xdiff * i );
    y = (double)y1 + ( ydiff * i );
    
    //Changes the color at the points on the line
    if ( (int)round(x) >= 0 && (int)round(x) < WIDTH 
          && (int)round(y) >= 0 && (int)round(y) < HEIGHT ) {
      image[ (int)round(x) ][ (int)round(y) ] = color;
    }//Closes if statement
  }//Closes for loop

}//Closes drawLine function

/**
  This function is used to draw a circle on the picture based on
  what is read from the file.
  
  @param image The image that is being drawn on.
  @param cx The center x value for the circle.
  @param cy The center y value for the circle.
  @param radius The radius the circle is going to be.
  @param color The color that the circle will be.
*/
void drawCircle( unsigned char image [ WIDTH ] [ HEIGHT ], int cx,
    int cy, int radius, unsigned char color )
{
  if ( radius == 0 )
    return;
  int xsqrd;//Current x squared
  int ysqrd;//Current y squared
  int rsqrd = radius * radius;//Radius squared
  
  //Two for loops to traverse through the points of the circle
  for ( int i = cx - radius; i < cx + radius; i++ ) {
    for (int k = cy - radius; k < cy + radius; k++ ) {
      //Finds (x-h)^2 and (y-k)^2
      xsqrd = (i - cx) * (i - cx);
      ysqrd = (k - cy) * (k - cy);

      //Checks to see if it can draw the point in the circle
      if ( xsqrd + ysqrd < rsqrd ){
        //Bounds checking for the image
        if ( i >= 0 && i < WIDTH && k >= 0 && k < HEIGHT )
          image[i][k] = color;
      }
    }
  }
}
