/*
 * This program implements a tokenizer for Scheme.
 *
 * Authors: Yitong Chen, Yingying Wang, Megan Zhao
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tokenizer.h"

/*
 * Check whether the input character is a letter.
 * 
 * Returns true if ch is a letter, false otherwise.
 */
bool isLetter(char ch){
    return (ch == 'a' || ch == 'b' || ch == 'c' || ch == 'd' || ch == 'e' ||
        ch == 'g' || ch == 'h' || ch == 'i' || ch == 'j' || ch == 'k' ||
        ch == 'l' || ch == 'm' || ch == 'n' || ch == 'o' || ch == 'p' ||
        ch == 'q' || ch == 'r' || ch == 's' || ch == 't' || ch == 'u' ||
        ch == 'v' || ch == 'w' || ch == 'x' || ch == 'y' || ch == 'z' ||
        ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D' || ch == 'E' ||
        ch == 'G' || ch == 'H' || ch == 'I' || ch == 'J' || ch == 'K' ||
        ch == 'L' || ch == 'M' || ch == 'N' || ch == 'O' || ch == 'P' ||
        ch == 'Q' || ch == 'R' || ch == 'S' || ch == 'T' || ch == 'F' ||
        ch == 'V' || ch == 'W' || ch == 'X' || ch == 'Y' || ch == 'Z');
}

/*
 * Check whether the input character is an initial.
 * 
 * Returns true if ch is an initial, false otherwise.
 */
bool isInitial(char ch){
    return (isLetter(ch) || ch == '!' || ch == '$' || ch == '%' || 
            ch == '&' || ch == '*' || ch == '/' || ch == ':' || ch == '<' ||
            ch == '=' || ch == '>' || ch == '?' || ch == '~' || ch == '_' ||
            ch == '^');
}

/*
 * Check whether the input character is a digit.
 * 
 * Returns true if ch is a digit, false otherwise.
 */
bool isDigit(char ch){
    if (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' ||
        ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9') {
        return true;
    }
    return false;
}

/*
 * Check whether the input character is a subsequent.
 * 
 * Returns true if ch is a subsequent, false otherwise.
 */
bool isSubsequent(char ch){
    return (isInitial(ch) || isDigit(ch) || ch == '.' || ch == '+' ||
            ch == '-');
}

/*
 * Given a pointer to an existent (but uninitialized) Vector, allocates the
 * internal values of the Vector to initialize it to be empty.  The initial
 * size of the internally stored array is the given capacity.
 *
 * Returns: 0 for success
 *          1 for failure
 */
int init_vector(Vector *list, int initialCapacity) {
    list->capacity = initialCapacity;
    list->size = 0;
    char* data = talloc(sizeof(char) * initialCapacity);
    if (!data) {
        printf("Out of memory!");
        return 1;
    }
    list->data = data;
    return 0;
}

/* 
 * Helper function to ensure that the given list has room
 * for additional items.
 * 
 * Returns: 0 for success
 *          1 for failure
 */
int ensure_capacity_vector(Vector *list) {
    if (list->size == list->capacity) {
        list->capacity = list->capacity * 2;
        char* new_data = talloc(sizeof(char) * list->capacity);
        // check whether allocation is successful
        if (!new_data) {
            printf("Out of memory!");
            return 1;
        }
        // copy data over to the new array
        for (int i = 0; i < list->size; i ++) {
            new_data[i] = list->data[i];
        }
        list->data = new_data;
        return 0;
    } else {
        return 0;
    }
}

/* 
 * Helper function to make room for the new entry.
 */
void make_room_vector(int index, Vector *list) {
    for (int i = list->size - 1; i >= index; i --) {
        list->data[i + 1] = list->data[i];
    }
}

/*
 * Inserts the given value at the given index inside the given Vector.
 * 
 * Returns: 0 for success
 *          1 for failure
 */
int add(Vector *list, int index, char value) {
    // Check whether the index is valid
    if ((index < 0) || (index > list->size)) {
        return 1;
    }
    if (ensure_capacity_vector(list)) {
        return 1;
    } else {
        // Insert to the valid indicated position
        make_room_vector(index, list);
        list->data[index] = value;
        list->size ++;
        return 0;
    }
}

/*
 * Return the values stored in a vector as a string.
 */
char *convertVector(Vector *list, bool isStr) {
    char *result = talloc(sizeof(char) * (list->size) + 3);
    if (!result) {
        printf("Error! Not enough memory!\n");
        return NULL;
    }
    if (isStr) {
        result[0] = '"';
        for (int i = 1; i <= list->size; i ++) {
            result[i] = list->data[list->size - i];
        }
        result[list->size+1] = '"';
    } else {
        for (int i = 0; i < list->size; i ++) {
            result[i] = list->data[list->size - i - 1];
        }
    }
    return result;
}

bool parseNumber(Value *entry) {
    char sign = fgetc(stdin);
    bool neg;
    if (sign == '+') {
        neg = false;
    } else if (sign == '-') {
        neg = true;
    } else {
        ungetc(sign, stdin);
    }
    
    Vector *vector = talloc(sizeof(Vector));
    init_vector(vector, 10);
    if (!vector) {
        printf("Error! Out of memory!\n");
        return false;            
    }
    bool isFloat = false;
    
    char nextChar = fgetc(stdin);
    if (!isDigit(nextChar)) {
        if (nextChar == '.') {
            isFloat = true;
            char follow = fgetc(stdin);
            if (!isDigit(follow)) {
                printf("Error! Unrecognized sequence!\n");
                return false;
            }
            ungetc(follow, stdin);
        } else {
            printf("Error! Unrecognized sequence!\n");
            return false;
        }
    } 
    add(vector, 0, nextChar);
    nextChar = fgetc(stdin);
    while (nextChar != EOF && nextChar != ' ' && nextChar != ')') {
        if (nextChar == '.') {
            if (isFloat) {
                printf("Error! Unrecognized sequence!\n");
                return false;
            } else {
                isFloat = true;
            }
        } else if (!isDigit(nextChar)) {
            printf("Error! Unrecognized sequence!\n");
            return false;
        }
        add(vector, 0, nextChar);
        nextChar = fgetc(stdin);
    }
    char *valueStr = convertVector(vector, false);
    if (isFloat) {
        entry->type = DOUBLE_TYPE;
        double value = atof(valueStr);
        if (neg) {
            value = value * -1;
        }   
        entry->d = value;
    } else {
        entry->type = INT_TYPE;
        int value = atoi(valueStr);
        if (neg) {
            value = value * -1;
        }   
        entry->i = value;
    }
    
    if (nextChar == ')') {
        ungetc(')', stdin);
    }
    return true;
}

bool parseString(Value *entry) {
    bool balanced = false;
    entry->type = STR_TYPE;
    // Create a vector to store the string
    Vector *vector = talloc(sizeof(Vector));
    init_vector(vector, 10);
    if (!vector) {
        printf("Error! Out of memory!\n");
        return false;            
    }
    char nextChar = fgetc(stdin);
    while (nextChar != EOF) {
        if (nextChar == '"') {
            balanced = true;
            break;
        }
        if (nextChar == '\\') {
            char escaped = fgetc(stdin);
            switch (escaped) {
                case 'n':
                    add(vector, 0, '\n');
                    break;
                case 't':
                    add(vector, 0, '\t');
                    break;
                case '"':
                    add(vector, 0, '\"');
                    break;
                case '\'':
                    add(vector, 0, '\'');
                    break;
                case '\\':
                    add(vector, 0, '\\');
                    break; 
                default:
                    printf("Error! Unknown escape string!\n");
                    return false;
            }
        } else {
            add(vector, 0, nextChar);
        }
        nextChar = fgetc(stdin);
    }
    if (!balanced) {
        printf("Error! Expected a closing \"!\n");
        return false;
    }
    entry->s = convertVector(vector, true);
    return true;
}


bool parseBool(Value *entry) {
    entry->type = BOOL_TYPE;
            char lookAhead = fgetc(stdin);
            if (lookAhead == 't') {
                entry->s = "#t";
            } else if (lookAhead == 'f') {
                entry->s = "#f";
            } else {
                printf("Error! Unrecognized symbol!\n");
                return false;
            }
    return true;
}

bool parseIdentifier(Value *entry) {
    entry->type = SYMBOL_TYPE;
    Vector *vector = talloc(sizeof(Vector));
    init_vector(vector, 10);
    if (!vector) {
        printf("Error! Out of memory!\n");
        return NULL;            
    }
    char nextChar = fgetc(stdin);
    while (nextChar != EOF && nextChar != ')' && nextChar != ' ') {
        if (isSubsequent(nextChar)) {
            add(vector, 0, nextChar);
        } else {
            printf("Error! Unrecognized sequence!\n");
            return false;
        }
        nextChar = fgetc(stdin);
    }
    entry->s = convertVector(vector, false);
    if (nextChar == ')') {
        ungetc(')', stdin);
    }
    return true;
}

/* 
 * This function tokenizes the input stream.
 *
 * Return a null pointer if memory allocation fails or scanning error.
 * Returns a list of tokens otherwise.
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
//        printf("new: %c\n", charRead);
        Value *entry = talloc(sizeof(Value));
        if (!entry) {
            printf("Error! Not enough memory!\n");
            return entry;
        }
        if (charRead == '(') {
            entry->type = OPEN_TYPE;
        } else if (charRead == ')') {
            entry->type = CLOSE_TYPE;
        } else if (charRead == ' ') {
            entry->type = SYMBOL_TYPE;
            entry->s = " ";
        } else if (charRead == '#') {
            bool success = parseBool(entry);
            if (!success) {
                return NULL;
            }
        } else if (charRead == '"') {
            bool success = parseString(entry);
            if (!success) {
                return NULL;
            }
        } else if (charRead == '+' || charRead == '-') {
            char nextChar = fgetc(stdin);
            if (nextChar == ' ' || nextChar == ')') {
                entry->type = SYMBOL_TYPE;
                switch (charRead) {
                    case '+':
                        entry->s = "+";
                        break;
                    case '-':
                        entry->s = "-";
                        break;
                }
                if (nextChar == ')') {
                    ungetc(')', stdin);
                } 
            } else if (isDigit(nextChar)) {
                ungetc(nextChar, stdin);
                ungetc(charRead, stdin);
                bool success = parseNumber(entry);
                if (!success) {
                    return NULL;
                }
            } else {
                printf("Error! Illegal identifier!\n");
                return NULL;
            }
        } else if (isInitial(charRead)) {
//            printf("run here! %c\n", charRead);
            ungetc(charRead, stdin);
            bool success = parseIdentifier(entry);
            if (!success) {
                return NULL;
            }
        } else if (charRead == ';') {

            char nextChar = fgetc(stdin);
            while (nextChar != EOF && nextChar != '\n') {
                nextChar = fgetc(stdin);
            }
            entry->type = SYMBOL_TYPE;
            entry->s = ";";
        } else if (charRead == '\n') {
            entry->type = SYMBOL_TYPE;
            entry->s = "\\n";
        } else if (isDigit(charRead) || charRead == '.') {
            ungetc(charRead, stdin);
            bool success = parseNumber(entry);
            if (!success) {
                return NULL;
            }
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
                if ((strcmp(car(cur)->s, " ") != 0) 
                    && (strcmp(car(cur)->s, "\\n") != 0)
                    && (strcmp(car(cur)->s, ";") != 0)) {
                    printf("%s:symbol\n", car(cur)->s);
                }
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

