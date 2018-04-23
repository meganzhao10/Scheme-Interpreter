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

   Value *head = makeNull();
   head = cons(val1, head);
   head = cons(val2, head);

   display(head);
   printf("Length = %i\n", length(head));
   printf("Empty? %i\n", isNull(head));
   cleanup(head);

   return 0;
}
