%option noyywrap nodefault yylineno

%{
#include "y.tab.h"
#include "hoc.h"
%}

EXP ([Ee][-+]?[0-9]+)

%%
"+" |
"(" |
")" |
"-" |
"*" |
"%" |
"/" |
"=" {
  return yytext[0];
}

[a-z] {
  yylval.index = yytext[0] - 'a';
  return VAR;
}

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? {
  sscanf(yytext, "%lf", &yylval.val);
  return NUMBER;
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

