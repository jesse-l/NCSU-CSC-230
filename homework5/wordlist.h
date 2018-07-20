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
#ifndef _WORDLIST_H_
#define _WORDLIST_H_

#include <stdbool.h>

/** Maximum length of a word in wordlist. */
#define WORD_MAX 20

/**
  Word type, used to store elements of the word list,
  with room for a word of up to 20 characters.
*/
typedef char Word[ WORD_MAX + 1 ];

/**
  Representation for the whole wordlist.  It contains
  the list of words as a resizable, dynamically allocated
  array, along with supporting fields for resizing.
*/
typedef struct {
  /** Number of words in the wordlist. */
  int len;

  /** Capacity of the wordlist, so we can know when we need to resize. */
  int capacity;

  /** List of words.  Should be sorted lexicographically once the word list
      has been read in. */
  Word *words;
} WordList;

/**
  This function is responsable for building the word list. It reads words form word
  file given as fname. After reading all the words from the word file, it adds
  single-character words for each of the 98 valid characters. Finally, it sorts the word
  list lexicographically so that the index of each word is its code.
*/
WordList *readWordList( char const *fname );

/**
  Given a character, this function returns true if it's one of the 98 valid
  characters that can appear in a word of the wordlist or in the text file being
  compressed. This function is may be used internally while reading the word file,
  but it's also exposed in the header file so other components can use it.
*/
bool validChar( char ch );

/**
  Given a word list, this function returns the best code for representing the
  sequence of characters at the start of the given string. (It returns the index
  of the longest word in the wordList that matches a prefix of the string.)
*/
int bestCode( WordList *wordList, char const *str );

/**
  This function adds the extra characters to the wordlist that allow anything to
  be compressed.
*/
WordList *addExtra( WordList *wList, int wCount );



/**
  This function frees the memory for the given wordList, including the dynamically
  allocated list of words inside and the wordList structure itself.
*/
void freeWordList( WordList *wordList );

/**
  Compares to values that are passed into it and returns the results.
*/
int cmpfunc (const void * a, const void * b);

#endif
