#include <stdlib.h>
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"

#ifndef TOKENIZER_H
#define TOKENIZER_H

struct Vector {
   char* data;      /* A pointer to a traditional C array to hold data */
   int capacity;   /* The size of the actual C array that you have allocated */
   int size;       /* The number of data items actually stored in the Vector */
};

/* Introduces "Vector" as an abbreviation for "struct Vector", for ease. */
typedef struct Vector Vector;

/* The tokenize function should read stdin (the standard input 
 * stream in C) in its entirety and return a linked list consisting 
 * of all tokens found.
 */
Value *tokenize(FILE *src);

/* 
 * The displayTokens function takes a linked list of tokens as 
 * input, and displays those tokens, one per line, with each 
 * token's type.
 */
void displayTokens(Value *list);

#endif
