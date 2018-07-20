#include "basic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Literal

// Representation for a Literal expression, derived from Expr.
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  /** Literal value of this expression. */
  char *val;
} LiteralExpr;

// Function to evaluate a literal expression.
static char *evalLiteral( Expr *expr, Context *ctxt )
{
  // Cast the this pointer to a more specific type.
  LiteralExpr *this = (LiteralExpr *)expr;

  // Make and return a copy of the value we contain.
  char *result = (char *) malloc( strlen( this->val ) + 1 );
  strcpy( result, this->val );
  return result;
}

// Function to free a literal expression.
static void destroyLiteral( Expr *expr )
{
  // Cast the this pointer to a more specific type.
  LiteralExpr *this = (LiteralExpr *)expr;

  // Free the value we contain and the literal object itself.
  free( this->val );
  free( this );
}

Expr *makeLiteral( char *val )
{
  // Allocate space for the LiteralExpr object
  LiteralExpr *this = (LiteralExpr *) malloc( sizeof( LiteralExpr ) );

  // Remember our virutal functions.
  this->eval = evalLiteral;
  this->destroy = destroyLiteral;

  // Remember the literal string we contain.
  this->val = val;

  // Return the result, as an instance of the base.
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// print

// Representation for a print expression, derived from Expr.
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  /** Argumetn expression we're supposed to evaluate and print. */
  Expr *arg;
} PrintExpr;

// Function to evaluate a print expression.
static char *evalPrint( Expr *expr, Context *ctxt )
{
  // Cast the this pointer to a more specific type.
  PrintExpr *this = (PrintExpr *)expr;

  // Evaluate our argument and print the result.
  char *result = this->arg->eval( this->arg, ctxt );
  printf( "%s", result );
  
  // The print expression evaluates to the thing it printed (clever, then
  // we don't have to do an unnecessary malloc/free.
  return result;
}

// Function to free a print expression.
static void destroyPrint( Expr *expr )
{
  // Cast the this pointer to a more specific type.
  PrintExpr *this = (PrintExpr *)expr;

  // Free our subexpression then the print expression itself.
  this->arg->destroy( this->arg );
  free( this );
}

Expr *makePrint( Expr *arg )
{
  // Allocate space for the PrintExpr object
  PrintExpr *this = (PrintExpr *) malloc( sizeof( PrintExpr ) );

  // Remember our virutal functions.
  this->eval = evalPrint;
  this->destroy = destroyPrint;

  // Remember our argument subexpression.
  this->arg = arg;

  // Return the result, as an instance of the base.
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
// Compound

// Representation for a compound expression, derived from Expr.
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  /** List of subexpressions in the compound. */
  Expr **eList;

  /** Number of subexpressions in the compound. */
  int len;
} CompoundExpr;

// Function to evaluate a compound expression.
static char *evalCompound( Expr *expr, Context *ctxt )
{
  // Cast the this pointer to a more specific type.
  CompoundExpr *this = (CompoundExpr *)expr;

  // Evaluate the sequence of expressions in this compound
  for ( int i = 0; i < this->len; i++ ) {
    char *result = this->eList[ i ]->eval( this->eList[ i ], ctxt );
    
    // Return the value of the last subexpression.
    if ( i + 1 >= this->len )
      return result;
    
    // Or free it, if we don't need it.
    free( result );
  }
  
  // Never reached.
  return NULL;
}

// Function to free a compound expression.
static void destroyCompound( Expr *expr )
{
  // Cast the this pointer to a more specific type.
  CompoundExpr *this = (CompoundExpr *)expr;

  // Free the list of subexpressions and the compond object itslef.
  for ( int i = 0; i < this->len; i++ )
    this->eList[ i ]->destroy( this->eList[ i ] );
  free( this->eList );
  free( this );
}

Expr *makeCompound( Expr **eList, int len )
{
  // Allocate space for the CompoundExpr object
  CompoundExpr *this = (CompoundExpr *) malloc( sizeof( CompoundExpr ) );

  // Remember our virutal functions.
  this->eval = evalCompound;
  this->destroy = destroyCompound;

  if ( len <= 0 ) {
    fprintf( stderr, "line %d: empty compound expression\n", linesRead() );
    exit( EXIT_FAILURE );
  }

  // Remember our list of subexpressions.
  this->eList = eList;
  this->len = len;

  // Return the result, as an instance of the base.
  return (Expr *) this;
}
