# A interpreter for Scheme (R5RS) written in C that uses a linked list as the only data structure

## How To Run
```
# Clone this repository
$ git clone https://github.com/meganzhao/Scheme-Interpreter.git

# Go into the repository
$ cd Scheme-Interpreter

# Make file
$ make interpreter
```
You can either run the interface or Scheme files:
### Interactive interface
```
$ ./interpreter
# Type in Scheme command, for example:
> (+ 1 2)
3
```
![Alt text](img-demo/img1.png?raw=true "Title")
### Run Scheme test files
```
# To run test file test.eval.input.01
$ ./interpreter < test.eval.input.01
```

## Phases of this project
### Phase 1: Linked List
To implement a linked list that will be used throughout the interpreter project.
##### Main files at this phase: 
linkedlist.c linkedlist.h

### Phase 2: Talloc("tracked malloc")
To create a garbage collector to manage memory usage throughout the interpreter project.
##### Main files at this phase: 
talloc.c talloc.h
### Phase 3: Tokenizer
To implement a tokenizer for Scheme in C.
##### Main files at this phase: 
tokenizer.c tockenizer.h
### Phase 4: Parser
To parse the tokens into a syntax tree.
##### Main files at this phase: 
parser.c parser.h

### Phase 5: Eval
To evaluate the Scheme code. Specifically allow the evaluation of
bounded variables, if, let and quote special forms.
##### Main files at this phase:
interpreter.h interpreter.c

### Phase 6: Apply
Extend the evaluator's ability to handle define and lambda special forms.
##### Main files at this phase:
interpreter.h interpreter.c

### Phase 7: Primitives
To support applying Scheme primitive functions implemented in C; to implement a few primitive functions.
##### Main files at this phase:
interpreter.h interpreter.c

### Phase 8: Interpreter
To add a myriad of features to the interpreter including:
##### Main files at this phase:
interpreter.h interpreter.c

##### Primitive procedures:
*, -, /, <=, eq?, pair?, and apply.
###### Main file:
interpreter.c

##### Library procedures:
=, modulo, zero?, equal?, list, and append.
###### Main file:
lists.scm (include list and append)
math.scm (include =, modulo, zero?, and equal?)
##### Special forms: 
lambda, let*, letrec, and, or, cond, set!, and begin.
###### Main file:
interpreter.c


##### Extensions 
###### Main file:
N/A
###### A simple interface
The classic core of an interpreter is the read–eval–print loop, a.k.a. REPL. Adding this functionality to our code allows for interactive usage.
###### Main file:
tokenizer.c main.c

##### ' as the shorthand for quote special form NOT YET!!!!!
For example, '(2 2 8) as the shorthand for (quote (2 2 8))
Currently doesn't work for nested lists
###### Main file:
tokenizer.c 

##### The function load
The expression (load "tofu.scm") reads in the file and excutes the Scheme code within as if it were typed directly as part of the input.
###### Main file:
interpreter.c

##### More built-in functions to manipulate lists.
In a file called lists.scm, implement the following functions (refer to R5RS, Dybvig, or Racket reference for specification) using only special forms and primitives that you've implemented (e.g., car, cdr, cons, null?, pair?, and apply).
###### Main file:
lists.scm

##### More built-in functions with regards to arithmetic.
To implement the following functions using only special forms and primitives that we've implemented (e.g., +, -, *, /, and <=) in a file called math.scm.
###### Main file:
math.scm





## Author: 
* **Yitong Chen** - [yitongc19](https://github.com/yitongc19)
* **Yingying Wang** - [yingyingww](https://github.com/yingyingww)
* **Megan Zhao** - [meganzhao](https://github.com/meganzhao)
