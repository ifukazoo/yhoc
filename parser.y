%{
#include <math.h>
#include "hoc.h"

double memory[26];

%}
%union {
  double val;
  int index;
}

%token NUMBER
%token EOS
%token VAR
%right '='
%left '+' '-'
%left '*' '/' '%'
%right UNARYMUNUS UNARYPLUS

%type <val> NUMBER expr
%type <index> VAR

%start list
%%
list     :
         | list EOS
         | list expr    EOS { printf("\t%.8g\n", $2); }
         | list error   EOS { yyerrok; }
         ;
expr     : NUMBER        { $$ = $1; }
         | VAR           { $$ = memory[$1]; }
         | VAR  '=' expr { memory[$1] = $3; $$ = $3; }
         | expr '+' expr { $$ = $1 + $3; }
         | expr '-' expr { $$ = $1 - $3; }
         | expr '*' expr { $$ = $1 * $3; }
         | expr '/' expr { $$ = $1 / $3; }
         | expr '%' expr { $$ = fmod($1, $3); }
         | '(' expr ')'  { $$ = $2;}
         | '-' expr %prec UNARYMUNUS { $$ = -$2;}
         | '+' expr %prec UNARYPLUS {}
         ;
%%
