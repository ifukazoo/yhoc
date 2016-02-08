#ifndef _SYMBOL_H_
#define _SYMBOL_H_

typedef struct symbol {
  struct symbol* next;
  char* name;
  int   type;
  union {
    double (*func)(double);
    double val;
  } un;
} symbol_t;
#define func_of(s) (s)->un.func
#define value_of(s) (s)->un.val
#define call_func(s, ...) (*(s)->un.func)(__VA_ARGS__)

symbol_t* install(char* name, int type);
symbol_t* lookup(char* name);

#endif
