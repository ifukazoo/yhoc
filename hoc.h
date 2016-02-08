#ifndef _HOC_H_
#define _HOC_H_

#include <stdlib.h>

extern int yylineno;
void yyerror(const char* fmt, ...);
int yylex(void);
void* emalloc(size_t size);
void reset_error(void);

#endif
