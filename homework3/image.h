/**
  @file image.h
  @author Jesse Liddle (jaliddl2)

  This is the header file for the image.c file.
  This just outlines the functions in the .c file
*/
#include <stdio.h>

//Defines a WIDTH and HEIGHT for the picture array
#define WIDTH 256
#define HEIGHT 256

//Declares the global variable for the array
extern unsigned char picture[ WIDTH ][ HEIGHT ];

/**
  This function is used to clear the image.  It sets all the pixels to one
  specific color that is passed to this function.
  
  @param image The image that is being edited.
  @param color The color that is being put on all pixels.
*/
void clearImage( unsigned char image [ WIDTH ] [ HEIGHT ], unsigned char color );

/**
  This function is used to save the image to the output file
  that was entered by the user.
  
  @param image The image that is being saved.
  @param outputFile The file where data is being wrote to.
*/
void saveImage( unsigned char image [ WIDTH ] [ HEIGHT ], FILE *outputFile );

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
    int y1, int x2, int y2, unsigned char color );

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
    int cy, int radius, unsigned char color );
