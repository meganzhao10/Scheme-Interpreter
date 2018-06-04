/* This code implements a garbage collector to manage memory
 * usage throughout the interpreter project. 
 * 
 * Authors: Yitong Chen, Yingying Wang, Megan Zhao
 */

#include <assert.h>
#include <stdio.h>
#include "talloc.h"
#include "linkedlist.h"

// The global static variable
static Value *head;
//static Value *usefulListHead;
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
void *talloc(size_t size){
	if (head == NULL){
		head = talloc_makeNull();
		if (!head){
			printf("Out of memory!\n");
			return head;
		}
	}
	Value *newHead = malloc(sizeof(Value));
	if (!newHead){
		printf("Out of memory!\n");
		return newHead;
	}
	newHead->type = PTR_TYPE;
    newHead->useful = false;
	newHead->p = malloc(size);
//    newHead->useful = false;
	if (!newHead->p){
		printf("Out of memory!\n");
        return newHead->p;
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
	while (cur->type != NULL_TYPE){
		next = cur->c.cdr;
		free(cur->c.car->p);
		free(cur->c.car);
		free(cur);
		cur = next;
	} 
	free(next);
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


// /*
//  * Helper function to check whether a pointer is useful in the given
//  * frame.
//  */
// bool useful(Value *pointer, Frame *frame) {
//     return true;
// }

// /* 
//  * Start with objects bound in the top/global environment, mark them 
//  * as useful. Recursively mark everything pointed by them as useful
//  * as well.
//  */
// void mark(Frame *global){
// //    usefulListHead = talloc_makeNull();
// //    usefulListHead->useful = true;
//     global->useful = true;
//     markValue(global->bindings);
    
// //    usefulListHead->useful = true;
// //    // Add the usefulListHead to usefulList
// //    Value *wrapper = malloc(sizeof(Value));
// //    wrapper->p = global;
// //    usefulListHead = cons(wrapper, usefulListHead);
// //    printf("%p\n", global);
// //    printf("%p\n", &global);
// ////    
//     Value *cur = head;
//     while (cur->type != NULL_TYPE) {
//         if (car(cur)->p == global) {
// //            car(cur)->useful = true;
// //            car(cur)->useful = true;
//             Value *carVal = car(cur)->p;
//             Frame *carFrame = car(cur)->p;
//             if (carVal->useful || carFrame->useful) {
//                 printf("made it\n");
//             }
//             printf("trye\n");
//         }
//         cur = cdr(cur);
//     }
// //    Value *bindings = global->bindings;
// //    
// //    Value *cur = head;
// //    while (cur->type != NULL_TYPE) {
// //        if (useful(car(cur)->p, global)) {
// //            car(cur)->useful = true;
// //        } else {
// //            car(cur)->useful = false;
// //        }
// //        cur = cdr(cur);
// //    }
// }

// /* 
//  * Sweep useless objects by: i) free the memory ii) remove the pointer
//  * from the active list of all allocated pointers so that we don't 
//  * double free.
//  */
// void sweep() {
// //    Value *cur = head;
// //    
// //    while (cur->type != NULL_TYPE) {
// ////        Value *my = car(cur)->p;
// ////        printf("type of %i\n", my->type);
// //        if (!car(cur->useful)) {
// //            // Free memmory, remove from list
// //        }
// //        cur = cdr(cur);
// //    }
// //    free(usefulListHead);
//     printf("Huh?");
// //    free(car(usefulListHead));
// //    free(usefulListHead);
// }
