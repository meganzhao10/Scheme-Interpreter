#include <stdio.h>
#include "parser.h"
#include "tokenizer.h"

int main(void) {
    Value *list = tokenize();
    if (list == NULL) {
        texit(1);
    }
    Value *tree = parse(list);
    if (tree == NULL) {
        texit(1);
    }

    printTree(tree);
    printf("\n");
    texit(0);

}