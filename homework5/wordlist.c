/**
    @file wordlist.c
    @author Jesse Liddle - jaliddl2

    This file is responsable for creating and sorting the wordlist for both the
    pack and unpack functions.  This list is used to generate the code that is
    writen to the compressed file and finding the word using the 9bit code.

    When run from the pack function this file reads the wordlist and creates a
    dynamically allocated list of all the words in the wordslist file.  Once it
    is created then it sorts the list alphabetically.  Once the list is sorted
    it is now ready to use to generate the 9bit code that will be used to
    compress the file.

    When run from the unpack function this file reads the wordlist and creates a
    dynamically allocated list of all the words in the wordslist file.  Once it
    is created then it sorts the list alphabetically.  Once the list is sorted
    it is now ready to use to find the word that was writed by reading the 9bit
    code and converting that to a number and counting that far into the wordlist
    which will give you a letter, word, or character.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "wordlist.h"

#define TAB 0x09
#define NEWLINE 0x0A
#define CAR_RET 0x0D
#define SPACE 0x20
#define TILDE 0x7E

/**
  Given a character, this function returns true if it's one of the 98 valid
  characters that can appear in a word of the wordlist or in the text file being
  compressed. This function is may be used internally while reading the word file,
  but it's also exposed in the header file so other components can use it.
  
  ---Valid Characters---
  TAB               0x09
  NEWLINE           0x0A
  CARRAGE RETURN    0x0D
  SPACE             0x20
  TILDE             0x7E
*/
bool validChar( char ch )
{
  if ( (unsigned char)ch == TAB ) {
    return true;
  } else if ( (unsigned char)ch == NEWLINE ) {
    return true;
  } else if ( (unsigned char)ch == CAR_RET ) {
    return true;
  } else if ( (unsigned char)ch >= SPACE && (unsigned char)ch <= TILDE ) {
    return true;
  } else {
    return false;
  }
}

/**
  This function is responsable for building the word list. It reads words form word
  file given as fname. After reading all the words from the word file, it adds
  single-character words for each of the 98 valid characters. Finally, it sorts the word
  list lexicographically so that the index of each word is its code.
*/
WordList *readWordList( char const *fname )
{
#ifdef DEBUG
  printf("Creating wordlist\n");
#endif

  //Creates a generic wordlist
  WordList *wList = malloc( sizeof( WordList ) );
  wList->len = 0;
  wList->capacity = 2;
  wList->words = malloc( sizeof( Word) * wList->capacity ); 

#ifdef DEBUG
  printf("Blank wordlist\n");
#endif

  //Opens the wordlist file
  FILE *fp = fopen( fname, "r" );

#ifdef DEBUG
  printf("Checking to see if file opened\n");
#endif

  //Checks to see if file is null
  if ( fp == NULL ) {
#ifdef DEBUG
  printf("Error opening file\n");
#endif
    fprintf( stderr, "Can't open word file: %s\n", fname );
    return NULL;
  }

#ifdef DEBUG
  printf("Wordlist file opened\n");
#endif

  //number of the item in the wordlist
  int number = 0;
  //Storage for reading a word
  char word1[21];
  char cmp;
  int wCount = 0;
  int counter = 0;
  int readCount = fscanf( fp, "%d %20s", &number, word1 );

#ifdef DEBUG
  printf("Starting loop to read file\n");
#endif

  //Loop for reading file
  while ( readCount != 0 && readCount != EOF ) {
    //Sets cmp to first character of word
    cmp = word1[0];
    counter = 0;
    //Compares to make sure word characters are valid
    while ( cmp != 0x00 ) {
      if ( !validChar(cmp) ) {
        return NULL;
      }

      counter++;
      cmp = word1[counter];
    }

    //Expands the wordlist when it gets full
    if ( (wList->len + 1) > wList->capacity ) {
      wList->capacity *= 2;
      wList->words = realloc( wList->words, ( sizeof( Word ) * wList->capacity ) );
    }
    
    //Checks to make sure word is less than 20 characters
    if ( number <= WORD_MAX ) {
      strncpy( wList->words[wCount], word1, sizeof(Word) );
      wCount++;
      wList->len++;
    }

    //Reads next line in the list and stores the count of read items
    readCount = fscanf( fp, "%d %20s", &number, word1 );
#ifdef DEBUG
  printf("Read another line\n");
#endif
  }
  //Sending the word list for the extra characters to be added
  wList = addExtra( wList, wCount );
#ifdef DEBUG
  // Report the entire contents of the word list, once it's built.
  if ( wList->len != 0 ) {
    printf( "---- word list -----\n" );
    for ( int i = 0; i < wList->len; i++ )
      printf( "%d == %s\n", i, wList->words[ i ] );
    printf( "--------------------\n" );
  }
#endif
  qsort( wList->words, wList->len, sizeof(Word), cmpfunc );

#ifdef DEBUG
  //Displays the sorted word list
  if ( wList->len >= 0 ) {
    printf( "---- sorted word list -----\n" );
    for ( int i = 0; i < wList->len; i++ )
      printf( "%d == %s\n", i, wList->words[ i ] );
    printf( "--------------------\n" );
  }
  printf("Adding null terminator to end of list\n");
#endif
  strncpy( wList->words[wList->len], "\0", sizeof(Word) );
  wList->len++;

  //Checks to make sure there isn't an invalid argument input.
  if ( readCount > 3 ) {
    fprintf( stderr, "Invalid word file.\n" );
    return NULL;
  } else {
    //Return wordlist
    return wList;
  }

}

/**
  This function adds the extra characters to the wordlist that allow anything to
  be compressed.
  
  TAB      0x09
  NEWLINE  0x0A
  CAR_RET  0x0D
  SPACE    0x20
  TILDE    0x7E
*/
WordList *addExtra( WordList *wList, int wCount )
{
  int len = wList->len;
  int cap = wList->capacity;
  char tab[] = { TAB };
  char newline[] = { NEWLINE };
  char carRet[] = { CAR_RET };

  //Adds tab to the list
  if ( cap > len ) {
    strncpy( wList->words[wList->len], tab, sizeof(Word) );
    wList->len++;
    wCount++;
  } else {

    wList->capacity *= 2;
    wList->words = realloc( wList->words, ( sizeof( Word ) * wList->capacity + 1 ) );
    strncpy( wList->words[wCount], tab, sizeof( Word ) );
    wCount++;
    wList->len++;
  }


  //Adds newline character to list
  if ( cap > len ) {
    strncpy( wList->words[wCount], newline, sizeof(Word) );
    wList->len++;
    wCount++;
  } else {
    wList->capacity *= 2;
    wList->words = realloc( wList->words, ( sizeof( Word ) * wList->capacity ) );
    strncpy( wList->words[wCount], newline, sizeof(Word) );
    wList->len++;
    wCount++;
  }

  //Adds CR character to list
  if ( cap > len ) {
    strncpy( wList->words[wCount], carRet, sizeof(Word) );
    wList->len++;
    wCount++;
  } else {
    wList->capacity *= 2;
    wList->words = realloc( wList->words, ( sizeof( Word ) * wList->capacity ) );
    strncpy( wList->words[wCount], carRet, sizeof(Word) );
    wList->len++;
    wCount++;
  }


  char i;
  for ( i = ' '; i <= '~'; ++i ) {

    if ( cap > len ) {
      strncpy( wList->words[wCount], &i, sizeof(Word) );
      wList->len++;
      wCount++;
    } else {
      wList->capacity *= 2;
      wList->words = realloc( wList->words, ( sizeof( Word ) * wList->capacity ) );
      strncpy( wList->words[wCount], &i, sizeof(Word) );
      wList->len++;
      wCount++;
    }
  } 
  return wList;
}

/**
  Given a word list, this function returns the best code for representing the
  sequence of characters at the start of the given string. (It returns the index
  of the longest word in the wordList that matches a prefix of the string.)
*/
int bestCode( WordList *wordList, char const *str )
{
#ifdef DEBUG
  printf("Trying to find the best code.\n");
#endif
  int match = 0;
  match = false;
  int length = strlen( str );
  Word *matchS; 

  for ( int i = length-1; i >= 0; i-- ) {
    matchS = bsearch( str, wordList->words, wordList->len, sizeof(Word), cmpfunc );
    
    if ( matchS != NULL ) {
      match = &matchS - &wordList->words;
      break;
    }
  } 

#ifdef DEBUG
  printf( "String is:    %s\n", str );
  printf( "Code is:      %d\n", match );
#endif

  return match;
}


/**
  This function frees the memory for the given wordList, including the dynamically
  allocated list of words inside and the wordList structure itself.
*/
void freeWordList( WordList *wordList )
{
  int len = wordList->capacity;
#ifdef DEBUG
  printf("Capacity:   %d\n", len );
#endif
  //Frees the words in the list
  for( int i = 0; i < len; i++ ) {
#ifdef DEBUG
  printf("Free:   %d\n", i );
#endif
    free( wordList->words[i] );
  }
  //free( wordList );
}

/**
  Compares to values that are passed into it and returns the results.
*/
int cmpfunc (const void * a, const void * b)
{
   return strcmp( a, b );
}
