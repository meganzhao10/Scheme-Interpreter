/* 
 * This program executes tokenization.
 *
 * Authors: Yitong Chen, Yingying Wang, Megan Zhao
 */

#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"
#include "talloc.h"

int main(void) {
    Value *list = tokenize(stdin);
    if (list == NULL) {
        texit(1);
    }
    displayTokens(list);
    texit(0);
}
