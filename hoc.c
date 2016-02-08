#include <stdio.h>
#include <stdarg.h>
#include "hoc.h"
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
int main(void)
{
  return yyparse();
}
