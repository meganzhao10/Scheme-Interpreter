#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
#include "linkedlist.h"

int main(void) {
   Value *val1 = malloc(sizeof(Value));
   val1->type = INT_TYPE;
   val1->i = 23;
    car(val1);
    
     Value *val11 = malloc(sizeof(Value));
   val11->type = DOUBLE_TYPE;
   val11->d = 23.09;
    car(val11);

   Value *val2 = malloc(sizeof(Value));
   val2->type = STR_TYPE;
   val2->s = malloc(10 * sizeof(char));
   strcpy(val2->s, "tofu");
    car(val2);
////   
//   Value *val3 = malloc(sizeof(Value));
//   val3->type = CONS_TYPE;
//   //val3->c = malloc(10 * sizeof(Value));
//    val3->c.car = 1;
//    //val3->c.cdr = 


   Value *head = makeNull();
   head = cons(val1, head);
   //head = cons(val2, head);

   //display(head);
   //printf("Length = %i\n", length(head));
   //printf("Empty? %i\n", isNull(head));
   //cleanup(head);

   return 0;
}
