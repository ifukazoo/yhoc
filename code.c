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
int run(void)
{
  int success = 1;
  while (*pc != STOP && success) {
    success = (*pc++)();
    // 実行直後にpcをincrementしていることに注意．
    // pcは命令に制御が移った時点で次の位置にある.
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
inst_t* next_code()
{
  return pc;
}
int pushconst(void)
{
  /*
   * 次のinstructionに命令ではなく
   * 値を格納している前提
   */
  stack_t d;
  d.n = value_of((symbol_t*)(*pc));
  push(d);
  // 次のinstructionに移動
  pc++;

  return 1;
}
int pushvar(void)
{
  stack_t d;
  d.sym = (symbol_t*)(*pc);
  push(d);
  pc++;

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
int not(void)
{
  stack_t x = pop();
  stack_t d;
  d.n = ! x.n;
  push(d);

  return 1;
}
int gt(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = x.n > y.n;
  push(d);

  return 1;
}
int ge(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = x.n >= y.n;
  push(d);

  return 1;
}
int lt(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = x.n < y.n;
  push(d);

  return 1;
}
int le(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = x.n <= y.n;
  push(d);

  return 1;
}
int eq(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = x.n == y.n;
  push(d);

  return 1;
}
int ne(void)
{
  stack_t y = pop();
  stack_t x = pop();
  stack_t d;
  d.n = x.n != y.n;
  push(d);

  return 1;
}
int andleft(void)
{
  stack_t x = pop();
  stack_t d;
  d.n = x.n > 0;
  push(d);
  if (x.n) {
    pc++;
  } else {
    pc = (inst_t*)(*pc);
  }

  return 1;
}
int right(void)
{
  stack_t y = pop();
  stack_t d;
  d.n = y.n > 0;
  push(d);

  return 1;
}
int orleft(void)
{
  stack_t x = pop();
  stack_t d;
  d.n = x.n > 0;
  push(d);
  if (x.n) {
    pc = (inst_t*)(*pc);
  } else {
    pc++;
  }

  return 1;
}
int print(void)
{
  stack_t d = pop();
  printf("= %g\n", d.n);
  return 1;
}
int prexpr(void)
{
  stack_t d = pop();
  printf("%g\n", d.n);
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
int addassign(void)
{
  stack_t var = pop();
  stack_t value = pop();
  if ( ! (var.sym->type == VAR)) {
    yyerror("%s:not a variable.", var.sym->name);
    reset_error(); // longjmp
    /* not reach */
  }
  double newval = value_of(var.sym) + value.n;
  value_of(var.sym) = newval;
  push(var);

  return 1;
}
int subassign(void)
{
  stack_t var = pop();
  stack_t value = pop();
  if ( ! (var.sym->type == VAR)) {
    yyerror("%s:not a variable.", var.sym->name);
    reset_error(); // longjmp
    /* not reach */
  }
  double newval = value_of(var.sym) - value.n;
  value_of(var.sym) = newval;
  push(var);

  return 1;
}
int mulassign(void)
{
  stack_t var = pop();
  stack_t value = pop();
  if ( ! (var.sym->type == VAR)) {
    yyerror("%s:not a variable.", var.sym->name);
    reset_error(); // longjmp
    /* not reach */
  }
  double newval = value_of(var.sym) * value.n;
  value_of(var.sym) = newval;
  push(var);

  return 1;
}
int divassign(void)
{
  stack_t var = pop();
  stack_t value = pop();
  if ( ! (var.sym->type == VAR)) {
    yyerror("%s:not a variable.", var.sym->name);
    reset_error(); // longjmp
    /* not reach */
  }
  double newval = value_of(var.sym) / value.n;
  value_of(var.sym) = newval;
  push(var);

  return 1;
}
int modassign(void)
{
  stack_t var = pop();
  stack_t value = pop();
  if ( ! (var.sym->type == VAR)) {
    yyerror("%s:not a variable.", var.sym->name);
    reset_error(); // longjmp
    /* not reach */
  }
  double newval = fmod(value_of(var.sym), value.n);
  value_of(var.sym) = newval;
  push(var);

  return 1;
}
int powassign(void)
{
  stack_t var = pop();
  stack_t value = pop();
  if ( ! (var.sym->type == VAR)) {
    yyerror("%s:not a variable.", var.sym->name);
    reset_error(); // longjmp
    /* not reach */
  }
  double newval = pow(value_of(var.sym), value.n);
  value_of(var.sym) = newval;
  push(var);

  return 1;
}
int whilecode(void)
{
  /*
     while
     inst --------+     (whilecode + 1)
     inst [next]  |     (whilecode + 2)
     cond         |     (whilecode + 3)
     ...          |
     ...          |
     STOP         |
     ...          |
     body code  <-+
     ...
     ...
     STOP
     next
 */

  inst_t* base = pc - 1;

  for (;;) {
    // cond
    pc = base + 3;
    run();
    stack_t cond = pop();
    if (! cond.n)  break;

    pc = (inst_t*)*(base + 1);
    run();
  }

  pc = (inst_t*)*(base + 2);

  return 1;
}
int forcode(void)
{
  /*
     for
     inst      --+
     inst        | --+
     inst        |   | --+
     inst        |   |   | --+
     assign      |   |   |   |
     ...         |   |   |   |
     STOP        |   |   |   |
     cond      <-+   |   |   |
     ...             |   |   |
     STOP            |   |   |
     after     <-----+   |   |
     ...                 |   |
     STOP                |   |
     stmt      <---------+   |
     ...                     |
     STOP                    |
     next_code <-------------+
 */

  inst_t* base = pc - 1;

  // assign
  pc = base + 5;
  run();

  for (;;) {
    // cond
    pc = (inst_t*)*(base + 1);
    run();
    stack_t cond = pop();
    if (! cond.n)  break;

    // statement
    pc = (inst_t*)*(base + 3);
    run();

    // after
    pc = (inst_t*)*(base + 2);
    run();
  }

  // next
  pc = (inst_t*)*(base + 4);

  return 1;
}
int ifcode(void)
{
  /*
     ifcode
     inst --------+     (ifcode + 1)
     inst --------+--+  (ifcode + 2)
     inst [next]  |  |  (ifcode + 3)
     cond         |  |  (ifcode + 4)
     ...          |  |
     ...          |  |
     STOP         |  |
     ...          |  |
     then code  <-+  |
     ...             |
     ...             |
     STOP            |
     ...             |
     else code  <----+
     ...
     ...
     STOP
     next
 */

  inst_t* base = pc - 1;

  // cond
  pc = base + 4;
  run();

  stack_t cond = pop();
  if (cond.n) { // then branch
    pc = (inst_t*)*(base + 1);
    run();

  } else {      // else branch
    pc = (inst_t*)*(base + 2);
    if (pc)
      run();
  }

  pc = (inst_t*)*(base + 3);

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
