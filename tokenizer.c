/*
 *  Authors: Yitong Chen, Yingying Wang, Megan Zhao
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"
#include "tokenizer.h"
#include "talloc.h"
#include "value.h"

bool isLetter(char ch){
    if (ch == 'a' || ch == 'b' || ch == 'c' || ch == 'd' || ch == 'e' ||
        ch == 'g' || ch == 'h' || ch == 'i' || ch == 'j' || ch == 'k' ||
        ch == 'l' || ch == 'm' || ch == 'n' || ch == 'o' || ch == 'p' ||
        ch == 'q' || ch == 'r' || ch == 's' || ch == 't' || ch == 'f' ||
        ch == 'v' || ch == 'w' || ch == 'x' || ch == 'y' || ch == 'z' ||
        ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D' || ch == 'E' ||
        ch == 'G' || ch == 'H' || ch == 'I' || ch == 'J' || ch == 'K' ||
        ch == 'L' || ch == 'M' || ch == 'N' || ch == 'O' || ch == 'P' ||
        ch == 'Q' || ch == 'R' || ch == 'S' || ch == 'T' || ch == 'F' ||
        ch == 'V' || ch == 'W' || ch == 'X' || ch == 'Y' || ch == 'Z') {
        return true;
    }
    return false;
}

bool isDigit(char ch){
    if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' ||
        ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9') {
        return true;
    }
    return false;
}



/* The tokenize function should read stdin (the standard input 
 * stream in C) in its entirety and return a linked list consisting 
 * of all tokens found.
 */
Value *tokenize(){
    char charRead;
    Value *list = makeNull();
    charRead = fgetc(stdin);
    while (charRead != EOF) {
        // The character is an open parenthesis '('
        if (charRead == '('){
            Value *newValue = talloc(sizeof(Value));
            newValue->type = OPEN_TYPE;
            newValue->s = talloc(sizeof(char));
            strcpy(newValue->s, '(');
            cons(newValue, list);
        } // The character is a close parenthesis ')'
        else if (charRead == ')') { 
            Value *newValue = talloc(sizeof(Value));
            newValue->type = CLOSE_TYPE;
            newValue->s = talloc(sizeof(char));
            strcpy(newValue->s, ')');
            cons(newValue, list);
        }
        charRead = fgetc(stdin);
    }
    return reverse(list);
}

/* The displayTokens function takes a linked list of tokens as 
 * input, and displays those tokens, one per line, with each 
 * token's type.
 */
void displayTokens(Value *list){
}
