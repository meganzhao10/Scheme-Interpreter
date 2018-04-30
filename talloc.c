/* This code implements a garbage collector to manage memory 
 * usage throughout the interpreter project.
 *
 * Author: Yitong Chen, Megan Zhao, Yingying Wang
 */
#include <stdio.h>
#include "talloc.h"

// The global static variable
static Value* active_list;

/*
 * Create an empty list (a new Value object of type NULL_TYPE).
 *
 * Returns a pointer to an empty list.
 * If memory allocation fails, returns a null pointer.
 */
Value *talloc_makeNull() {
    Value *nullList = malloc(sizeof(Value));
    if (!nullList) {
        printf("Out of memory!\n");
        return nullList;
    }
    nullList->type = NULL_TYPE;
    return nullList;
}

/*
 * Create a nonempty list (a new Value object of type CONS_TYPE).
 *
 * Returns a pointer to a non-empty list.
 * If memory allocation fails, returns a null pointer.
 * Asserts that car is not a list (so no nested list)
 */
Value *talloc_cons(Value *car, Value *cdr) {
    struct ConsCell cell;
    cell.car = car;
    cell.cdr = cdr;
    Value *newValue = malloc(sizeof(Value));
    if (!newValue) {
        printf("Out of memory!\n");
        return newValue;
    }
    newValue->type = CONS_TYPE;
    newValue->c = cell;
    return newValue;
}

/*
 * A malloc-like function that allocates memory, tracking all allocated
 * pointers in the "active list."  (You can choose your implementation of the
 * active list, but whatever it is, your talloc code should NOT call functions
 * in linkedlist.h; instead, implement any list-like behavior directly here.
 * Otherwise you'll end up with circular dependencies, since you're going to
 * modify the linked list to use talloc instead of malloc.)
 */
void *talloc(size_t size) {
    if (active_list == NULL) {
        active_list = talloc_makeNull();
    }
    void *new_pointer = malloc(size);
    if (!new_pointer) {
        printf("Out of memory!\n");
        return new_pointer;
    }
    active_list = talloc_cons(new_pointer, active_list);
    return new_pointer;
}

/*
 * Free all pointers allocated by talloc, as well as whatever memory you
 * malloc'ed to create/update the active list.
 */
void tfree() {
    Value *next;
    next = active_list;                     
    for (Value *cur = active_list; cur->type!=NULL_TYPE; cur = next){
        next = cur->c.cdr;
        free(cur->c.car);
        free(cur);
    }
    free(next);
    active_list = NULL;
}

/*
 * A simple two-line function to stand in the C function "exit", which calls
 * tfree() and then exit().  (You'll use this later to allow a clean exit from
 * your interpreter when you encounter an error: so memory can be automatically
 * cleaned up when exiting.)
 */
void texit(int status) {
    tfree();
    exit(status);
}
