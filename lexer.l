%option noyywrap nodefault yylineno

%{
#include "symbol.h"
#include "hoc.h"
#include "code.h"
#include "y.tab.h"
%}

EXP ([Ee][-+]?[0-9]+)

%%
"+=" { return ADDASGN; }
"-=" { return SUBASGN; }
"*=" { return MULASGN; }
"/=" { return DIVASGN; }
"%=" { return MODASGN; }
"^=" { return POWASGN; }
">=" { return GE; }
">"  { return GT; }
"<=" { return LE; }
"<"  { return LT; }
"==" { return EQ; }
"!=" { return NE; }
"!"  { return NOT; }

"+" |
"(" |
")" |
"{" |
"}" |
"-" |
"*" |
"%" |
"/" |
"^" |
"=" {
  return yytext[0];
}

[a-zA-Z][0-9a-zA-Z]* {
  symbol_t* sym = lookup(yytext);
  if (sym) {
    yylval.sym = sym;
  } else {
    // まだassignしてない変数
    yylval.sym = install(yytext, UNDEF);
  }

  if (yylval.sym->type == UNDEF || yylval.sym->type == CONST) {
    return VAR;
  } else {
    return yylval.sym->type;
  }
}

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? {
  double d;
  sscanf(yytext, "%lf", &d);
  yylval.sym = install("", NUMBER);
  yylval.sym->un.val = d;
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

