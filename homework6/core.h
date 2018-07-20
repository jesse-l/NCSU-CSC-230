/**
  @file core.h
  @author David Sturgill (dbsturgi)

  Central functionality for our interpreter.  Support for the context
  object, tokenization and definition of the Expr base class.
*/

#ifndef _CORE_H_
#define _CORE_H_

#include <stdio.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////
// Context

// Maximum length of a variable name.
#define MAX_VAR_NAME 20

/**
   Short typename for the Context structure.  It's definition is an
   implementation detail of the language.c, not visible to client code.
*/
typedef struct ContextTag Context;

/** Create and return a new, empty context object.
    @return new context object.  The caller must eventually free this
    with freeContext().
*/
Context *makeContext();

/** Return a copy of the value of the variable with the given name.  If the
    variable isn't defined, this function returns a dynamically allocated copy
    of the empty string.
    @param ctxt context object in which to lookup the variable name.
    @param new value for the variable name.
    @return the variable's value.  This is a pointer into the context's
    representation and should be directly freed or modified by the caller.
*/
char const *getVariable( Context *ctxt, char const *name );

/** Set the value of the given variable in this context to the given name.
    This function will copy the given value (and variable name if necessary) into
    dynamically allocated memory stored in the context, so it can
    store them as long as necessary.
    @param ctxt context in which to store the variable name / value.
    @param name of the varialbe to set the value for.
    @param value new value for this variable.
*/
void setVariable( Context *ctxt, char const *name, char *value );

/** Free all the memory associated with this context.
    @param ctxt context to free memory for.
*/
void freeContext( Context *ctxt );

/** This is the struct for the context storage of the variables
    that are defined in the program.  This struct takes a name,
    a value and a pointer to the next context.  Also has a variable
    that acts as a counter for the number of variables stored at the
    current time.
*/
struct ContextTag {
  // Name of the variable being stored
  char *name;
  // Value of the variable being stored
  char *value;
  // Pointer to the next Context if one exist
  Context *next;
};

//////////////////////////////////////////////////////////////////////
// Input totkenization

// Maximum length of a token in the source file.
#define MAX_TOKEN 1023

/** Read and the next token from the given file, a space-delimtied
    word, a double quoted string or either of the curly brackets.
    @param tok storage for the token, with room for a string of up to
     MAX_TOKEN characters.
    @param fp file to read tokens from.
    @return true if the token is successfully read.
    @sideeffect increments a global line count as it
    parses newlines.
*/
bool nextToken( char *tok, FILE *fp );

/** Return the number of lines read so far.  This is maintained by nextToken.
    @return the number of lines read so far.
*/
int linesRead();

//////////////////////////////////////////////////////////////////////
// Expr

/** A short name to use for the expression interface. */
typedef struct ExprTag Expr;

/** Representation for an Expr interface.  Classes implementing this
    have these two fields as their first members.  They will set eval
    to point to appropriate functions to evaluate the type of
    expression their class represents, and they will set destroy to
    point to a function that frees memory for their type of expresson.
*/
struct ExprTag {
  /** Pointer to a function to evaluate the given expression and
      return the result as a dynamically allocated string.
      @param expr expression to be evaluated.
      @param ctxt current values of all variables.
      @return string respresentation of the result. The caller is responsible
      for freeing this.
   */
  char *(*eval)( Expr *expr, Context *ctxt );

  /** Free memory for this expression, including any subexpressions
      it contains.
      @param expr expression to free.
  */
  void (*destroy)( Expr *expr );
};

#endif
