/*
 * proj1: Linked list
 * Due: Friday, 04/27 at 22:00
 * Yitong Chen, Yingying Wang, Megan Zhao
 * 
 * Yitong (1 2 8)
 * Yingying (4 5 9) 
 * Megan (3 6 7)
 * in 1 or 2 sentences describe the
 * purpose of this file.
 */


#include "linkedlist.h"

#include <stdlib.h>
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
 */
Value *cons(Value *car, Value *cdr) {
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
 */
void display(Value *list){
    
}

/*
 * Get the car value of a given list.
 * (Uses assertions to ensure that this is a legitimate operation.)
 */
Value *car(Value *list){
    assert(list->type = CONS_TYPE);
    return list->c.car;
}

/*
 * Get the cdr value of a given list.
 * (Uses assertions to ensure that this is a legitimate operation.)
 */
Value *cdr(Value *list){
    assert(list->type = CONS_TYPE);
    return list->c.cdr;
}

/*
 * Test if the given value is a NULL_TYPE value.
 * (Uses assertions to ensure that this is a legitimate operation.)
 */
bool isNull(Value *value){
    
}

/*
 * Compute the length of the given list.
 * (Uses assertions to ensure that this is a legitimate operation.)
 */
int length(Value *value){
    
}


/*
 * Create a new linked list whose entries correspond to the given list's
 * entries, but in reverse order.  The resulting list is a deep copy of the
 * original.
 * Reversing a non-list Value results in assertion failure.
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
                new_value->s = cur->c.car->s;
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
 * (Uses assertions to ensure that this is a legitimate operation.)
 *
 * FAQ: What if there are nested lists inside that list?
 * ANS: There won't be for this assignment. There will be later, but that will
 *      be after we've got an easier way of managing memory.
*/
void cleanup(Value *list){
   //probably wrong... need to check 
    assert(list != NULL);
    
    Value *next;
    printf("first free:\n");
    
    free(list->c.car);
    
    //always free() what we malloc()
    for (Value *cur = list->c.cdr; cur; cur = next){
        printf("cur:\n");
        display(cur);
        next = cur->c.cdr;
        printf("next:\n");
        display(next);
        free(cur->c.car);
    }
    
}



