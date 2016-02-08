%{
#include <math.h>
#include "hoc.h"

%}
%union {
  double v;
}

%token NUMBER
%token EOS
%left '+' '-'
%left '*' '/' '%'
%right UNARYMUNUS UNARYPLUS

%type <v> NUMBER expr

%start list
%%
list     :
         | list EOS
         | list expr  EOS { printf("\t%.8g\n", $2); }
         | list error EOS { yyerrok; }
         ;
expr     : NUMBER          { $$ = $1; }
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
