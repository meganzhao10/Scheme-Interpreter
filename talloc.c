#include <stdlib.h>
#include "value.h"
#include <assert.h>
#include <stdio.h>
#include "talloc.h"



static Value *head;

/*
 * Create a nonempty list (a new Value object of type CONS_TYPE).
 *
 * Returns a pointer to a non-empty list.
 * If memory allocation fails, returns a null pointer.
 * Asserts that car is not a list (so no nested list)
 */
Value *talloc_cons(Value *car, Value *cdr) {
    assert(car->type != CONS_TYPE && car->type != NULL_TYPE);
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
void *talloc(size_t size){
	Value *newHead = malloc(sizeof(Value));
	if (!newHead){
		printf("Out of memory!\n");
	}
	newHead->type = PTR_TYPE;
	newHead->p = malloc(size);
	if (!newHead->p){
		printf("Out of memory!\n");
	}
	head = talloc_cons(newHead, head);
	return newHead->p;
}

/*
 * Free all pointers allocated by talloc, as well as whatever memory you
 * malloc'ed to create/update the active list.
 */
void tfree(){
	Value *cur = head;
	Value *next;
	while (cur != NULL && cur->type == CONS_TYPE){
		next = cur->c.cdr;
		free(cur->c.car->p);
		free(cur->c.car);
		free(cur);
		cur = next;
	} 
	head = NULL;
}

/*
 * A simple two-line function to stand in the C function "exit", which calls
 * tfree() and then exit().  (You'll use this later to allow a clean exit from
 * your interpreter when you encounter an error: so memory can be automatically
 * cleaned up when exiting.)
 */
void texit(int status){
	tfree();
	exit(status);
}


