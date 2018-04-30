#include "talloc.h"

#include <assert.h>
#include <stdio.h>

static Value *head;



/*
 * Create an empty list (a new Value object of type NULL_TYPE).
 *
 * Returns a pointer to an empty list.
 * If memory allocation fails, returns a null pointer.
 */
Value *tallocMakeNull() {
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
Value *tallocCons(Value *car, Value *cdr) {
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
    if (head == NULL){
        head = tallocMakeNull();
    }
    void *newMemory = malloc(size);
    if (!newMemory){
        printf ("Out of Memory! \n");
        //return ;  它不让我只return；
    }
    Value *newPointer = tallocMakeNull();
    newPointer->type = PTR_TYPE;
    newPointer->p = newMemory;
    head = tallocCons(newPointer, head);
    
    return newMemory;
}
        
        
void helperForTfree(Value *list){
    if (list->type ==CONS_TYPE){
        free(list->c.car->p);
        free(list->c.car);
        helperForTfree(list->c.cdr);
        free(list);
    }
    else{
        free(list);
    }
}
            

/*
 * Free all pointers allocated by talloc, as well as whatever memory you
 * malloc'ed to create/update the active list.
 */
void tfree(){
    assert(head != NULL && head->type == CONS_TYPE
           || head->type == NULL_TYPE);
    helperForTfree(head);
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

