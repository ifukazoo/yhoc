%{
#include <math.h>
#include "symbol.h"
#include "code.h"
#include "hoc.h"

inline inst_t* code2(void* a, void* b) {
  inst_t* start = code((inst_t)a); code((inst_t)b);
  return start;
}
inline inst_t* code3(void* a, void* b, void* c) {
  inst_t* start = code((inst_t)a); code((inst_t)b); code((inst_t)c);
  return start;
}
%}
%union {
  inst_t*  inst;
  symbol_t* sym;
}

%token NUMBER VAR BUILTIN CONST UNDEF GT GE LT LE EQ NE AND OR NOT EOS PRINT SEM
%token ADDASGN SUBASGN MULASGN DIVASGN MODASGN POWASGN
%token IF ELSE WHILE FOR
%left ADDASGN SUBASGN MULASGN DIVASGN MODASGN POWASGN
%right '='
%left OR
%left AND
%left EQ NE
%left GT GE LT LE
%left '+' '-'
%left '*' '/' '%'
%right UNARYMUNUS UNARYPLUS NOT
%right '^' /* - 2 ^ 3 => - (2 ^ 3) */

%type <inst> expr assign stmt stmtlist while for delim if cond end andleft and
%type <sym> NUMBER VAR BUILTIN PRINT

%start list
%%
list             :
                 | list EOS
                 | list assign  EOS { code2(shift, STOP); return 1; }
                 | list stmt    EOS { code(STOP)        ; return 1; }
                 | list expr    EOS { code2(print, STOP); return 1; }
                 | list error   EOS { yyerrok; }
                 ;
assign           : VAR '='     expr {code3(pushvar, $1, assign);    $$ = $3;}
                 | VAR ADDASGN expr {code3(pushvar, $1, addassign); $$ = $3;}
                 | VAR SUBASGN expr {code3(pushvar, $1, subassign); $$ = $3;}
                 | VAR MULASGN expr {code3(pushvar, $1, mulassign); $$ = $3;}
                 | VAR DIVASGN expr {code3(pushvar, $1, divassign); $$ = $3;}
                 | VAR MODASGN expr {code3(pushvar, $1, modassign); $$ = $3;}
                 | VAR POWASGN expr {code3(pushvar, $1, powassign); $$ = $3;}
                 ;
stmt             : expr             {code(shift);  $$ = $1;}
                 | PRINT expr       {code(prexpr); $$ = $2;}
                 | '{' stmtlist '}' {              $$ = $2;}
                 | while cond stmt end {
                     *($1 + 1) = (inst_t)$3;
                     *($1 + 2) = (inst_t)$4;
                   }
                 | if cond stmt end ELSE stmt end {
                     *($1 + 1) = (inst_t)$3;
                     *($1 + 2) = (inst_t)$6;
                     *($1 + 3) = (inst_t)$7;
                   }
                 | if cond stmt end {
                     *($1 + 1) = (inst_t)$3;
                     *($1 + 3) = (inst_t)$4;
                   }
                 | for '(' assign delim expr delim assign end ')' stmt end {
                     *($1 + 1) = (inst_t)$5;
                     *($1 + 2) = (inst_t)$7;
                     *($1 + 3) = (inst_t)$10;
                     *($1 + 4) = (inst_t)$11;
                   }
                 ;
stmtlist         :                      { $$ = next_code(); }
                 | stmtlist EOS         { $$ = $1; }
                 | stmtlist stmt        { $$ = $1; }
while            : WHILE                { $$ = code(whilecode); code2(STOP, STOP); }
                 ;
for              : FOR                  { $$ = code(forcode); code2(STOP, STOP);code2(STOP, STOP);}
                 ;
delim            : SEM                  { code(STOP); $$ = next_code();}
                 ;
if               : IF                   { $$ = code(ifcode); code3(STOP, STOP, STOP); }
                 ;
cond             : '(' expr ')'         { code(STOP); $$ = $2;}
                 ;
end              :                      { code(STOP); $$ = next_code();}
                 ;
andleft          : expr AND             {
                     $$ = code(andleft);
                     code((inst_t)$1);
                   }
                 ;
and              : andleft expr         {
                     code(andright);
                     $$ = (inst_t*)*($1 + 1);
                     *($1 + 1) = (inst_t)next_code();
                   }
                 ;
expr             : BUILTIN '(' expr ')' { $$ = code3(pushvar, $1, callbuiltin); }
                 | NUMBER               { $$ = code2(pushconst, $1); }
                 | VAR                  { $$ = code3(pushvar, $1, eval); }
                 | and                  { $$ = $1; }
                 | assign        {code(eval); $$ = $1;}
                 | expr '+' expr {code(add);  $$ = $1;}
                 | expr '-' expr {code(sub);  $$ = $1;}
                 | expr '*' expr {code(mul);  $$ = $1;}
                 | expr '/' expr {code(div_); $$ = $1;}
                 | expr '%' expr {code(mod);  $$ = $1;}
                 | expr '^' expr {code(pow_); $$ = $1;}
                 | '(' expr ')'  {            $$ = $2;}
                 | '-' expr %prec UNARYMUNUS {$$ = code(negate);}
                 | '+' expr %prec UNARYPLUS  {$$ = $2; }
                 | expr GT  expr {code(gt);   $$ = $1;}
                 | expr GE  expr {code(ge);   $$ = $1;}
                 | expr LT  expr {code(lt);   $$ = $1;}
                 | expr LE  expr {code(le);   $$ = $1;}
                 | expr EQ  expr {code(eq);   $$ = $1;}
                 | expr NE  expr {code(ne);   $$ = $1;}
                 | expr OR  expr {code(or);   $$ = $1;}
                 | NOT expr      {code(not);  $$ = $2;}
                 ;
%%
