#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "hoc.h"
#include "symbol.h"
#include "builtin.h"
#include "y.tab.h"

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
void init(void)
{
  static struct {
    char* name;
    double val;
  } consts[] = {
    { "PI",     3.14159265358979323846 },
    { "E",      2.71828182845904523536 },
    { "GAMMA",  0.57721566490153286050 },
    { "DEG",   57.29577951308232087680 },
    { "PHI",    1.61803398874989484820 },
    { NULL,     0                      }, // sentinel
  };
  int i;
  struct symbol* sym;
  for (i = 0; consts[i].name; i++) {
    install(consts[i].name, VAR);
    sym = lookup(consts[i].name);
    sym->un.val = consts[i].val;
  }

  static struct {
    char* name;
    double (*func)(double);
  } builtins[] = {
    {"sin",   sin_},
    {"cos",   cos_},
    {"atan",  atan_},
    {"log",   log_},
    {"log10", log10_},
    {"exp",   exp_},
    {"sqrt",  sqrt_},
    {"abs",   abs_},
    { NULL,   NULL}, // sentinel
  };
  for (i = 0; builtins[i].name; i++) {
    install(builtins[i].name, BUILTIN);
    sym = lookup(builtins[i].name);
    sym->un.func = builtins[i].func;
  }
}

int main(void)
{
  init();
  return yyparse();
}
