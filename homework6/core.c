#include "core.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////
// Context
Context *makeContext()
{
  Context *newC = (Context *)malloc( sizeof( struct ContextTag ));
  return  newC;
}

char const *getVariable( Context *ctxt, char const *name )
{
  char *result = (char*)malloc( sizeof( char * ) + 1 );
  Context *search = ctxt;
  while ( search->name != name ) {
    search = search->next;
    if( search->name == NULL ) {
      sprintf( result, "" );
      return result;
    }
  }
  sprintf( result, "%s", search->value );
  return result;
}

void setVariable( Context *ctxt, char const *name, char *value )
{
  if ( ctxt->name == NULL ) {
    sprintf( ctxt->name, "%s", name );
    sprintf( ctxt->value, "%s", value );
    ctxt->next = (Context *)malloc( sizeof( Context ) );
  } else {
    Context *search = ctxt;
    while ( search->name != NULL ) {
      search = search->next;
    }
  }
}

void freeContext( Context *ctxt )
{
  if ( ctxt->name != NULL ) {
    Context *search = ctxt;
    while ( ctxt->name != NULL ) {
      ctxt = ctxt->next;
      free( search->next );
      free( search );
      search = ctxt;
    }
    free( ctxt );
  }
}

//////////////////////////////////////////////////////////////////////
// Input tokenization

// Current line we're parsing, starting from 1 like most editors.
static int lineCount = 1;

bool nextToken( char *token, FILE *fp )
{
  int ch;

  // Skip whitespace and comments.
  while ( isspace( ch = fgetc( fp ) ) || ch == '#' ) {
    // If we hit the comment characer, skip the whole line.
    if ( ch == '#' )
      while ( ( ch = fgetc( fp ) ) != EOF && ch != '\n' )
        ;

    if ( ch == '\n' )
      lineCount++;
  }
    
  if ( ch == EOF )
    return false;

  // Record the character we'ver read and keep up with the token length.
  int len = 0;
  token[ len++ ] = ch;

  // Handle punctuation.
  if ( ch == '{' || ch == '}' ) {
    token[ len++ ] = '\0';
    return true;
  }

  // Handle non-quoted words.
  if ( ch != '"' ) {
    while ( ( ch = fgetc( fp ) ) != EOF && !isspace( ch ) &&
            ch != '{' && ch != '}' && ch != '"' && ch != '#' ) {
      // Complain if the token is too long.
      if ( len >= MAX_TOKEN ) {
        fprintf( stderr, "line %d: token too long\n", linesRead() );
        exit( EXIT_FAILURE );
      }

      token[ len++ ] = ch;
    }

    // We had to read one character too far to find the end of the token.
    // put it back.
    if ( ch != EOF )
      ungetc( ch, fp );

    token[ len++ ] = '\0';
    return true;
  }

  // Most interesting case, handle strings.

  // Is the next character escaped.
  bool escape = false;

  // Keep reading until we hit the matching close quote.
  while ( ( ch = fgetc( fp ) ) != '"' || escape ) {
    // Error conditions
    if ( ch == EOF || ch == '\n' ) {
      fprintf( stderr, "line %d: %s while reading parsing string literal.\n",
               linesRead(), ch == EOF ? "EOF" : "newline" );
      exit( EXIT_FAILURE );
    }
      
    // On a backslash, we just enable escape mode.
    if ( !escape && ch == '\\' ) {
      escape = true;
    } else {
      // Interpret escape sequences if we're in escape mode.
      if ( escape ) {
        switch ( ch ) {
        case 'n':
          ch = '\n';
          break;
        case 't':
          ch = '\t';
          break;
        case '"':
          ch = '"';
          break;
        case '\\':
          ch = '\\';
          break;
        default:
          fprintf( stderr, "line %d: Invalid escape sequence \"\\%c\"\n",
                   linesRead(), ch );
          exit( EXIT_FAILURE );
        }
        escape = false;
      }

      // Complain if this string, with the eventual close quote, is too long.
      if ( len + 1 >= MAX_TOKEN ) {
        fprintf( stderr, "line %d: token too long\n", linesRead() );
        exit( EXIT_FAILURE );
      }
      token[ len++ ] = ch;
    }
  }
    
  // Store the closing quote and the terminator and return.
  token[ len++ ] = '"';
  token[ len++ ] = '\0';
  return token;
}

int linesRead()
{
  return lineCount;
}

