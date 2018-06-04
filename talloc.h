#include <stdlib.h>
#include "value.h"

#ifndef TALLOC_H
#define TALLOC_H

/*
 * A malloc-like function that allocates memory, tracking all allocated
 * pointers in the "active list."  (You can choose your implementation of the
 * active list, but whatever it is, your talloc code should NOT call functions
 * in linkedlist.h; instead, implement any list-like behavior directly here.
 * Otherwise you'll end up with circular dependencies, since you're going to
 * modify the linked list to use talloc instead of malloc.)
 */
void *talloc(size_t size);

/*
 * Free all pointers allocated by talloc, as well as whatever memory you
 * malloc'ed to create/update the active list.
 */
void tfree();

/*
 * A simple two-line function to stand in the C function "exit", which calls
 * tfree() and then exit().  (You'll use this later to allow a clean exit from
 * your interpreter when you encounter an error: so memory can be automatically
 * cleaned up when exiting.)
 */
void texit(int status);

/* 
 * Start with objects bound in the top/global environment, mark them 
 * as useful. Recursively mark everything pointed by them as useful
 * as well.
 */
void mark(Frame *global);

/* 
 * Sweep useless objects by: i) free the memory ii) remove the pointer
 * from the active list of all allocated pointers so that we don't 
 * double free.
 */
void sweep();

#endif
