 /* 
  * Authors: Yitong Chen, Yingying Wang, Megan Zhao
  */

#include <stdlib.h>
#include "tokenizer.h"

int main(void) {
        Value *list = tokenize();
        displayTokens(list);
        tfree();
        return 0;
}
