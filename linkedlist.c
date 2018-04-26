/*
 * proj1: Linked list
 * Due: Friday, 04/27 at 22:00
 * Yitong Chen, Yingying Wang, Megan Zhao
 * 
 * This project implements the Scheme list as an linkedlist
 * using C.
 */

#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/*
 * Create an empty list (a new Value object of type NULL_TYPE).
 */
Value *makeNull() {
    Value *nullList = malloc(sizeof(Value));
    nullList->type = NULL_TYPE;
    return nullList;
}

/*
 * Create a nonempty list (a new Value object of type CONS_TYPE).
 * Assume that car is not a list (so no nested list)
 */
Value *cons(Value *car, Value *cdr) {
    assert(car->type != CONS_TYPE && car->type != NULL_TYPE);
    struct ConsCell cell;
    cell.car = car;
    cell.cdr = cdr;
    Value *newValue = malloc(sizeof(Value));
    newValue->type = CONS_TYPE;
    newValue->c = cell;
    return newValue;
}

/*
 * Print a representation of the contents of a linked list.
 * Assume that list is CONS_TYPE 
 */
void display(Value *list){
    assert(list->type == CONS_TYPE);
    Value *cur = list;
    while(cur->type != NULL_TYPE){
        switch(cur->c.car->type){
            case INT_TYPE:
                printf("type:INT_TYPE; value: %i\n",cur->c.car->i);
                break;
            case DOUBLE_TYPE:
                printf("type:DOUBLE_TYPE; value: %f\n",cur->c.car->d);
                break;
            case STR_TYPE:
                printf("type:STRING_TYPE; value: %s\n",cur->c.car->s);
                break;
            default:
                printf(" ");
                break;     
        }
        cur = cur->c.cdr;
    }
}

/*
 * Get the car value of a given list.
 * 
 * Asserts that this function can only be called on a non-empty list 
 * (Value of type CONS_TYPE).
 */
Value *car(Value *list){
    assert(list->type == CONS_TYPE);
    return list->c.car;
}

/*
 * Get the cdr value of a given list.
 * 
 * Asserts that this function can only be called on a non-empty list 
 * (Value of type CONS_TYPE).
 */
Value *cdr(Value *list){
    assert(list->type == CONS_TYPE);
    return list->c.cdr;
}

/*
 * Test if the given value is a NULL_TYPE value.
 *
 * Asserts that the list has been allocated.
 */
bool isNull(Value *value){
    assert(value != NULL);
    if (value->type == NULL_TYPE){
        return true;
    } else{
        return false;
    }
}

/*
 * Compute the length of the given list.
 * 
 * Asserts that the list has been allocated.
 */
int length(Value *value){
    assert(value != NULL);
    int length = 0;
    Value *cur;
    cur = value;
    while (cur->type != NULL_TYPE){
        length++;
        cur = cur->c.cdr;
    }
    return length;
}


/*
 * Create a new linked list whose entries correspond to the given list's
 * entries, but in reverse order.  The resulting list is a deep copy of the
 * original.
 * 
 * Asserts that the reverse function can only be called on a list.
 */
Value *reverse(Value *list) {
    // Reverse can only be applied to an empty list or a non-empty list 
    assert(list->type == NULL_TYPE || list->type == CONS_TYPE);
    // Create new linked list
    Value *reversed = makeNull();
    if (list->type == NULL_TYPE) {
        return reversed;
    }
    for (Value *cur = list; cur->type != NULL_TYPE; cur = cur->c.cdr) {
        // Allocate space for the deep copy and make the copy
        Value *new_value = malloc(sizeof(Value));
        new_value->type = cur->c.car->type;
        switch (cur->c.car->type) {
            case INT_TYPE:
                new_value->i = cur->c.car->i;
                break;
            case DOUBLE_TYPE:
                new_value->d = cur->c.car->d;
                break;
            case STR_TYPE:
                new_value->s = malloc(10 * sizeof(char));
                strcpy(new_value->s, cur->c.car->s);
                break;
            case CONS_TYPE:
                new_value->c = cur->c.car->c;
                break;
            case NULL_TYPE:
                break;
        } 
        reversed = cons(new_value, reversed);
    }
    return reversed;
}


/*
 * Frees up all memory directly or indirectly referred to by list.
 *
 * Asserts that the list to be cleaned up has been allocated.
*/
void cleanup(Value *list){

    assert(list != NULL);
    Value *next;
                         
    for (Value *cur = list; cur->type!=NULL_TYPE; cur = next){
        if (cur->c.car->type == STR_TYPE){   
            free(cur->c.car->s);
         }
         next = cur->c.cdr;
         free(cur->c.car);
         free(cur);
    }
    free(next);
}




