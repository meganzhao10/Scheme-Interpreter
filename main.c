
#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "tokenizer.h"
#include "parser.h"
#include "interpreter.h"
#include <unistd.h>

int main(void) {
    bool terminal = isatty(fileno(stdin));
    // Create the global frame
    Frame *topFrame = talloc(sizeof(Frame));
    topFrame->bindings = makeNull();
    topFrame->parent = NULL;
    if (!topFrame) 
        printf("Error! Not enough memory!\n");
    do {
        Value *list = tokenize(stdin);
        if (list == NULL) {
            texit(1);
        }
        Value *tree = parse(list);
        if (tree == NULL) {
            texit(1);
        }
        interpret(tree, topFrame); 
     } while (terminal);
    tfree();
    return 0;
}