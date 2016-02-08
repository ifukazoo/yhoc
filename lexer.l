%option noyywrap nodefault yylineno

%{
#include "y.tab.h"
#include "hoc.h"
%}

%%
"."?[0-9]+      |
[0-9]+"."[0-9]* {
  sscanf(yytext, "%lf", &yylval.v);
  return NUMBER;
}
"+" |
"(" |
")" |
"-" |
"*" |
"%" |
"/" {
  return yytext[0];
}
;|\n {
  return EOS;
}
[ \t] {
}
. {
  yyerror("Mistery character %c\n",  *yytext);
}

%%

