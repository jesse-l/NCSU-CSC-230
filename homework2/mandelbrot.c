/**
  @file mandelbrot.c
  @author Jesse Liddle (jaliddl2)
  
  This program draws a section of the mandelbrot series based on the user
  input of the minimum real and imaginary numbers and the size.  Then
  divides the size so that it is a 70 x 35 grid that the symbols are
  drawn onto.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mandelbrot.h"

//Limit for the loops for dwell
#define LIMIT 150
//Number of height divisions
#define HEIGHT_DIVS 35
//Number of width divisions
#define WIDTH_DIVS 70
/** Dwell cut-off for drawing with ' ' */
#define LEVEL_1 10
/** Dwell cut-off for drawing with '.' */
#define LEVEL_2 20
/** Dwell cut-off for drawing with ':' */
#define LEVEL_3 30
/** Dwell cut-off for drawing with '-' */
#define LEVEL_4 40
/** Dwell cut-off for drawing with '=' */
#define LEVEL_5 50
/** Dwell cut-off for drawing with '+' */
#define LEVEL_6 60
/** Dwell cut-off for drawing with '*' */
#define LEVEL_7 70
/** Dwell cut-off for drawing with '#' */
#define LEVEL_8 80
/** Dwell cut-off for drawing with '%' */
#define LEVEL_9 90

/**
  This is the main function of the program it gathers the input from the user
  and sends the needed information to the other functions to be used for the
  calculations.
 */
int main()
{
  double minReal; //Variable for minimum real number
  double minImag; //Variable for minimum imaginary number
  double size; //Variable for size
  int match = 0;
  
  //Ask for information then gathers the information
  printf("Minimun real: ");
  match = scanf("%lf", &minReal);
  if ( match != 1 ) {
    printf("Invalid input");
    return EXIT_FAILURE;
  }
  printf("Minimum imaginary: ");
  match = scanf("%lf", &minImag);
  if ( match != 1 ) {
    printf("Invalid input");
    return EXIT_FAILURE;
  }
  printf("Size: ");
  match = scanf("%lf", &size);
  if ( match != 1 ) {
    printf("Invalid input");
    return EXIT_FAILURE;
  }
  
  //Gives the variables to the function that draws the figure.
  drawFigure(minReal, minImag, size);
}

/**
  This is the function that determines the actual value that will be
  represented by the symbol later.
 */
int testPoint( double cReal, double cImag )
{
  int dwell = 0; //Starts dwell at 0
  long double zReal = cReal; //Copies cReal
  long double zImag = cImag; //Copies cImag
  long double mag = 0;
  long double oldZR;
  long double oldZI;
  
  while ( dwell < LIMIT ) {

    oldZR = zReal;
    oldZI = zImag;

    // Z^2 + C
    zReal = pow(oldZR, 2.0) - pow(oldZI, 2.0) + cReal;
    zImag = (oldZR * oldZI) + (oldZI * oldZR) + cImag;

    //Checks the magnitude of the new complex number breaks if >2
    mag = findMag(zReal, zImag);
    if ( mag > 2)
      break;

    dwell++; //Increase dwell count
  }
  return dwell;
}

/**
  This function compares the dwell to the table and finds which symbol that
  it needs to return to be printed in that spot.
 */
char dwellSymbol ( int dwell )
{
  if ( dwell < LEVEL_1 ) {
    return ' ';
  } else if ( LEVEL_1 <= dwell && dwell < LEVEL_2 ) {
    return '.';
  } else if ( LEVEL_2 <= dwell && dwell < LEVEL_3 ) {
    return ':';
  } else if ( LEVEL_3 <= dwell && dwell < LEVEL_4 ) {
    return '-';
  } else if ( LEVEL_4 <= dwell && dwell < LEVEL_5 ) {
    return '=';
  } else if ( LEVEL_5 <= dwell && dwell < LEVEL_6 ) {
    return '+';
  } else if ( LEVEL_6 <= dwell && dwell < LEVEL_7 ) {
    return '*';
  } else if ( LEVEL_7 <= dwell && dwell < LEVEL_8 ) {
    return '#';
  } else if ( LEVEL_8 <= dwell && dwell < LEVEL_9 ) {
    return '%';
  } else {
    return '@';
  }
}

/**
  This is used to draw the output displayed by the program.
 */
void drawFigure ( double minReal, double minImag, double size)
{
  long double divsH = size / HEIGHT_DIVS; //Finds the interval for the height divisions
  long double divsW = size / WIDTH_DIVS; //Finds the interval for the width divisions
  int dwell = 0;
  
  for ( int i = HEIGHT_DIVS; i >= 0; i-- ) {
    for ( int r = 1; r < WIDTH_DIVS ; r++ ) {
      dwell = testPoint( (minReal + (divsW * r) ), ( minImag +  (divsH * i) ) );
      printf("%c", dwellSymbol(dwell));
    } //Closes the for loop for the width

    printf("\n");
  } //Closes the for loop for the height
}

/**
  This function is used to find the magnitude of the complex number
  and return it to the testPoint function.
  
  Magnitude  -->  sqrt( a * a + b * b )
 */
double findMag ( double cReal, double cImag )
{
  return sqrt(cReal * cReal + cImag * cImag);
}
