/**
    @file unpack.c
    @author Jesse Liddle - jaliddl2

    This file handles the unpacking part of the program.  When
    called with at least two command line arguments this program
    will use an output file and a compressed file to uncompress the
    input file so that you are able to read it.

    The compressed file will consist of 9bit binary values that relate
    to a letter, word, or character.  Once this 9bit code is read then
    the program goes into the wordlist and counts down  that many entries
    to find the letter, word, or character that is represented.

    ---9bits---
    0 0000 0000

    Program correct usage
    usage: unpack <compressed.raw> <output.txt> [word_file.txt]

    -The compressed.raw file is the compressed file that the program
      is trying to uncompress.
    -The output.txt file is the file that it is printing out to once the
      code has been read and convereted to a word.
    -The word_file.txt is optional and it is the file that contains the
      wordlist.  The default is words.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "wordlist.h"
#include "bits.h"

int main( int argc, char *argv[] )
{
  //Points to the list
  char *wordFile;
  char *fnameL = "words.txt" ;

  //Checks for correct number of inputs
  if ( argc < 3 || argc > 4 ) {
    fprintf( stderr, "usage: unpack <compressed.raw> <output.txt> [word_file.txt]\n" );
    return EXIT_FAILURE;
  } else if ( argc == 4 ) {
    wordFile = argv[3];
  } else {
    wordFile = fnameL;
  }

//Checks command line args
#ifdef DEBUG
  if ( argc == 3 ) {
    printf("arg[0]:  %s\n", argv[0] );
    printf("arg[1]:  %s\n", argv[1] );
    printf("arg[2]:  %s\n", argv[2] );
  } else {
    printf("arg[0]:  %s\n", argv[0] );
    printf("arg[1]:  %s\n", argv[1] );
    printf("arg[2]:  %s\n", argv[2] );
    printf("arg[3]:  %s\n", argv[3] );
  }
#endif

  WordList *wordList = readWordList( wordFile );

  //Opens file pointers to both input and output
  FILE *inputF = fopen( argv[1], "rb" );
  FILE *outputF = fopen( argv[2], "w" );

  //Checks for files not opened
  if ( inputF == NULL ) {
    fprintf( stderr, "Can't open file: %s\n", argv[1] );
    return EXIT_FAILURE;
  }
  if ( outputF == NULL ) {
    fprintf( stderr, "Can't open file: %s\n", argv[2] );
    return EXIT_FAILURE;
  }

  PendingBits *pending = malloc( sizeof( PendingBits ) );
  pending->bits = 0;
  pending->bitCount = 0;
  
  int code = readCode( pending, inputF );
#ifdef DEBUG
  printf( "Code is:   %d\n", code );
#endif
  Word *str;
  while ( code != EOF ) {
#ifdef DEBUG
  printf( "Code is:   %d\n", code );
#endif
    str = &wordList->words[code];
    fprintf( outputF, "%s", (char *)str );

    code = readCode( pending, inputF );
  }

  //Frees dynamic mem and open files.
  freeWordList( wordList );
  free( pending );
  fclose( inputF );
  fclose( outputF );

  //Exit successfully
  return EXIT_SUCCESS;
}
