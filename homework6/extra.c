#include "extra.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <ctype.h>

//////////////////////////////////////////////////////////////////////
// Binary expressions

// For arithmetic operators, this is the maximum length of a long, printed
// out as a decimal (with a sign).
#define MAX_NUMBER 20

/** Representation for an arbitrary binary operator.  The eval
    pointer decides what it computes. */
typedef struct {
  char *(*eval)( Expr *oper, Context *ctxt );
  void (*destroy)( Expr *oper );

  // Two perand expressions.
  Expr *op1, *op2;
} BinaryExpr;

/** Free memory for any type of BinaryExpr instance. */
static void destroyBinary( Expr *expr )
{
  BinaryExpr *this = (BinaryExpr *)expr;

  // Free our operand subexpressions.
  this->op1->destroy( this->op1 );
  this->op2->destroy( this->op2 );

  // Then the BinaryExpr struct itself.
  free( this );
}

/** Construct a BinaryExpr representation and fill in the parts
    that are common to all BinaryExpr instances. */
static BinaryExpr *buildBinaryExpr( Expr *op1, Expr *op2 )
{
  BinaryExpr *this = (BinaryExpr *) malloc( sizeof( BinaryExpr ) );
  this->destroy = destroyBinary;

  this->op1 = op1;
  this->op2 = op2;

  return this;
}

//////////////////////////////////////////////////////////////////////
//Eval functions for the maker functions
/** For instances of BinaryExpr that do addition, this
    is the funciton they call for eval. */
static char *evalAdd( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  BinaryExpr *this = (BinaryExpr *)expr;

  // Evaluate our two operands
  char *left = this->op1->eval( this->op1, ctxt );
  char *right = this->op2->eval( this->op2, ctxt );

  // Parse the left and right operands as long ints.  Set them
  // to zero if they don't parse correctly.
  long a, b;
  if ( sscanf( left, "%ld", &a ) != 1 )
    a = 0;

  if ( sscanf( right, "%ld", &b ) != 1 )
    b = 0;

  // We're done with the values returned by our two subexpressions,
  // We just needed to get them as long ints.
  free( left );
  free( right );

  // Compute the result, store it in a dynamically allocated string
  // and return it to the caller.
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  sprintf( result, "%ld", a + b );
  return result;
}

static char *evalSub( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  BinaryExpr *this = (BinaryExpr *)expr;

  // Evaluate our two operands
  char *left = this->op1->eval( this->op1, ctxt );
  char *right = this->op2->eval( this->op2, ctxt );

  // Parse the left and right operands as long ints.  Set them
  // to zero if they don't parse correctly.
  long a, b;
  if ( sscanf( left, "%ld", &a ) != 1 )
    a = 0;

  if ( sscanf( right, "%ld", &b ) != 1 )
    b = 0;

  // We're done with the values returned by our two subexpressions,
  // We just needed to get them as long ints.
  free( left );
  free( right );

  // Compute the result, store it in a dynamically allocated string
  // and return it to the caller.
  char *result = (char *)malloc ( MAX_NUMBER + 1 );
  sprintf( result, "%ld", a - b );
  return result;
}

static char *evalMul( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  BinaryExpr *this = (BinaryExpr *)expr;

  // Evaluate our two operands
  char *left = this->op1->eval( this->op1, ctxt );
  char *right = this->op2->eval( this->op2, ctxt );

  // Parse the left and right operands as long ints.  Set them
  // to zero if they don't parse correctly.
  long a, b;
  if ( sscanf( left, "%ld", &a ) != 1 )
    a = 0;

  if ( sscanf( right, "%ld", &b ) != 1 )
    b = 0;

  // We're done with the values returned by our two subexpressions,
  // We just needed to get them as long ints.
  free( left );
  free( right );

  // Compute the result, store it in a dynamically allocated string
  // and return it to the caller.
  char *result = (char *)malloc (MAX_NUMBER + 1 );
  // Creates a couter for the loop for the multiplication
  int i = 0;
  // Storage for the sum of the addition
  long sum = 0;
  // Loop to replicated the multiplication
  while ( i < b ) {
    sum += a;
    i++;
  }
  sprintf( result, "%ld", sum );
  return result;
}

static char *evalDiv( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  BinaryExpr *this = (BinaryExpr *)expr;

  // Evaluate our two operands
  char *left = this->op1->eval( this->op1, ctxt );
  char *right = this->op2->eval( this->op2, ctxt );

  // Parse the left and right operands as long ints.  Set them
  // to zero if they don't parse correctly.
  long a, b;
  if ( sscanf( left, "%ld", &a ) != 1 )
    a = 0;
  if ( sscanf( right, "%ld", &b ) != 1 )
    b = 0;
  
  if ( b == 0 || a == 0 ) {
    fprintf( stderr, "Runtime Error: divide by zero\n" );
    exit( EXIT_FAILURE );
  }
  
  //Free both left and right variables
  free( left );
  free( right );
  
  long counter = 0;
  while ( a >= b ) {
    a = a - b;
    counter = counter + 1;
  } 
  
  // Compute the result, store it in a dynamically allocated string
  // and return it to the caller.
  char *result = (char *)malloc (MAX_NUMBER + 1 );
  sprintf( result, "%ld", counter );
  return result;
}

static char *evalEqual( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  BinaryExpr *this = (BinaryExpr *)expr;

  // Evaluate our two operands
  char *left = this->op1->eval( this->op1, ctxt );
  char *right = this->op2->eval( this->op2, ctxt );

  // Parse the left and right operands as long ints.  Set them
  // to zero if they don't parse correctly.
  long a, b;
  if ( sscanf( left, "%ld", &a ) != 1 )
    a = 0;
  if ( sscanf( right, "%ld", &b ) != 1 )
    b = 0;
  
  //Free both left and right variables
  free( left );
  free( right );
  
  // Compute the result, store it in a dynamically allocated string
  // and return it to the caller.
  char *result = (char *)malloc (MAX_NUMBER + 1 );
  if ( a == b ) {
    sprintf( result, "True" );
  } else {
    sprintf( result, "" );
  }
  
  return result;
}

static char *evalLess( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  BinaryExpr *this = (BinaryExpr *)expr;

  // Evaluate our two operands
  char *left = this->op1->eval( this->op1, ctxt );
  char *right = this->op2->eval( this->op2, ctxt );

  // Parse the left and right operands as long ints.  Set them
  // to zero if they don't parse correctly.
  long a, b;
  if ( sscanf( left, "%ld", &a ) != 1 )
    a = 0;
  if ( sscanf( right, "%ld", &b ) != 1 )
    b = 0;
  
  //Free both left and right variables
  free( left );
  free( right );
  
  // Compute the result, store it in a dynamically allocated string
  // and return it to the caller.
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  if ( a < b ) {
    sprintf( result, "True" );
  } else {
    sprintf( result, "" );
  }
  
  return result;
}

static char *evalNot( Expr *expr, Context *ctxt )
{
  BinaryExpr *this = (BinaryExpr *)expr;
  char *result = (char *)malloc (MAX_NUMBER + 1 );
  if ( strcmp( this->op1->eval( this->op1, ctxt ), "" ) == 0 ) {
    sprintf( result, "True" );
  } else {
    sprintf( result, "" );
  }
  
  return result;
}

static char *makVariable( Expr *expr, Context *ctxt )
{
  BinaryExpr *this = (BinaryExpr *)expr;
  char *name = this->op1->eval( this->op1, ctxt );
  char *value = this->op2->eval( this->op2, ctxt );
  
  setVariable( ctxt, name, value );
  
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  sprintf( result, "" );
  return result;
}

static char *evalIf( Expr *expr, Context *ctxt )
{
  BinaryExpr *this = (BinaryExpr *)expr;
  char *cond = this->op1->eval( this->op1, ctxt );
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  sprintf( result, "" );
  
  if ( strcmp( cond, "") == 0 ) {
    sprintf( result, "%s", this->op2->eval( this->op2, ctxt ) );
  }
   
  return result;
}

static char *evalWhile( Expr *expr, Context *ctxt )
{
  BinaryExpr *this = (BinaryExpr *)expr;
  char *cond = this->op1->eval( this->op1, ctxt );
  char *result = (char *)malloc( MAX_NUMBER + 1 );
  sprintf( result, "" );
  while ( strcmp( cond, "" ) != 0 ) {
    sprintf( result, this->op2->eval( this->op2, ctxt ) );
    cond = this->op1->eval( this->op1, ctxt );
  }
  
  return result;
}

static char *evalAnd( Expr *expr, Context *ctxt )
{
  // Get a pointer to the more specific type this function works with.
  BinaryExpr *this = (BinaryExpr *)expr;

  // Evaluate our two operands
  char *left = this->op1->eval( this->op1, ctxt );
  char *right = this->op2->eval( this->op2, ctxt );
  
  char *result = (char *)malloc ( MAX_NUMBER + 1 );
  
  if ( strcmp( left, "" ) != 0 && strcmp( right, "" ) != 0 ) {
    // We're done with the values returned by our two subexpressions,
    // We just needed to get them as long ints.
    free( left );
    free( right );
    sprintf( result, "True" );
  } else { 
    sprintf( result, "" );
  }

  //Return the results to the caller
  return result;
}

static char *evalOr( Expr *expr, Context *ctxt )
{
  BinaryExpr *this = (BinaryExpr *)expr;
  
  // Evaluate our two operands
  char *left = this->op1->eval( this->op1, ctxt );
  char *right = this->op2->eval( this->op2, ctxt );
  
  char *result = (char *)malloc ( MAX_NUMBER + 1 );
  if ( strcmp( left, "" ) != 0 ) {
    sprintf( result, "True");
  } else if ( strcmp( right, "" ) != 0 ) {
    sprintf( result, "True");
  } else {
    sprintf( result, "" );
  }
  
  return result;
}

static char *evalConcat( Expr *expr, Context *ctxt )
{
  BinaryExpr *this = (BinaryExpr *)expr;
  
  // Evaluate our two operands
  char *left = this->op1->eval( this->op1, ctxt );
  char *right = this->op2->eval( this->op2, ctxt );
  
  //Combine the two strings and return the result
  char *result = (char *)malloc ( MAX_NUMBER + 1 );
  sprintf( result, "%s%s", left, right );
  return result;
}

static char *evalSubstr( Expr *expr, Context *ctxt )
{
  BinaryExpr *this = (BinaryExpr *)expr;
  this = (BinaryExpr *)this->op2;
  char *SChar = this->op1->eval( this->op1, ctxt );
  char *EChar = this->op2->eval( this->op2, ctxt );
  
  //Scan values from the strings
  long a, b;
  if ( sscanf( SChar, "%ld", &a ) != 1 )
    a = 0;

  if ( sscanf( EChar, "%ld", &b ) != 1 )
    b = 0;
  
  //Checks if a is greater than b  
  if ( a > b ) {
    fprintf( stderr, "Cannot start higher than you end.\n" );
    exit( EXIT_FAILURE );
  }
  
  this = (BinaryExpr *)expr;
  char *str = this->op1->eval( this->op1, ctxt );
  char *result = (char *)malloc ( ( b - a ) + 1 );
  char subbuff[ b - a + 1 ];
  memcpy( subbuff, &str[ a ], ( b - a ) );
  sprintf( result, "%s", subbuff );
  
  return result;
}

//////////////////////////////////////////////////////////////////////
//Maker functions for the expressions
Expr *makeAdd( Expr *op1, Expr *op2 )
{
  // Get in a generic instance of BinaryExpr
  BinaryExpr *this = buildBinaryExpr( op1, op2 );

  // Fill in our function to do adding.
  this->eval = evalAdd;

  // Return the instance as if it's an Expr (which it sort of is)
  return (Expr *) this;
}

Expr *makeSub( Expr *op1, Expr *op2 )
{
  BinaryExpr *this = buildBinaryExpr( op1, op2 );
  
  //Fill in the function to do subtraction
  this->eval = evalSub;
  
  return (Expr *) this;
}

Expr *makeMul( Expr *op1, Expr *op2 )
{
  BinaryExpr *this = buildBinaryExpr( op1, op2 );
  
  this->eval = evalMul;
  
  return (Expr *) this;
}

Expr *makeDiv( Expr *op1, Expr *op2 )
{
  BinaryExpr *this = buildBinaryExpr( op1, op2 );
  
  this->eval = evalDiv;
  
  return (Expr *) this;
}

Expr *makeEqual( Expr *op1, Expr *op2 )
{
  BinaryExpr *this = buildBinaryExpr( op1, op2 );
  
  this->eval = evalEqual;
  
  return (Expr *) this;  
}

Expr *makeLess( Expr *op1, Expr *op2 )
{
  BinaryExpr *this = buildBinaryExpr( op1, op2 );
  
  this->eval = evalLess;
  
  return (Expr *) this;
}

Expr *makeNot( Expr *op1 )
{
  BinaryExpr *this = buildBinaryExpr( op1, NULL );
  this->eval = evalNot;
  return (Expr *) this;
}

Expr *makeVariable( char const *name )
{
  Expr *this = makeLiteral( (char *)name );
  return (Expr *)this;
}

Expr *makeSet( char const *name, Expr *expr )
{
  int letter = name[0];
  int let = isalpha( letter );
  if ( let != 0 ) {
    Expr *var = makeVariable( name );
    
    BinaryExpr *this = buildBinaryExpr( var, expr );
    this->eval = makVariable;
    return (Expr *) this;
  }

}

Expr *makeIf( Expr *cond, Expr *body )
{
  BinaryExpr *this = buildBinaryExpr( cond, body );
  
  this->eval = evalIf;
  
  return (Expr *) this;
}

Expr *makeWhile( Expr *cond, Expr *body )
{
  BinaryExpr *this = buildBinaryExpr( cond, body );
  
  this->eval = evalWhile;
  
  return (Expr *) this;
}

//////////////////////////////////////////////////////////////////////
//Extra Credit Functionality
Expr *makeAnd( Expr *op1, Expr *op2 )
{
  // Get a pointer to the more specific type this function works with.
  BinaryExpr *this = buildBinaryExpr( op1, op2 );
  
  this->eval = evalAnd;
  
  return (Expr *) this;
}

Expr *makeOr( Expr *op1, Expr *op2 )
{
  // Get a pointer to the more specific type this function works with.
  BinaryExpr *this = buildBinaryExpr( op1, op2 );
  
  this->eval = evalOr;
  
  return (Expr *) this;
}

Expr *makeConcat( Expr *op1, Expr *op2 )
{
  // Get a pointer to the more specific type this function works with.
  BinaryExpr *this = buildBinaryExpr( op1, op2 );
  
  this->eval = evalConcat;
  
  return (Expr *) this;
}

Expr *makeSubstr( Expr *op1, Expr *start, Expr *end )
{
  BinaryExpr *this = buildBinaryExpr( start, end );
  
  BinaryExpr *sup = buildBinaryExpr( op1, (Expr *)this );
  
  sup->eval = evalSubstr;
  
  return (Expr *) sup;
}
