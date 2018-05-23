#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "tokenizer.h"
#include "parser.h"
#include "interpreter.h"

int main(void) {
    Value *list = tokenize();
    if (list == NULL) {
        texit(1);
    }
    Value *tree = parse(list);
    if (tree == NULL) {
        texit(1);
    }
    interpret(tree);
    tfree();
    return 0;
}
