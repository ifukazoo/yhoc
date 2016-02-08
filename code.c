#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "symbol.h"
#include "builtin.h"
#include "hoc.h"
#include "code.h"
#include "y.tab.h"

enum { NSTACK = 256, NINST  = 256 };
static inst_t    instruction[NINST];
static stack_t   stack[NSTACK];
static inst_t*   pc;
static stack_t*  sp;

static stack_t pop(void);
static void push(stack_t d);

void init_code(void)
{
  init_builtin();
}
int run_code(void)
{
  int success = 1;
  pc = &instruction[0];
  while (*pc != STOP && success) {
    success = (*pc)();
    pc++;
  }
  return success;
}
void reset_code(void)
{
  pc = &instruction[0];
  sp = &stack[0];
}
inst_t* code(inst_t inst)
{
  inst_t* cur = pc;
  *pc++ = inst;
  return cur;
}
int pushconst(void)
{
  /*
   * 次の命令に値を格納している前提
   */
  stack_t d;
  d.n = value_of((symbol_t*)(*++pc));
  push(d);

  return 1;
}
int pushvar(void)
{
  /*
   * 次の命令に値を格納している前提
   */
  stack_t d;
  d.sym = (symbol_t*)(*++pc);
  push(d);

  return 1;
}
int eval(void)
{
  stack_t x = pop();
  if (x.sym->type == UNDEF) {
    yyerror("undefined var:%s", x.sym->name);
    reset_error(); // longjmp
    /* not reach */
  }
  double n = value_of(x.sym);
  stack_t d;
  d.n = n;
  push(d);

  return 1;
}
int callbuiltin(void)
{
  stack_t func = pop();
  stack_t arg = pop();
  stack_t d;
  d.n = call_func(func.sym, arg.n);
  push(d);

  return 1;
}
int add(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = x.n + y.n;
  push(d);

  return 1;
}
int sub(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = x.n - y.n;
  push(d);

  return 1;
}
int mul(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = x.n * y.n;
  push(d);

  return 1;
}
int div_(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = x.n / y.n;
  push(d);

  return 1;
}
int mod(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = fmod(x.n, y.n);
  push(d);

  return 1;
}
int pow_(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = pow(x.n, y.n);
  push(d);

  return 1;
}
int negate(void)
{
  stack_t x = pop();
  stack_t d;
  d.n = -x.n;
  push(d);

  return 1;
}
int print(void)
{
  stack_t d = pop();
  printf("= %g\n", d.n);

  return 1;
}
int shift(void)
{
  pop();
  return 1;
}
int assign(void)
{
  stack_t var = pop();
  stack_t value = pop();
  if ( ! (var.sym->type == VAR || var.sym->type == UNDEF)) {
    yyerror("assignment to non variable:%s", var.sym->name);
    reset_error(); // longjmp
    /* not reach */
  }
  value_of(var.sym) = value.n;
  var.sym->type = VAR;
  push(var);

  return 1;
}
static stack_t pop(void)
{
  if (sp <= stack) {
    yyerror("stack underflow");
    abort();
  }
  stack_t d = *(--sp);
  return d;
}
static void push(stack_t d)
{
  if (sp >= stack + NSTACK) {
    yyerror("stack overflow");
    abort();
  }
  *sp++ = d;
  return;
}
