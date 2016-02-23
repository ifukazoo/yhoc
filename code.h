#ifndef _CODE_H_
#define _CODE_H_

#include <stdlib.h>
#include "symbol.h"

typedef void (*inst_t)(void);
#define STOP NULL

typedef union {
  symbol_t* sym;
  double    n;
} stack_t;

void init_code(void);
void run(void);
void reset_code(void);
inst_t* code(inst_t inst);
inst_t* next_code();
void pushconst(void);
void pushvar(void);
void add(void);
void sub(void);
void mul(void);
void div_(void);
void mod(void);
void pow_(void);
void negate(void);
void not(void);
void gt(void);
void ge(void);
void lt(void);
void le(void);
void eq(void);
void ne(void);
void andleft(void);
void orleft(void);
void right(void);
void print(void);
void prexpr(void);
void shift(void);
void eval(void);
void callbuiltin(void);
void assign(void);
void addassign(void);
void subassign(void);
void mulassign(void);
void divassign(void);
void modassign(void);
void powassign(void);
void whilecode(void);
void forcode(void);
void ifcode(void);

#endif
