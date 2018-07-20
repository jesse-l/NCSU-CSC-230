/**
  @file basic.h
  @author David Sturgill (dbsturgi)

  Representation for basic types of expressions, the stuff you are given at the start of
  the assignment.
*/

#ifndef _BASIC_H_
#define _BASIC_H_

#include "core.h"

/** Make a literal expressin that evaluates to the given string.
    @param val value this expression evaluates to.  This should be a dynamically allocated
    string that the expression will be responsible for freeing.
    @return a new expression that evaluates to a copy of the given value.
 */
Expr *makeLiteral( char *val );

/** Make an expressin that evaluates and prints the given expression argument.
    @param arg expression to print.  The print expression will be responsible for freeing arg.
    @return a new expression that prints when evaluated.
 */
Expr *makePrint( Expr *arg );

/** Make a compound expression, representing the sequence of expressions 
    @param eList list of subexpressions to evaluate.  The compound expression
    will be responsible for freeing this list and the subexpressions it contains.
    @param len number of expressions in eList.
    @return a new expression that evaluates all the expressions in eList.
 */
Expr *makeCompound( Expr **eList, int len );

#endif
