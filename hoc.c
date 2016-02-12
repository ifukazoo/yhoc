#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <math.h>
#include "hoc.h"
#include "code.h"

#ifdef HOC_DEBUG
int yydebug = 1;
#else
int yydebug = 0;
#endif

static jmp_buf begin;

void reset_error(void)
{
  longjmp(begin, 0);
}
void yyerror(const char *fmt,  ...)
{
  va_list ap;
  va_start(ap,  fmt);
  fprintf(stderr,  "%d: error: ",  yylineno);
  vfprintf(stderr,  fmt,  ap);
  fprintf(stderr,  "\n");
  va_end(ap);
}
void* emalloc(size_t size)
{
  void* p = malloc(size);
  if ( ! p) {
    fputs("out of memory",  stderr);
    abort();
  }
  return p;
}
int main(void)
{
  init_code();

  setjmp(begin);

  int success = 1;
  while (success) {

    reset_code();
    if (! yyparse()) break;

    reset_code();
    success = run();
  }
  return 0;
}
