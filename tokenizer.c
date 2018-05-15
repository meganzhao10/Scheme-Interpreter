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
#include <ctype.h>
#include "tokenizer.h"
#include "talloc.h"
#include "value.h"

/* 
 * Check whether the input character is a delimiter.
 *
 * Return true if it is and false otherwise.
 * TODO: check whether the ch as a \t is a delimiter.
 */
bool isDelimiter(char ch) {
    return (ch == ' ' || ch == '(' || ch == ')' || ch == '\n'
           || ch == EOF || ch == ';' || ch == '\t');
}

/*
 * Check whether the input character is an initial.
 * 
 * Returns true if ch is an initial, false otherwise.
 */
bool isInitial(char ch){
    return (isalpha(ch) || ch == '!' || ch == '$' || ch == '%' || 
            ch == '&' || ch == '*' || ch == '/' || ch == ':' || ch == '<' ||
            ch == '=' || ch == '>' || ch == '?' || ch == '~' || ch == '_' ||
            ch == '^');
}

/*
 * Check whether the input character is a subsequent.
 * 
 * Returns true if ch is a subsequent, false otherwise.
 */
bool isSubsequent(char ch){
    return (isInitial(ch) || isdigit(ch) || ch == '.' || ch == '+' ||
            ch == '-');
}

/*
 * Given a pointer to an existent (but uninitialized) Vector, allocates the
 * internal values of the Vector to initialize it to be empty.  The initial
 * size of the internally stored array is the given capacity.
 *
 * Returns: 0 for success and 1 for failure
 */
int initVector(Vector *list, int initialCapacity) {
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
int ensureCapacityVector(Vector *list) {
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
void makeRoomVector(int index, Vector *list) {
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
int addVector(Vector *list, int index, char value) {
    // Check whether the index is valid
    if ((index < 0) || (index > list->size)) {
        return 1;
    }
    if (ensureCapacityVector(list)) {
        return 1;
    } else {
        // Insert to the valid indicated position
        makeRoomVector(index, list);
        list->data[index] = value;
        list->size ++;
        return 0;
    }
}

/*
 * Convert the given vector into a string.
 * 
 * Returns a string constructed from the values stored in the vector.
 * Appends additional quotation marks when if isStr is true.
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
        result[list->size + 1] = '"';
        result[list->size + 2] = '\0';
    } else {
        for (int i = 0; i < list->size; i ++) {
            result[i] = list->data[list->size - i - 1];
        }
        result[list->size] = '\0';
    }
    return result;
}

/*
 * Helper function to parse a number.
 *
 * Return true if the parsing is successful, false if the parsing fails.
 */
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
    
    // Construct vector to store the number
    Vector *vector = talloc(sizeof(Vector));
    initVector(vector, 10);
    if (!vector) {
        printf("Error! Out of memory!\n");
        return false;            
    }
    
    bool isFloat = false;
    
    // Check if the number starts with a decimal point
    char nextChar = fgetc(stdin);
    if (!isdigit(nextChar)) {
        if (nextChar == '.') {
            isFloat = true;
            char follow = fgetc(stdin);
            if (!isdigit(follow)) {
                printf("Error! Unrecognized sequence with %c!\n", follow);
                return false;
            }
            ungetc(follow, stdin);
        } else {
            printf("Error! Unrecognized sequence with %c!\n", nextChar);
            return false;
        }
    } 
    addVector(vector, 0, nextChar);
    nextChar = fgetc(stdin);
    while (!isDelimiter(nextChar)) {
        if (nextChar == '.') { 
            // Multiple decimal points are not allowed
            if (isFloat) { 
                printf("Error! Unrecognized sequence"
                       "with multiple decimal points!\n");
                return false;
            } else {
                isFloat = true;
            }
        } else if (!isdigit(nextChar)) {
            printf("Error! Unrecognized sequence with %c in number!\n", nextChar);
            return false;
        }
        addVector(vector, 0, nextChar);
        nextChar = fgetc(stdin);
    }
    // Convert vector into string
    char *valueStr = convertVector(vector, false);
    
    // Convert string into numeric values
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
    
    // Restore the delimiter
    ungetc(nextChar, stdin);
    return true;
}


/*
 * Helper function to parse a string.
 *
 * Return true if the parsing is successful, false if the parsing fails.
 */
bool parseString(Value *entry) {
    bool balanced = false;
    
    // Create a vector to store the string
    Vector *vector = talloc(sizeof(Vector));
    initVector(vector, 10);
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
                    addVector(vector, 0, '\n');
                    break;
                case 't':
                    addVector(vector, 0, '\t');
                    break;
                case '"':
                    addVector(vector, 0, '\"');
                    break;
                case '\'':
                    addVector(vector, 0, '\'');
                    break;
                case '\\':
                    addVector(vector, 0, '\\');
                    break; 
                default:
                    printf("Error! Unknown escape string!\n");
                    return false;
            }
        } else {
            addVector(vector, 0, nextChar);
        }
        nextChar = fgetc(stdin);
    }
    // Raise error if the string is not closed
    if (!balanced) {
        printf("Error! Expected a closing \"!\n");
        return false;
    }
    entry->type = STR_TYPE;
    entry->s = convertVector(vector, true);
    return true;
}

/*
 * Helper function to parse a boolean.
 *
 * Return true if the parsing is successful, false if the parsing fails.
 */
bool parseBool(Value *entry) {
    char lookAhead = fgetc(stdin);
    if (lookAhead == 't' || lookAhead == 'f') {
        char follow = fgetc(stdin);
        if (isDelimiter(follow)) {
            ungetc(follow, stdin);
            entry->type = BOOL_TYPE;
            if (lookAhead == 't') {
                entry->s = "#t";
            } else {
                entry->s = "#f";
            }
        } else {
            printf("Error! Unrecognized boolean sequence!\n");
            return false;
        }
    } else {
            printf("Error! Unrecognized boolean sequence!\n");
            return false;
    }
    return true;
}

/*
 * Helper function to parse an identifier.
 *
 * Return true if the parsing is successful, false if the parsing fails.
 */
bool parseIdentifier(Value *entry) {
    // Construct vector to store the identifier
    Vector *vector = talloc(sizeof(Vector));
    initVector(vector, 10);
    if (!vector) {
        printf("Error! Out of memory!\n");
        return false;            
    }
    
    char nextChar = fgetc(stdin);
    
    while (!isDelimiter(nextChar)) {
        if (isSubsequent(nextChar)) {
            addVector(vector, 0, nextChar);
        } else {
            printf("Error! Unrecognized identifier sequence!\n");
            return false;
        }
        nextChar = fgetc(stdin);
    }
    entry->type = SYMBOL_TYPE;
    entry->s = convertVector(vector, false);
    // Restore the delimiter
    ungetc(nextChar, stdin);
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
        Value *entry = talloc(sizeof(Value));
        if (!entry) {
            printf("Error! Not enough memory!\n");
            return entry;
        }
        if (charRead == '(') {
            entry->type = OPEN_TYPE;
        } else if (charRead == ')') {
            entry->type = CLOSE_TYPE;
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
            if (isDelimiter(nextChar)) {
                entry->type = SYMBOL_TYPE;
                switch (charRead) {
                    case '+':
                        entry->s = "+";
                        break;
                    case '-':
                        entry->s = "-";
                        break;
                }
                ungetc(nextChar, stdin);
            } else if (isdigit(nextChar) || nextChar == '.') {
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
            charRead = fgetc(stdin);
            continue;
        } else if (charRead == '\n' || charRead == '\t' || charRead == ' ') {
            charRead = fgetc(stdin);
            continue;
        } else if (isdigit(charRead) || charRead == '.') {
            ungetc(charRead, stdin);
            bool success = parseNumber(entry);
            if (!success) {
                return NULL;
            }
        } else {
            printf("Error! Unrecognized symbol %c in input!\n", charRead);
            return NULL;
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

