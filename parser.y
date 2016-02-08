%{
#include <math.h>
#include "symbol.h"
#include "code.h"
#include "hoc.h"

#define code2(a,b)   do { code((inst_t)a); code((inst_t)b); } while (0)
#define code3(a,b,c) do { code((inst_t)a); code((inst_t)b); code((inst_t)c); } while (0)
%}
%union {
  inst_t*  inst;
  symbol_t* sym;
}

%token NUMBER VAR BUILTIN CONST UNDEF EOS
%right '='
%left '+' '-'
%left '*' '/' '%'
%right UNARYMUNUS UNARYPLUS
%right '^' /* - 2 ^ 3 => - (2 ^ 3) */

%type <inst> expr assign
%type <sym> NUMBER VAR BUILTIN

%start list
%%
list     :
         | list EOS
         | list assign  EOS { code2(shift, STOP); return 1; }
         | list expr    EOS { code2(print, STOP); return 1; }
         | list error   EOS { yyerrok; }
         ;
assign   : VAR '=' expr { code3(pushvar, $1, assign); }
         ;
expr     : NUMBER        { code2(pushconst, $1); }
         | BUILTIN '(' expr ')' { code3(pushvar, $1, callbuiltin); }
         | assign        {}
         | VAR           { code3(pushvar, $1, eval); }
         | expr '+' expr { code(add); }
         | expr '-' expr { code(sub); }
         | expr '*' expr { code(mul); }
         | expr '/' expr { code(div_);}
         | expr '%' expr { code(mod); }
         | expr '^' expr { code(pow_);}
         | '(' expr ')'  {}
         | '-' expr %prec UNARYMUNUS { code(negate);}
         | '+' expr %prec UNARYPLUS {}
         ;
%%
