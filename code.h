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
int run_code(void);
void reset_code(void);
inst_t* code(inst_t inst);
int pushconst(void);
int pushvar(void);
int add(void);
int sub(void);
int mul(void);
int div_(void);
int mod(void);
int pow_(void);
int negate(void);
int print(void);
int shift(void);
int eval(void);
int callbuiltin(void);
int assign(void);

#endif
