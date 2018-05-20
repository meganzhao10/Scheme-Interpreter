#ifndef VALUE_H
#define VALUE_H

typedef enum {
   PTR_TYPE,
   OPEN_TYPE,
   CLOSE_TYPE,
   BOOL_TYPE,
   SYMBOL_TYPE,
   INT_TYPE,
   DOUBLE_TYPE,
   STR_TYPE,
   CONS_TYPE,
   NULL_TYPE,
   VOID_TYPE,
   CLOSURE_TYPE
} valueType;

struct Value {
   valueType type;
   union {
      void *p;
      int i;
      double d;
      char *s;
      struct ConsCell {
         struct Value *car;
         struct Value *cdr;
      } c;
   };
};

typedef struct Value Value;

#endif
