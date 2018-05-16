/*
 * This program implements a parser for Scheme code with 
 * functionality to display the parse tree.
 *
 * Author: Yitong Chen, Megan Zhao and Yingying Wang
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"
#include "talloc.h"
#include "linkedlist.h"

/*
 * Check whether the given token is an atom.
 */
bool isAtom(Value *token) {
    int tokenType = token->type;
    return (tokenType == BOOL_TYPE || tokenType == SYMBOL_TYPE ||
           tokenType == INT_TYPE || tokenType == DOUBLE_TYPE ||
           tokenType == STR_TYPE);
}

bool specialChar(Value *ch){
    if ((strcmp(ch->s, " ") != 0)
        && (strcmp(ch->s, "\\n") != 0)
        && (strcmp(ch->s, ";") != 0)
        && (strcmp(ch->s, "\\t") != 0)) {
    	return false;
    }
    return true;
}

/*
 * Helper function for displaying a parse tree to the screen.
 */
void printTreeHelper(Value *tree, Value *prev) {
    Value *cur = tree;
    while (cur != NULL && cur->type != NULL_TYPE) {
        if (isAtom(car(cur))) {
            switch(car(cur)->type) {
                case BOOL_TYPE:
                    if (prev != NULL && prev->type != OPEN_TYPE)
                        printf(" ");
                    printf("%s", car(cur)->s);
                    break;
                case SYMBOL_TYPE:
		    if (!specialChar((car(cur)))){
                            if (prev != NULL && prev->type != OPEN_TYPE)
                                printf(" ");
                            printf("%s", car(cur)->s);
                    }
                    break;
                case INT_TYPE:
                    if (prev != NULL && prev->type != OPEN_TYPE)
                        printf(" ");
                    printf("%d", car(cur)->i);
                    break;
                case DOUBLE_TYPE:
                    if (prev != NULL && prev->type != OPEN_TYPE)
                        printf(" ");
                    printf("%f", car(cur)->d);
                    break;
                case STR_TYPE:
                    if (prev != NULL && prev->type != OPEN_TYPE)
                        printf(" ");
                    printf("%s", car(cur)->s);
                    break;
                default:
                    printf("ERROR\n");
            }
            prev = car(cur);
        } else {
            if (prev != NULL && prev->type != NULL_TYPE && prev->type != OPEN_TYPE)
                printf(" ");
            printf("(");
            prev->type = OPEN_TYPE;
            printTreeHelper(car(cur), prev);
            printf(")");
            prev->type = CLOSE_TYPE;
        }
        cur = cdr(cur);
    }
}

/* 
 * This function returns a parse tree representing a Scheme
 * program on the input of a linkedlist of tokens from that 
 * program.
 *
 * Returns a list representing the parse tree or NULL if 
 * the parsing fails.
 */
Value *parse(Value *tokens) {
    Value *stack = makeNull();
    if (!stack) {
        texit(1);
    }
    
    int depth = 0;
    Value *current = tokens;

    while (current->type != NULL_TYPE) {
        Value *token = car(current);
        if (token->type == OPEN_TYPE) {
            depth ++;
            stack = cons(token, stack);
        } else if (token->type == CLOSE_TYPE) {
            // Pop from the stack until reaching (
            if (depth == 0) {
                printf("Error! Unbalanced use of parentheses!\n");
                texit(1);
            }
            depth --;
            // Access top item in the list
            Value *head = car(stack);
            // Create a list of items being popped
            Value *inner = makeNull();
            while (head->type != OPEN_TYPE) {
                inner = cons(head, inner);
                // Pop off the top item
                stack = cdr(stack);
                head = car(stack);
            }
            // Pop off the (
            stack = cdr(stack);
            // Push the list back on to the stack
            stack = cons(inner, stack);
        } else {
            if (token->type == SYMBOL_TYPE) {
                if (!specialChar(token)) {
                   stack = cons(token, stack); 
                }
            } else {
                stack = cons(token, stack);
            }
        }
        current = cdr(current);
    }
    if (depth != 0) {
        printf("Error! Unbalanced use of parentheses!\n");
        texit(1);
    }
    return reverse(stack);
}

/* 
 * This function displays a parse tree to the screen.
 */
void printTree(Value *tree) {
    assert(tree != NULL &&
           (tree->type == CONS_TYPE || tree->type == NULL_TYPE));
    if (isNull(tree)) {
        printf("()");
        return;
    }
    Value *prev = talloc(sizeof(Value));
    if (!prev) {
        printf("Error! Not enough memory!\n");
        return;
    }
    prev->type = NULL_TYPE; 
    printTreeHelper(tree, prev);
}
