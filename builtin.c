#include <stdlib.h>
#include <math.h>
#include "builtin.h"
#include "symbol.h"
#include "code.h"
#include "y.tab.h"

static void install_const(void);
static void install_builtin(void);

double sin_(double x)   { return sin(x);   }
double cos_(double x)   { return cos(x);   }
double atan_(double x)  { return atan(x);  }
double log_(double x)   { return log(x);   }
double log10_(double x) { return log10(x); }
double exp_(double x)   { return exp(x);   }
double sqrt_(double x)  { return sqrt(x);  }
double abs_(double x)   { return fabs(x);  }
void init_builtin(void)
{
  install_const();
  install_builtin();
}
static void install_const(void)
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
  symbol_t* sym;
  for (i = 0; consts[i].name; i++) {
    install(consts[i].name, CONST);
    sym = lookup(consts[i].name);
    value_of(sym) = consts[i].val;
  }
}
static void install_builtin(void)
{
  int i;
  symbol_t* sym;

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
    func_of(sym) = builtins[i].func;
  }
}
