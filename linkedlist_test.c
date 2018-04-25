#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
#include "linkedlist.h"

int main(void) {
   Value *val1 = malloc(sizeof(Value));
   val1->type = INT_TYPE;
   val1->i = 23;

   Value *val2 = malloc(sizeof(Value));
   val2->type = STR_TYPE;
   val2->s = malloc(10 * sizeof(char));
   strcpy(val2->s, "tofu");
  
    printf("val1 value: %i\n", val1->i);
    printf("val1 address: %p\n", val1);
    printf("val2 value: %s\n", val2->s);

    printf("------------------------------\n");
    
   Value *head = makeNull();
    printf("type head: %i\n", head->type);
    printf("head address: %p\n", head);
   head = cons(val1, head);
    printf("type head after first cons: %i\n", head->type);
    printf("head address: %p\n", head);
    printf("&head: %p\n", &head);
    printf("head car type: %i\n", head->c.car->type);
    printf("head car value: %i\n", head->c.car->i);
    printf("head cdr type: %i\n", head->c.cdr->type);
    
    printf("------------------------------\n");
   head = cons(val2, head);
    printf("type head after first cons: %i\n", head->type);
    printf("head address: %p\n", head);
    printf("&head: %p\n", &head);
    printf("head car type: %i\n", head->c.car->type);
    printf("head car value: %s\n", head->c.car->s);
    printf("head cdr type: %i\n", head->c.cdr->type);
    printf("head cadr type: %i\n", head->c.cdr->c.car->type);
    printf("head cadr value: %i\n", head->c.cdr->c.car->i);
    printf("head cddr type: %i\n", head->c.cdr->c.cdr->type);
    
    printf("------------------------------\n");

    printf("display whole list: ");
    printf("head address: %p\n", head);
    printf("&head: %p\n", &head);
//    
    printf("display car: \n");
////    printf("head car type: %s\n", head->c.car->s);
    printf("type is: %i\n", car(head)->type);
    printf("value is: %s\n", car(head)->s);
    display(car(head));
////
////
    
        
    printf("------------------------------\n");
   Value *reversed;
   reversed = reverse(head);
    printf("reversed type: %i\n", reversed->type);
    printf("reverse car type: %i\n", reversed->c.car->type);
    display(head);
    display(reversed);
//    printf("&reversed: %p\n", &reversed);
//    printf("head cadr type: %i\n", head->c.cdr->c.car->type);
//    printf("head cadr value: %i\n", head->c.cdr->c.car->i);
//    printf("&head car: %p\n", &(head->c.car));
//    printf("reversed car type: %i\n", reversed->c.car->type);
//    printf("reversed car value: %i\n", reversed->c.car->i);
//    printf("&reversed car: %p\n", &(reversed->c.car));
//    printf("reversed cdr type: %i\n", reversed->c.cdr->type);
//    printf("reversed cadr type: %i\n", reversed->c.cdr->c.car->type);
//    printf("reversed cadr value: %s\n", reversed->c.cdr->c.car->s);
//    printf("reversed cddr type: %i\n", reversed->c.cdr->c.cdr->type);
//////   printf("Length = %i\n", length(head));
////   printf("Empty? %i\n", isNull(head));

//
//    Value *val3 = malloc(sizeof(Value));
//    val3->type = INT_TYPE;
//    val3->i = 33332;
//    
//    head->c.car = val3;
//    display(head);
//    display(reversed);
    
    head->c.car->s = "string";
    display(head);
    display(reversed);
    cleanup(head);
    cleanup(reverse);
    return 0;
}


