/**
    @file pack.c
    @author Jesse Liddle - jaliddl2

    This file handles the unpacking part of the program.  When
    called with at least two command line arguments this program
    will use an input file and a compressed file to compress the
    input file so that you are able to read it.

    The compressed file will consist of 9bit binary values that relate
    to a letter, word, or character.  Once this wordlist is read then
    the program goes into the wordlist and counts until it find the
    bestCode to match the word from the file and reports that and then
    it get converted into 9bit code.

    ---9bits---
    0 0000 0000

    Program correct usage
    usage: pack <input.txt> <compressed.raw> [word_file.txt]

    -The input.txt file is the file that it is being compressed.
    -The compressed.raw file is the output for the input file being
      compressed.
    -The word_file.txt is optional and it is the file that contains the
      wordlist.  The default is words.txt

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//Local includes
#include "wordlist.h"
#include "bits.h"

//Defines different constants
#define WORD_MAX 20
#define SPACE 0x20
#define MASK 0x01FF
#define MIN_ARG 3
#define MAX_ARG 4


//Function prototype
char *readFile( FILE *fp );

/**
  This is the starting point of the program and is called at the beginning
  of the program.  This function is responsible for handling all the packing
  of the input file.  It calls the functions that are responsible for doing
  all the work on the file.
  
  @param argc The number of command line arguments that were entered into the
      system when running the program.

  @param argv The command line arguments that were entered into the system.
      There should be at least 3 but can have 4 if they specify a wordlist to
      read from.

  pack <input.txt> <compressed.raw> [word_file.txt]
*/
int main( int argc, char *argv[] )
{
  //Checks to make sure argument count is correct
  if ( argc < MIN_ARG || argc > MAX_ARG ) {
    fprintf( stderr, "usage: pack <input.txt> <compressed.raw> [word_file.txt]\n" );
    return EXIT_FAILURE;
  }

//Checks command line args
#ifdef DEBUG
  if ( argc == MIN_ARG ) {
    printf("arg[0]:  %s\n", argv[0] );
    printf("arg[1]:  %s\n", argv[1] );
    printf("arg[2]:  %s\n", argv[MIN_ARG - 1] );
  } else {
    printf("arg[0]:  %s\n", argv[0] );
    printf("arg[1]:  %s\n", argv[1] );
    printf("arg[2]:  %s\n", argv[MIN_ARG - 1] );
    printf("arg[3]:  %s\n", argv[MIN_ARG] );
  }
#endif

  //Points to the list
  char *wordFile;
  char fnameL[] = "words.txt";

  if ( argc == MAX_ARG ) {
    char *fname = argv[MIN_ARG];
    wordFile = fname;
  } else {
    wordFile = fnameL;
  }
  
#ifdef DEBUG
  printf("Opening files\n");
#endif
  //Opens file pointers to both input and output
  FILE *input = fopen( argv[1], "r" );
  FILE *output = fopen( argv[2], "wb" );

#ifdef DEBUG
  printf("Files open\n");
#endif

  WordList *wordList = readWordList( wordFile );

#ifdef DEBUG
  printf("WordList made\n");
#endif

  //If wordlist is returned as null that means there was
  //an error while opening/ reading the wordlist
  if ( wordList == NULL ) {
    return EXIT_FAILURE;
  }

  //Checks for files not opened
  if ( input == NULL ) {
    fprintf( stderr, "Can't open file: %s\n", argv[1] );
    return EXIT_FAILURE;
  }
  if ( output == NULL ) {
    fprintf( stderr, "Can't open file: %s\n", argv[2] );
    return EXIT_FAILURE;
  }

  // Read the contents of the whole file into one big buffer.  This could be more
  // efficient, but it simplifies the rest of the program.
  char *buffer = readFile( input );
  
  if ( buffer == (char *)-1 ) {
    return EXIT_FAILURE;
  }
  
#ifdef DEBUG
  printf( "Buffer:  %s\n", buffer);
#endif

  // Write out codes for everything in the buffer.
  int pos = 0;
  PendingBits pending = { 0, 0 };
  int code = 0;
  while ( buffer[ pos ] ) {
#ifdef DEBUG
  printf( "Buffer[%d]:  %c\n", pos, buffer[pos]);
#endif
    // Get the next code.
    code = bestCode( wordList, buffer + pos );

#ifdef DEBUG
    printf( "%d <- %s\n", code, wordList->words[ code ] );
#endif

    // Write it out and move ahead by the number of characters we just encoded.
    writeCode( code, &pending, output );
    pos += strlen( wordList->words[ code ] );
  }

  // Write out any remaining bits in the last, partial byte.
  flushBits( &pending, output );
  
#ifdef DEBUG
  printf( "Freeing files and dynamic mem.\n" );
#endif
  freeWordList( wordList );
  fclose( input );
  fclose( output );
  
  //Exit successfully
  return EXIT_SUCCESS;
}

/**
  This function is responsible for reading the entire contents of the given
  file into a dynamically allocated sting.  It returns a pointer to a dynamically
  allocated, null terminated string containing the entire contents of the file.
*/
char *readFile( FILE *fp )
{
#ifdef DEBUG
  printf( "reading file\n" );
#endif

  char *fileStuff = malloc( sizeof( char ) );
  int size = 1;
  int counter = 0;
  int ch = fgetc( fp );
  
  while ( ch != EOF ) {
#ifdef DEBUG
  printf( "Character:  %c\n", ch);
#endif
    if ( validChar( ch ) ) {
      if ( counter < size ) {
        fileStuff[counter] = ch;
        counter++;
      } else {
        size *= 2;
        fileStuff = realloc( fileStuff, sizeof( char ) * size );
        
        fileStuff[counter] = ch;
        counter++;
      }
    } else {
      fprintf( stderr, "Invalid character code: %x\n", (unsigned char)ch );
      return NULL;
    }
    
    ch = fgetc( fp );
  }
#ifdef DEBUG
  printf( "File Stuff:  %s\n", fileStuff);
#endif
  return fileStuff;
}
