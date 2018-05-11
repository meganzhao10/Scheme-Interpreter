#include <stdlib.h>
#include "value.h"

#ifndef PARSER_H
#define PARSER_H

/* 
 * This function returns a parse tree representing a Scheme
 * program on the input of a linkedlist of tokens from that 
 * program.
 *
 * Returns a list representing the parse tree or NULL if 
 * the parsing fails.
 */
Value *parse(Value *tokens);

/* 
 * This function displays a parse tree to the screen.
 */
void printTree(Value *tree);

#endif