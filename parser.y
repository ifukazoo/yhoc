%{
#include <math.h>
#include "symbol.h"
#include "hoc.h"

%}
%union {
  double val;
  struct symbol* sym;
}

%token NUMBER VAR BUILTIN EOS
%right '='
%left '+' '-'
%left '*' '/' '%'
%right UNARYMUNUS UNARYPLUS
%right '^' /* - 2 ^ 3 => - (2 ^ 3) */

%type <val> NUMBER expr assign
%type <sym> VAR BUILTIN

%start list
%%
list     :
         | list EOS
         | list assign  EOS {}
         | list expr    EOS { printf("\t%.8g\n", $2); }
         | list error   EOS { yyerrok; }
         ;
assign   : VAR  '=' expr { $1->un.val = $3; $$ = $3; }
         ;
expr     : NUMBER        { $$ = $1; }
         | BUILTIN '(' expr ')' { $$ = (*($1->un.func))($3); }
         | assign        { $$ = $1; }
         | VAR           { $$ = $1->un.val; }
         | expr '+' expr { $$ = $1 + $3; }
         | expr '-' expr { $$ = $1 - $3; }
         | expr '*' expr { $$ = $1 * $3; }
         | expr '/' expr { $$ = $1 / $3; }
         | expr '%' expr { $$ = fmod($1, $3); }
         | expr '^' expr { $$ = pow($1, $3); }
         | '(' expr ')'  { $$ = $2;}
         | '-' expr %prec UNARYMUNUS { $$ = -$2;}
         | '+' expr %prec UNARYPLUS {}
         ;
%%
