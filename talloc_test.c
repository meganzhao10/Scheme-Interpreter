#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "value.h"
#include "talloc.h"
#include "linkedlist.h"

#define INT 23
#define DOUBLE 2.3
#define STR "tofu"

int main(void) {
    Value *val1 = malloc(sizeof(Value));
    if (!val1) {
        printf("Out of memory!\n");
        return 1;
    }
    val1->type = INT_TYPE;
    val1->i = INT;

    Value *val2 = malloc(sizeof(Value));
    if (!val2) {
        printf("Out of memory!\n");
        return 1;
    }
    val2->type = STR_TYPE;
    val2->s = malloc(10 * sizeof(char));
    if (!(val2->s)) {
        printf("Out of memory!\n");
        return 1;
    }
    strcpy(val2->s, STR);
    
    Value *val3 = malloc(sizeof(Value));
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
    
    // Switching to talloc should not affect the behavior 
    // of the methods in Linkedlist
    Value *head = makeNull();
    if (!head) {
        printf("Out of memory!\n");
        return 1;
    }
    head = cons(val1, head);
    if (!head) {
        printf("Out of memory!\n");
        return 1;
    }
    head = cons(val2, head);
    if (!head) {
        printf("Out of memory!\n");
        return 1;
    }
    head = cons(val3, head);
    if (!head) {
        printf("Out of memory!\n");
        return 1;
    }
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
    // reversed should be a shallow copy, so changing head should
    // affect reversed
    car(head)->d = 3.2;
    strcpy(car(cdr(head))->s, "soup");
    printf("head: \n");
    display(head);
    printf("reversed: \n");
    display(reversed);
    
    // cleanup the original list and the reversed list
    texit(0);
    return 0;
}
