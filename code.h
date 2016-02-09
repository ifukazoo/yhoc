#ifndef _CODE_H_
#define _CODE_H_

#include <stdlib.h>
#include "symbol.h"

typedef int (*inst_t)(void);
#define STOP NULL

typedef union {
  symbol_t* sym;
  double    n;
} stack_t;

void init_code(void);
int run(void);
void reset_code(void);
inst_t* code(inst_t inst);
inst_t* next_code();
int pushconst(void);
int pushvar(void);
int add(void);
int sub(void);
int mul(void);
int div_(void);
int mod(void);
int pow_(void);
int negate(void);
int not(void);
int gt(void);
int ge(void);
int lt(void);
int le(void);
int eq(void);
int ne(void);
int and(void);
int or(void);
int print(void);
int prexpr(void);
int shift(void);
int eval(void);
int callbuiltin(void);
int assign(void);
int addassign(void);
int subassign(void);
int mulassign(void);
int divassign(void);
int modassign(void);
int powassign(void);
int whilecode(void);
int ifcode(void);

#endif
