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
Value *makeNull(){
    Value *emptyList = malloc(sizeof(Value));
    emptyList->type = NULL_TYPE;
    return emptyList;
}

/*
 * Create a nonempty list (a new Value object of type CONS_TYPE).
 */
Value *cons(Value *car, Value *cdr){
    struct ConsCell newCell;
    newCell.car = car;
    newCell.cdr = cdr;
    Value *newList = malloc(sizeof(Value));
    newList->type = CONS_TYPE;
    newList->c = newCell;
    return newList;
}

/*
 * Print a representation of the contents of a linked list.
 */
void display(Value *list){
    for (Value *cur = list; cur->type != NULL_TYPE; cur=cur->c.cdr) {
        switch (cur->c.car->type) {
            case INT_TYPE:
                printf("%i => ", cur->c.car->i);
                break;
            case DOUBLE_TYPE:
                printf("%f => ", cur->c.car->d);
                break;
            case STR_TYPE:
                printf("%s => ", cur->c.car->s);
                break;
            case CONS_TYPE:
                display(cur->c.car);
                break;
            case NULL_TYPE:
                return;
        }
    }
    printf("\n");
}

/*
 * Get the car value of a given list.
 * (Uses assertions to ensure that this is a legitimate operation.)
 */
Value *car(Value *list){
    assert(list->type == CONS_TYPE);
    //printf("num: %s", list->c.car->s);
    return list->c.car;
}

/*
 * Get the cdr value of a given list.
 * (Uses assertions to ensure that this is a legitimate operation.)
 */
Value *cdr(Value *list){
    assert(list->type == CONS_TYPE);
    return list->c.cdr;
}

/*
 * Test if the given value is a NULL_TYPE value.
 * (Uses assertions to ensure that this is a legitimate operation.)
 */
bool isNull(Value *value){
    assert(value != NULL);
    return value->type == NULL_TYPE;
}

/*
 * Compute the length of the given list.
 * (Uses assertions to ensure that this is a legitimate operation.)
 */
int length(Value *value){
    int length = 0;
    for (Value *cur=value; cur; cur = cur->c.cdr) {
        length ++;
    }
    return length - 1;
}

/*
 * Create a new linked list whose entries correspond to the given list's
 * entries, but in reverse order.  The resulting list is a deep copy of the
 * original: that is, there should be no shared memory between the original
 * list and the new one.
 *
 * (Uses assertions to ensure that this is a legitimate operation.)
 *
 * FAQ: What if there are nested lists inside that list?
 * ANS: There won't be for this assignment. There will be later, but that will
 *      be after we've got an easier way of managing memory.
 */
Value *reverse(Value *list){
    assert(list != NULL);
    // Return the list itself if it has type NULL_TYPE
    if (list->type == NULL_TYPE) {
        return list;
    } else {
        printf("type: %i", list->type);
        return cons(cdr(list), list->c.car);
        // recursively call cons
//return cons(reverse(cdr(list)), car(list));
//        Value *hello;
//        return hello;
    }
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
    
}
