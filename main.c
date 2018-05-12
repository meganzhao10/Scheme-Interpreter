#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "tokenizer.h"
#include "parser.h"
#include "interpreter.h"

int main(void) {
    Value *list = tokenize();
    Value *tree = parse(list);
    interpret(tree);
    tfree();
    return 0;
}
