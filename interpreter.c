/*
 * This program implements the evaluator for Scheme.
 *
 * Authors: Yitong Chen, Yingying Wang, Megan Zhao.
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "parser.h"
#include "linkedlist.h"
#include "interpreter.h"
#include <assert.h>
#include "talloc.h"

/*
 * Print a representation of the contents of a linked list.
 */
void displayEval(Value *list, bool newline){
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
	    case BOOL_TYPE:
		printf("%s ",cur->s);
		break;
            case SYMBOL_TYPE:
                printf("%s ",cur->s);
            	break;
            case BOOL_TYPE:
                printf("%s ", cur->s);
                break;
	        case CONS_TYPE:
                {if (car(cur)->type == CONS_TYPE) {
                    printf("(");
                    displayEval(car(cur), false);
                    printf(")");
                } else {
                    displayEval(car(cur), false);
                }
                }
                break;
            case NULL_TYPE:
                printf("()");
                break;
            case VOID_TYPE: 
                newline = false;
                break;
            case CLOSURE_TYPE:
                printf("#procedure ");
                break;
            default:
                printf(" ");
                break;     
        }
        if (newline) {
            printf("\n");
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
        printf("Error! Not enough memory!\n");
        texit(1);
    }
    topFrame->bindings = makeNull();
    topFrame->parent = NULL;
    Value *cur = tree;
    while (cur != NULL && cur->type == CONS_TYPE){
    	Value *result = eval(car(cur), topFrame);
    	displayEval(result, true);
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

/*
 * Helper function to lookup symbols in the given environment.
 */
Value *lookUpSymbol(Value *expr, Frame *frame){
    // Get binding of the frame in which the expression is evaluated
    Frame *curF = frame;
    Value *binding = curF->bindings;
    while (curF != NULL){
       binding = curF->bindings;
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

/*
 * Helper function to evaluate the IF special form.
 */
Value *evalIf(Value *args, Frame *frame){
    if (eval(car(args), frame)->type == BOOL_TYPE &&
	!strcmp(eval(car(args), frame)->s, "#f")){
	if (cdr(cdr(args))->type != NULL_TYPE){
            return eval(car(cdr(cdr(args))), frame);
        } else{
            Value *emptyValue = talloc(sizeof(Value));
            if (!emptyValue){
                printf("Error! Not enough memory!\n");
            }
            emptyValue->type = VOID_TYPE;
            return emptyValue;
        }
    }
    return eval(car(cdr(args)), frame);
}

/* 
 * Helper function to check whether a variable is already bounded in
 * the current frame.
 *
 * Returns current binding if the variable is already bounded;
 *         null if the variable is not bounded.
 */
Value *isBounded(Value *var, Frame *frame) {
    Value *binding = frame->bindings;
    while (binding->type != NULL_TYPE){
           Value *curBinding = car(binding);
	   Value *name = car(curBinding);
	   Value *value = car(cdr(curBinding));
	   assert(name->type == SYMBOL_TYPE);
	   if (!strcmp(name->s, var->s)){
            return curBinding;	      
	   }
       binding = cdr(binding);
	}
    return NULL;
}

/* 
 * Helper function to create new let bindings.
 */
void addBindingLocal(Value *var, Value *expr, Frame *frame){
    if (isBounded(var, frame)) {
        printf("Duplicate identifier in local binding. ");
        evaluationError();
    }
    Value *nullTail = makeNull();
    if (!nullTail) {
        texit(1);
    }
    Value *list = cons(expr, nullTail);
    list = cons(var, list);
    Value *bindings = frame->bindings;
    frame->bindings = cons(list, bindings);
}

/* 
 * Helper function to create new define bindings.
 */
void addBindingDefine(Value *var, Value *expr, Frame *frame){
    Value *curBinding = isBounded(var, frame);
    Value *nullTail = makeNull();
    if (!nullTail) {
        texit(1);
    }
    // Modify existing binding
    if (curBinding) {
        curBinding->c.cdr = cons(expr, nullTail);
    } 
    // Create new binding
    else { 
        Value *list = cons(expr, nullTail);
        list = cons(var, list);
        Value *bindings = frame->bindings;
        frame->bindings = cons(list, bindings);
    }
}

/*
 * Helper function to evaluate the LET special form by 
 * creating bindings and then evaluate the body.
 */
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
    Value *body = cdr(args);
    Frame *frameG = talloc(sizeof(Frame));
    if (!frameG) {
        printf("Error! Not enough memory!\n");
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
	    addBindingLocal(car(car(cur)), v, frameG);
	    cur = cdr(cur);
    }
    // Evaluate all expressions but 
    // only return the last expression in body
    while (cdr(body)->type != NULL_TYPE){
        eval(car(body), frameG);
        body = cdr(body);
    }
    body = car(body);
    return eval(body, frameG);    
}

/*
 * Helper function to evaluate the DEFINE special form by 
 * creating bindings and then evaluate the body.
 */
Value *evalDefine(Value *args, Frame *frame){
    if (length(args) != 2){
	printf("Invalid syntax in 'define' bindings. ");
	evaluationError();	
    }
    // Find the global/top frame
    Frame *curFrame = frame;
    while (curFrame->parent != NULL){
	curFrame = curFrame->parent;
    } 
    addBinding(car(args), eval(car(cdr(args)), frame), curFrame);  
    Value *nullValue = makeNull();
    nullValue->type = VOID_TYPE;
    return nullValue; 
}

Value *createProcedure(Value *args, Frame *frame){
    Value *procedure = makeNull();
    procedure->type = CLOSURE_TYPE;
    procedure->o.parameters = car(args);
    // now only support one expr in lambda
    procedure->o.body = car(cdr(args));
    procedure->o.frame = frame;
    return procedure;
}

/*
 * Helper function that applies a function to a given set of 
 * arguments.
 *
 * Right now only supports applying closure type functions.
 */
Value *apply(Value *function, Value *args) {
    if (function->type != CLOSURE_TYPE) {
        printf("Expected the first argument to be a procedure! ");
        evaluationError();
    }
    Value *formal = function->closure.formal;
    Value *body = function->closure.body;
    Frame *parentFrame = function->closure.frame;
    if (length(formal) != length(args)) {
        printf("Expected %i arguments, supplied %i. ", 
               length(formal), length(args));
        evaluationError();
    }
    Frame *newFrame = talloc(sizeof(Frame));
    if (!newFrame) {
        printf("Error! Not enough memory!\n");
        texit(1);
    }
    newFrame->parent = parentFrame;
    newFrame->bindings = makeNull();
    Value *curFormal = formal;
    Value *curActual = args;
    while (curFormal->type != NULL_TYPE) {
        addBindingLocal(car(curFormal), car(curActual), newFrame);
        curFormal = cdr(curFormal);
        curActual = cdr(curActual);
    }
    // Evaluate multiple expressions
    while (cdr(body)->type != NULL_TYPE){
        eval(car(body), newFrame);
        body = cdr(body);
    }
    return eval(car(body), newFrame);
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
    		if (length(args) != 3 && length(args) != 2){
	            printf("Number of arguments for 'if' has to be 2 or 3. ");
                    evaluationError();
    		}
    		return evalIf(args, frame);
	    } 
	    else if (!strcmp(first->s, "quote")){
    		if (length(args) != 1){
                    printf("Number of arguments for 'quote' has to be 1. "); 
                    evaluationError();
                }
     	        return args;
	    } 
	    else if (!strcmp(first->s, "let")){ 
	    	return evalLet(args, frame);
	    }
        else if (!strcmp(first->s, "define")) {
            if (frame->parent != NULL) {
                printf("'define' expressions only allowed"
                       " in the global environment. ");
                evaluationError();
            }
            return evalDefine(args, frame);
        }
        else if (!strcmp(first->s, "lambda")) {
            if (length(args) < 2) {
                printf("There has to be at least 2 arguments for 'lambda'. ");
                evaluationError();
            }
            Value *closure = talloc(sizeof(Value));
            if (!closure) {
                printf("Error! Not enough memory!\n");
                texit(1);
            }
            closure->type = CLOSURE_TYPE;
            closure->closure.formal = car(args);
            closure->closure.body = cdr(args);
            closure->closure.frame = frame;
            return closure;
        }
	    else{
            // Stores the result of recursively evaluating e1...en
            Value *values = makeNull();
            if (!values) {
                texit(1);
            }
            Value *cur = expr;
            while (cur->type != NULL_TYPE) {
                Value *cur_value = eval(car(cur), frame);
                values = cons(cur_value, values);
                cur = cdr(cur);
            }
            values = reverse(values);
            Value *function = car(values);
            Value *actual = cdr(values);
            return apply(function, actual);
	    }		
	    break;
	}
	default:
	    evaluationError();
	    break;
    }
    return NULL;   
}



