#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "basic.h"
#include "extra.h"

/** Print a usage message then exit unsuccessfully. */
void usage()
{
  fprintf( stderr, "usage: interpreter <program-file>\n" );
  exit( EXIT_FAILURE );
}

// Initial capacity for the resizable array used to store
// subexpressions in a compound expression.
#define INITIAL_CAPACITY 5

/** Called when we expect another token on the input.  This 
    function parses the token and exits with an error if there isn't one.
    @param storage for the next token, with capacity for at least MAX_TOKEN characters.
    This buffer may be modified by the parse function as it reads additional tokens.
    @param fp file tokens should be read from.
    @return a copy of the pointer to the tok buffer, so this function can be used as a
    parameter to other parsing calls.
*/
char *expectToken( char *tok, FILE *fp )
{
  if ( !nextToken( tok, fp ) ) {
    fprintf( stderr, "line %d: token expected\n", linesRead() );
    exit( EXIT_FAILURE );
  }
  
  return tok;
}

/** Parse with one token worth of look-ahead, return the expression object representing
    the syntax parsed.
    @param tok next token from the input.
    @param fp file subsequent tokens are being read from.
    @return the expression object constructed from the input.
*/
Expr *parse( char *tok, FILE *fp )
{
  // Create a literal token for anything that looks like a number.
  {
    long dummy;
    int pos;
    // See if the whole token parses as a long int.
    if ( sscanf( tok, "%ld%n", &dummy, &pos ) == 1 &&
         pos == strlen( tok ) ) {
      // Copy the literal to a dynamically allocated string, since makeLiteral wants
      // a string it can keep.
      char *str = (char *) malloc( strlen( tok ) + 1 );
      return makeLiteral( strcpy( str, tok ) );
    }
  }

  // Create a literal token for a quoted string, without the quotes.
  if ( tok[ 0 ] == '"' ) {
    // Same as above, make a dynamically allocated copy of the token that the literal
    // expression can keep as long as at wants to.
    int len = strlen( tok );
    char *str = (char *) malloc( len - 1 );
    strncpy( str, tok + 1, len - 2 );
    str[ len - 2 ] = '\0';
    return makeLiteral( str );
  }

  // Handle compound statements
  if ( strcmp( tok, "{" ) == 0 ) {
    int len = 0;
    int cap = INITIAL_CAPACITY;
    Expr **eList = (Expr **) malloc( cap * sizeof( Expr * ) );

    // Keep parsing subexpressions until we hit the closing curly bracket.
    while ( strcmp( expectToken( tok, fp ), "}" ) != 0 ) {
      if ( len >= cap )
        eList = (Expr **) realloc( eList, ( cap *= 2 ) * sizeof( Expr * ) );
      eList[ len++ ] = parse( tok, fp );
    }
    
    if ( len == 0 ) {
      fprintf( stderr, "line %d: empty compound expression\n", linesRead() );
      exit( EXIT_FAILURE );
    }
    
    return makeCompound( eList, len );
  }

  // Handle language operators (reserved words)
  //Reserved Word: print
  if ( strcmp( tok, "print" ) == 0 ) {
    // Parse the one argument to print, and create a print expression.
    Expr *arg = parse( expectToken( tok, fp ), fp );
    return makePrint( arg );
  }
  
  //Reserved Word: add
  if ( strcmp( tok, "add" ) == 0 ) {
    // Parse the two operands, then make an add expression with them.
    Expr *op1 = parse( expectToken( tok, fp ), fp );
    Expr *op2 = parse( expectToken( tok, fp ), fp );
    return makeAdd( op1, op2 );
  }
  
  //Reserved Word: sub
  if ( strcmp( tok, "sub" ) == 0 ) {
    // Parse the two operands, then make a subtract expression with them.
    Expr *op1 = parse( expectToken( tok, fp ), fp );
    Expr *op2 = parse( expectToken( tok, fp ), fp );
    return makeSub( op1, op2 );
  }
  
  //Reserved Word: mul
  if ( strcmp( tok, "mul" ) == 0 ) {
    // Parse the two operands, then make a multiply expression with them.
    Expr *op1 = parse( expectToken( tok, fp ), fp );
    Expr *op2 = parse( expectToken( tok, fp ), fp );
    return makeMul( op1, op2 );
  }
  
  //Reserved Word: div
  if ( strcmp( tok, "div" ) == 0 ) {
    // Parse the two operands, then make a divide expression with them.
    Expr *op1 = parse( expectToken( tok, fp ), fp );
    Expr *op2 = parse( expectToken( tok, fp ), fp );
    return makeDiv( op1, op2 );
  }
  
  //Reserved Word: equal
  if ( strcmp( tok, "equal" ) == 0 ) {
    // Parse the two operands, then make an equal expression with them.
    Expr *op1 = parse( expectToken( tok, fp ), fp );
    Expr *op2 = parse( expectToken( tok, fp ), fp );
    return makeEqual( op1, op2 );
  }
  
  //Reserved Word: less
  if ( strcmp( tok, "less" ) == 0 ) {
    // Parse the two operands, then make a less expression with them.
    Expr *op1 = parse( expectToken( tok, fp ), fp );
    Expr *op2 = parse( expectToken( tok, fp ), fp );
    return makeLess( op1, op2 );
  }
  
  //Reserved Word: not
  if ( strcmp( tok, "not" ) == 0 ) {
    // Parse the operand, then make a not expression with it.
    Expr *op = parse( expectToken( tok, fp ), fp );
    return makeNot( op );
  }
  
  //Reserved Word: set
  if ( strcmp( tok, "set" ) == 0 ) {
    // Parse the two operands, then make a variable with them.
    char *var = expectToken( tok, fp );
    char *name = (char *)malloc( sizeof( MAX_VAR_NAME + 1 ) );
    printf( "Point 1\n" );
    sprintf( name, "%s", var );
    printf( "Point 2\n" );
    printf ( "%s\n", var );
    Expr *val = parse( expectToken( tok, fp ), fp );
    return makeSet( name, val );
  }
  
  //Reserved Word: if
  if ( strcmp( tok, "if" ) == 0 ) {
    // Parse the two operands, then make an if expression with them.
    Expr *cond = parse( expectToken( tok, fp ), fp );
    Expr *body = parse( expectToken( tok, fp ), fp );
    return makeIf( cond, body );
  }
  
  //Reserved Word: while
  if ( strcmp( tok, "while" ) == 0 ) {
    // Parse the two operands, then make a while expression with them.
    Expr *cond = parse( expectToken( tok, fp ), fp );
    Expr *body = parse( expectToken( tok, fp ), fp );
    return makeWhile( cond, body );
  }

  //Reserved Word: and
  if ( strcmp( tok, "and" ) == 0 ) {
    // Parse the two operands, then make a less expression with them.
    Expr *op1 = parse( expectToken( tok, fp ), fp );
    Expr *op2 = parse( expectToken( tok, fp ), fp );
    return makeAnd( op1, op2 );
  }
  
  //Reserved Word: or
  if ( strcmp( tok, "or" ) == 0 ) {
    // Parse the two operands, then make a less expression with them.
    Expr *op1 = parse( expectToken( tok, fp ), fp );
    Expr *op2 = parse( expectToken( tok, fp ), fp );
    return makeOr( op1, op2 );
  }
  
  //Reserved Word: concat
  if ( strcmp( tok, "concat" ) == 0 ) {
    // Parse the two operands, then make a less expression with them.
    Expr *op1 = parse( expectToken( tok, fp ), fp );
    Expr *op2 = parse( expectToken( tok, fp ), fp );
    return makeConcat( op1, op2 );
  }
  
  //Reserved Word: substr
  if ( strcmp( tok, "substr" ) == 0 ) {
    // Parse the two operands, then make a less expression with them.
    Expr *op1 = parse( expectToken( tok, fp ), fp );
    char *start = expectToken( tok, fp );
    char *end = expectToken( tok, fp );
    return makeSubstr( op1, start, end );
  }
   
  // Complain if we can't make sense of the token.
  fprintf( stderr, "line %d: invalid token \"%s\"\n", linesRead(), tok );
  exit( EXIT_FAILURE );
  
  // Never reached.
  return NULL;
}


int main( int argc, char *argv[] )
{
  // Open the program's source.
  if ( argc != 2 )
    usage();
  FILE *fp = fopen( argv[ 1 ], "r" );
  if ( !fp ) {
    fprintf( stderr, "Can't open file: %s\n", argv[ 1 ] );
    usage();
  }

  // Parse the whole program source into an expression object.
  // The parser uses a one-token lookahead to help parsing compound expressions.
  char tok[ MAX_TOKEN + 1 ];
  Expr *expr = parse( expectToken( tok, fp ), fp );
  
  // If this is a legal input, there shouldn't be any extra tokens at the end.
  if ( nextToken( tok, fp ) ) {
    fprintf( stderr, "line %d: unexpected token \"%s\"\n", linesRead(), tok );
    exit( EXIT_FAILURE );
  }

  fclose( fp );

  // Run the program.
  Context *ctxt = makeContext();
  char *result = expr->eval( expr, ctxt );
  
  // Everything evaluates to a dynamically allocated string, but we don't
  // do anything with the one out of the top-level expression.
  free( result );

  // We're done, free everything.
  freeContext( ctxt );
  expr->destroy( expr );

  return EXIT_SUCCESS;
}
