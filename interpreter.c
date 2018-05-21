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
	    case BOOL_TYPE:
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
      	    case VOID_TYPE: 
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
                printf("Error: not enough memory");
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
 */
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

/* 
 * Helper function to create new bindings.
 */
void addBinding(Value *var, Value *expr, Frame *frame){
   if (isBounded(var, frame)) {
        printf("Duplicate identifier in binding. ");
        evaluationError();
    }
    Value *list = cons(expr, makeNull());
    list = cons(var, list);
    Value *bindings = frame->bindings;
    frame->bindings = cons(list, bindings);
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
    Value *body = args;
    while (cdr(body)->type != NULL_TYPE){
        body = cdr(body);
    }
    body = car(body);
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
	    addBinding(car(car(cur)), v, frameG);
	    cur = cdr(cur);
    }
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

Value *apply(Value *function, Value *args){
   /* if (function->type != CLOSURE_TYPE){
        printf("Unrecognized forms! ");
	evaluationError();
    }*/
    if (length(function->o.parameters) != length(args)){
	printf("The number of arguments does not match the expected"
		"number of arguments for the function. ");
	evaluationError();
    }
    Frame *applyFrame = talloc(sizeof(Frame));
    if (!applyFrame) {
        printf("Error! Not enough space! ");
        texit(1);
    }
    applyFrame->parent = function->o.frame;
    applyFrame->bindings = makeNull(); 
    Value *applyArgs = args;
    Value *applyPara = function->o.parameters;
    while (applyArgs != NULL && applyArgs->type != NULL_TYPE){
	// frame of eval?
	addBinding(car(applyPara), eval(car(applyArgs), applyFrame), applyFrame);
	applyArgs = cdr(applyArgs);			
	applyPara = cdr(applyPara);		
    }	
    return eval(function->o.body, applyFrame);
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
	    else if (!strcmp(first->s, "define")){
		// do we want "\n" when define?
		return evalDefine(args, frame);	
	    }
	    else if (!strcmp(first->s, "lambda")){
	    	// should (lambda (x) 3) display anything?
		return createProcedure(args, frame);
	    }
	    else{
		Value *cur = expr;
		Value *evalList = makeNull();
		while (cur != NULL && cur->type != NULL_TYPE){
		    assert(cur->type == CONS_TYPE);
		    evalList = cons(eval(car(cur), frame), evalList);
		    cur = cdr(cur);
		}	
		evalList = reverse(evalList);
		return apply(car(evalList), cdr(evalList));
		// not a recognized special form
                // printf("Unrecognized forms! ");
		// evaluationError();
	    }		
	    break;
	}
	default:
	    evaluationError();
	    break;
    }
    return NULL;   
}


