#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "value.h"
#include "talloc.h"
#include "linkedlist.h"

#define INT 23
#define DOUBLE 2.3
#define STR "tofu"


int main(void) {
    Value *val1 = talloc(sizeof(Value));
    if (!val1) {
        printf("Out of memory!\n");
        return 1;
    }
    val1->type = INT_TYPE;
    val1->i = INT;

    Value *val2 = talloc(sizeof(Value));
    if (!val2) {
        printf("Out of memory!\n");
        return 1;
    }
    val2->type = STR_TYPE;
    val2->s = talloc(10 * sizeof(char));
    if (!(val2->s)) {
        printf("Out of memory!\n");
        return 1;
    }
    strcpy(val2->s, STR);
    
    Value *val3 = talloc(sizeof(Value));
    if (!val3) {
        printf("Out of memory!\n");
        return 1;
    }
    val3->type = DOUBLE_TYPE;
    val3->d = DOUBLE;

    Value *nullHead = makeNull();
    if (!nullHead) {
        printf("Out of memory!\n");
        return 1;
    }

    // Test the values are stored properly
    assert(val1->i == INT);
    assert(!strcmp(val2->s, STR));
    assert(val3->d == DOUBLE);
    
    // Test that makeNull creates an empty list
    Value *head = makeNull();
    if (!head) {
        printf("Out of memory!\n");
        return 1;
    }
    assert(head->type == NULL_TYPE);
    assert(isNull(head));
    assert(length(head) == 0);
    
    // Test the result of cons value with empty list
    head = cons(val1, head);
    if (!head) {
        printf("Out of memory!\n");
        return 1;
    }
    assert(length(head) == 1);
    assert(head->type == CONS_TYPE);
    assert(car(head) == val1);
    assert(!isNull(head));
    assert(isNull(cdr(head)));
    display(head);
    printf("------------------------\n");
    
    // Test the result of cons value to non-emptylist
    head = cons(val2, head);
    if (!head) {
        printf("Out of memory!\n");
        return 1;
    }
    assert(length(head) == 2);
    assert(head->type == CONS_TYPE);
    assert(car(head) == val2);
    assert(car(cdr(head)) == val1);
    assert(isNull(cdr(cdr(head))));
    display(head);
    printf("------------------------\n");
    
    // Add another value
    head = cons(val3, head);
    if (!head) {
        printf("Out of memory!\n");
        return 1;
    }
    assert(length(head) == 3);
    display(head);
    printf("------------------------\n");
    
    // Test the reverse method
    Value *reversed = reverse(head);
    if (!reversed) {
        printf("Out of memory!\n");
        return 1;
    }
    assert(!isNull(reversed));
    assert(length(reversed) == 3);
    // reversed actually reverse the list
    assert(car(head)->d == DOUBLE);
    assert(!strcmp(car(cdr(head))->s, STR));
    assert(car(cdr(cdr(head)))->i == INT);
    // reversed should be a deep copy, so changing head should
    // not affect reversed
    car(head)->d = 3.2;
    strcpy(car(cdr(head))->s, "soup");
    printf("head: \n");
    display(head);
    printf("reversed: \n");
    display(reversed);
    
    // cleanup the original list and the reversed list
//    cleanup(head);
//    cleanup(nullHead);
//    cleanup(reversed);
    return 0;

}

