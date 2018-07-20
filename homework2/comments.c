/**
  @file comments.c
  @author Jesse Liddle (jaliddl2)

  This program reads input from a file and counts how many
  characters there are in the file and how many old C-style comments
  there are in this file.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "comments.h"

#define EXIT_EMPTY 100
#define EXIT_UNTERM 101
#define EXIT_SUCCESS 0
#define PERCENT 100

// Total count of characters.
int totalChars = 0;

// Total count of characters that are part of a comment.
int commentChars = 0;

// Total number of comments in the input.
int commentCount = 0;

//True if the comment ended and false if it did not.
bool commentEnded = true;


/**
  This is the main function of the program and it starts when
  the program starts running.  It opens the input file and starts
  reading for comments then calls processComment when it sees a
  comment
 */
int main ()
{
  char nextChar = getchar();
  
  if ( nextChar == EOF ) {
    printf("Empty input\n");
    return EXIT_EMPTY;
  } else {
  
    while ( nextChar != EOF ) {
      totalChars++;
      
      if ( nextChar == '/' ) {
        nextChar = getchar();
        if ( nextChar == '*' ) {
          processComment();
        } else
          totalChars++;
      }
      
      nextChar = getchar();
    }
  }
  
  if ( commentEnded == false ) {
    printf("Unterminated comment\n");
    return EXIT_UNTERM;
  } else {
    totalChars += commentChars;

    //Computes the percent of characters that are inside a comment in the file.
    double percent = ((double) commentChars) / ((double) totalChars) * PERCENT;

    printf("Input characters: %d\n", totalChars);
    printf("Comments: %d (%.2f)\n", commentCount, percent);

    return EXIT_SUCCESS;
  }
}

/**
  This function will be called after the program sees the '/' '*'
  character sequence representing the start of a comment. Its job
  will be to keep reading input (and counting characters) until it
  sees the end of a comment sequence
 */
void processComment ()
{
  commentCount ++; //Increases the comment count.
  
  char nextChar = getchar(); //This is looking at the next char in the sequence
  int totalComment = 2;
  commentEnded = true;
  
  while ( nextChar != EOF ) {
    totalComment++; //Increases comment character count
    
    if ( nextChar == '*' ) {
      nextChar = getchar();
      if ( nextChar == '/' ) {
        commentChars = commentChars + totalComment;
        return;
      }
    }

    nextChar = getchar();
  } //End of while loop
  
  if ( nextChar == EOF ) {
    commentEnded = false;
    return;
  }
} //End of processComment function