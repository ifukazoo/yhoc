#ifndef _SYMBOL_H_
#define _SYMBOL_H_

struct symbol {
  struct symbol* next;
  char* name;
  int type;
  union {
    double (*func)(double);
    double val;
  } un;
};

struct symbol* install(char* name, int type);
struct symbol* lookup(char* name);

#endif
