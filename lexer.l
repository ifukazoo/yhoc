%option noyywrap nodefault yylineno

%{
#include "y.tab.h"
#include "hoc.h"
#include "symbol.h"
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
"^" |
"=" {
  return yytext[0];
}

[a-zA-Z][0-9a-zA-Z]* {
  struct symbol* sym = lookup(yytext);
  if (sym) {
    yylval.sym = sym;
  } else {
    yylval.sym = install(yytext, VAR);
  }
  return yylval.sym->type;
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

