/**
  @file basic.h
  @author David Sturgill (dbsturgi)

  Representation for extra language funcitonality, mostly expression types added
  by the student.
*/

#ifndef _EXTRA_H_
#define _EXTRA_H_

#include "core.h"
#include "basic.h"

/** Make an expression that interprets its operands as long ints and
    evaluates to their sum.
    @param op1 expression for the left-hand operand
    @param op2 expression for the right-hand operand
    @return a new expression object that adds the values of op1 and op2
 */
Expr *makeAdd( Expr *op1, Expr *op2 );

/** Make an expression that interprets its operands as long ints and
    evaluates to their difference.
    @param op1 expression for the left-hand operand
    @param op2 expression for the right-hand operand
    @return a new expression object that subtracts the values of op1 and op2
 */
Expr *makeSub( Expr *op1, Expr *op2 );

/** Make an expression that interprets its operands as long ints and
    multiplies them together.
    @param op1 expression for the left-hand operand
    @param op2 expression for the right-hand operand
    @return a new expression object that multiples the values of op1 and op2
 */
Expr *makeMul( Expr *op1, Expr *op2 );

/** Make an expression that interprets its operands as long ints and
    divides the first operand by the second.
    @param op1 expression for the left-hand operand
    @param op2 expression for the right-hand operand
    @return a new expression object that divides the values of op1 and op2
 */
Expr *makeDiv( Expr *op1, Expr *op2 );

/** Make an expression that interprets its operands as long ints and
    checks to see if they are equal.
    @param op1 expression for the left-hand operand
    @param op2 expression for the right-hand operand
    @return a new expression object that tells if op1 is equal to op2
 */
Expr *makeEqual( Expr *op1, Expr *op2 );

/** Make an expression that interprets its operands as long ints and
    checks to see if op1 is less than op2.
    @param op1 expression for the left-hand operand
    @param op2 expression for the right-hand operand
    @return a new expression object that tells if op1 is less than op2
 */
Expr *makeLess( Expr *op1, Expr *op2 );


Expr *makeNot( Expr *op1 );

/** Make an expression that creates a new variable
    @param name The name of the new variable
    @return a new expression object that is a variable
 */ 
Expr *makeVariable( char const *name );

/** Make an expression that interprets its operands and creates a new
    variable.
    @param name This is the name of the new variable
    @param expr This is the value of the variable
    @return a new expression object that is a variable
 */
Expr *makeSet( char const *name, Expr *expr );

/** Make an expression that interprets its operands and creates an if
    statement.
    @param cond The condition of the if statement
    @param body The body of the if statement
    @return a new expression object that is an if statement
 */
Expr *makeIf( Expr *cond, Expr *body );

/** Make an expression that interprets its operands and creates an while
    loop.
    @param cond The condition of the while loop
    @param body The body of the while loop
    @return a new expression object that is a while loop
 */
Expr *makeWhile( Expr *cond, Expr *body );

//////////////////////////////////////////////////////////////////////
//Extra Credit Functionality
/** takes two expressions as operands and evaluates its first operand
    and if true then evaluates the second operand and if it is true it
    returns true("true") else it returns false(Empty String).
*/
Expr *makeAnd( Expr *op1, Expr *op2 );

/** takes two expressions and evaluates that first and if false then 
    evaluates the second and if that is false then it evaluates to false
    (Empty String) otherwise it results to true.
*/
Expr *makeOr( Expr *op1, Expr *op2 );

/** takes two expressions and it evaluates to the concatenation of the string.
*/
Expr *makeConcat( Expr *op1, Expr *op2 );

/** takes three expressions and evaluates the first as a string and the second
    two as numbers and it pulls a string from the first number until the second
    number excluding the last spot.
*/
Expr *makeSubstr( Expr *op1, Expr *start, Expr *end );

#endif
