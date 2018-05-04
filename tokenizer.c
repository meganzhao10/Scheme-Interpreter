/*
 * This program implements a tokenizer for Scheme.
 *
 * Authors: Yitong Chen, Yingying Wang, Megan Zhao
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokenizer.h"

/* 
 * This function tokenizes the input stream.
 *
 * Return: a null pointer if memory allocation fails
 *         a list of tokens otherwise
 */
Value *tokenize(){
    char charRead;
    // Initialize the list to store the tokens
    Value *list = makeNull();
    // Check whether initialization is successful
    if (list == NULL) {
        printf("Error! Not enough memory!\n");
        return list;
    }
    charRead = fgetc(stdin);
    while (charRead != EOF) {
//        printf("%c\n", charRead);
        Value *entry = talloc(sizeof(Value));
        if (!entry) {
            printf("Error! Not enough memory!\n");
            return entry;
        }
        switch(charRead) {
            case '(':
                entry->type = OPEN_TYPE;
                break;
            case ')':
                entry->type = CLOSE_TYPE;
                break;
            default: 
                entry->type = BOOL_TYPE;
                entry->s = "#t";
        }
        list = cons(entry, list);
        charRead = fgetc(stdin);
    }
    return reverse(list);
}

/* 
 * The displayTokens function takes a linked list of tokens as 
 * input, and displays those tokens, one per line, with each 
 * token's type.
 */
void displayTokens(Value *list){
    assert(list != NULL &&
           (list->type == CONS_TYPE || list->type == NULL_TYPE));
    Value *cur = list;
    while (cur != NULL && cur->type != NULL_TYPE) {
        switch(car(cur)->type) {
            case OPEN_TYPE:
                printf("(:open\n");
                break;
            case CLOSE_TYPE:
                printf("):close\n");
                break;
            case BOOL_TYPE:
                printf("%s:boolean\n", car(cur)->s);
                break;
            case SYMBOL_TYPE:
                printf("%s:symbol\n", car(cur)->s);
                break;
            case INT_TYPE:
                printf("%d:integer\n", car(cur)->i);
                break;
            case DOUBLE_TYPE:
                printf("%f:double\n", car(cur)->d);
                break;
            case STR_TYPE:
                printf("%s:string\n", car(cur)->s);
                break;
            default:
                printf("ERROR\n");
        }
        cur = cdr(cur);
    }
}