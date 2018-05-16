/*
 * This program implements the evaluator for Scheme.
 *
 * Authors: Yitong Chen, Yingying Wang, Megan Zhao.
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "linkedlist.h"
#include "interpreter.h"
#include <assert.h>
#include "talloc.h"

/*
 * Print a representation of the contents of a linked list.
 * 
 * Asserts that input is a list (Value of type CONS_TYPE or 
 * NULL_TYPE).
 */
void displayEval(Value *list){
    assert(list != NULL);
    Value *cur = list;
    while(cur != NULL){
	switch(cur->type){
            case INT_TYPE:
                printf("%i ",cur->i);
                break;
            case DOUBLE_TYPE:
                printf("%f ",cur->d);
                break;
            case STR_TYPE:
                printf("%s ",cur->s);
                break;
            case SYMBOL_TYPE:
                printf("%s ",cur->s);
            	break;
	        case CONS_TYPE:
                {if (car(cur)->type == CONS_TYPE) {
                    printf("(");
                    displayEval(car(cur));
                    printf(")");
                } else {
                    displayEval(car(cur));
                }
                }
                break;
            case NULL_TYPE:
                printf("()");
            break;
            default:
                printf(" ");
                break;     
        }
	if (cur->type == CONS_TYPE && cdr(cur)->type != NULL_TYPE){
	   cur = cdr(cur);
	} else{
	   cur = NULL;
	}
    }
}

/*
 * This function takes a list of S-expressions and call eval on 
 * each S-expression in the top-level environment and prints each
 * result 
 */
void interpret(Value *tree){
    Frame *topFrame = talloc(sizeof(Frame));
    if (!topFrame) {
        printf("Error! Not enough space! ");
        texit(1);
    }
    topFrame->bindings = makeNull();
    Value *cur = tree;
    while (cur != NULL && cur->type == CONS_TYPE){
	Value *result = eval(car(cur), topFrame);
	displayEval(result);
	printf("\n");
	cur = cdr(cur);
    }
}

/* 
 * Helper function for displaying evaluation err message.
 */
void evaluationError(){
    printf("Evaluation error!\n");
    texit(1);
}

Value *lookUpSymbol(Value *expr, Frame *frame){
    // Get binding of the frame in which the expression is evaluated
    Value *binding = frame->bindings;
    Frame *curF = frame;
    while (curF != NULL){
       assert(binding != NULL);      
       while (binding->type != NULL_TYPE){
           Value *curBinding = car(binding);
	   Value *name = car(curBinding);
	   Value *value = car(cdr(curBinding));
	   assert(name->type == SYMBOL_TYPE);
	   if (!strcmp(name->s, expr->s)){
		return value;	      
	   }
       	   binding = cdr(binding);
	}
       curF = curF->parent;
    }
    printf("The symbol %s is unbounded! ", expr->s);
    evaluationError();
    return NULL;
}

Value *evalIf(Value *args, Frame *frame){
    if (eval(car(args), frame)->type == BOOL_TYPE &&
	!strcmp(eval(car(args), frame)->s, "#f")){
	return eval(car(cdr(cdr(args))), frame);
    }
    return eval(car(cdr(args)), frame);
}

bool isBounded(Value *var, Frame *frame) {
    Value *binding = frame->bindings;
    while (binding->type != NULL_TYPE){
           Value *curBinding = car(binding);
	   Value *name = car(curBinding);
	   Value *value = car(cdr(curBinding));
	   assert(name->type == SYMBOL_TYPE);
	   if (!strcmp(name->s, var->s)){
		return true;	      
	   }
       	   binding = cdr(binding);
	}
    return false;
        
}
void addBindingLet(Value *var, Value *expr, Frame *frame){
    if (isBounded(var, frame)) {
        printf("Duplicate identifier in 'let'. ");
        evaluationError();
    }
    Value *list = cons(expr, makeNull());
    list = cons(var, list);
    Value *bindings = frame->bindings;
    frame->bindings = cons(list, bindings);
}

Value *evalLet(Value *args, Frame *frame){
    Value *cur = car(args);
    if (!isNull(cur) && cur->type != CONS_TYPE) {
        printf("Invalid syntax in 'let'. ");
        evaluationError();
    }
    if (isNull(cdr(args))) {
        printf("Empty body in 'let'. ");
        evaluationError();
    }
    Value *body = car(cdr(args));
    /*if (cur->type != CONS_TYPE || isNull(cdr(cdr(args)))){
	evaluationError();
    } */  
    Frame *frameG = talloc(sizeof(Frame));
    if (!frameG) {
        printf("Error! Not enough space! ");
        texit(1);
    }
    frameG->parent = frame;
    frameG->bindings = makeNull();
    while (cur != NULL && cur->type != NULL_TYPE){
        if (car(cur)->type != CONS_TYPE || length(car(cur)) != 2) {
            printf("Invalid syntax in 'let' bindings. ");
            evaluationError();
        }
	Value *v = eval(car(cdr(car(cur))), frame);
    if (car(car(cur))->type != SYMBOL_TYPE) {
        printf("Invalid syntax in 'let'. Not a valid identifier! ");
        evaluationError();
    }    
	addBindingLet(car(car(cur)), v, frameG);
	cur = cdr(cur);
    }
    return eval(body, frameG);    
}

/*
 * The function takes a parse tree of a single S-expression and 
 * an environment frame in which to evaluate the expression and 
 * returns a pointer to a Value representating the value.
 */
Value *eval(Value *expr, Frame *frame){
    switch (expr->type) {
	case INT_TYPE:
	    return expr;
	    break;
	case DOUBLE_TYPE:
	    return expr;
	    break;
	case STR_TYPE:
	    return expr;
	    break;
	case BOOL_TYPE:
	    return expr;
	    break;
	case SYMBOL_TYPE: 
	    return lookUpSymbol(expr, frame); 		
	    break;
	case CONS_TYPE: {
	    Value *first = car(expr);
	    Value *args = cdr(expr);
	    if (!strcmp(first->s, "if")){
    		if (length(args) != 3){
	            evaluationError();
    		}
		return evalIf(args, frame);
	    } 
	    else if (!strcmp(first->s, "quote")){
		return args;
	    } 
	    else if (!strcmp(first->s, "let")){ 
	    	return evalLet(args, frame);
	    }
	    else{
		// not a recognized special form
		evaluationError();
	    }		
	    break;
	}
	default:
	    evaluationError();
	    break;
    }
    return NULL;   
}


