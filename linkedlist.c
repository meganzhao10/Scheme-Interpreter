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

#include <stdlib.h>
#include "linkedlist.h"
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
 * (Uses assertions to ensure that this is a legitimate operation.)
 */
Value *car(Value *list){
    
}

/*
 * Get the cdr value of a given list.
 * (Uses assertions to ensure that this is a legitimate operation.)
 */
Value *cdr(Value *list){
    
}

/*
 * Test if the given value is a NULL_TYPE value.
 * (Uses assertions to ensure that this is a legitimate operation.)
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
 * (Uses assertions to ensure that this is a legitimate operation.)
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
        assert(list != NULL);
        Value *next;
        //always free() what we malloc()
        
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
