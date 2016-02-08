#include <string.h>
#include "hoc.h"
#include "symbol.h"

static symbol_t* head = NULL;

symbol_t* install(char* name, int type)
{
  symbol_t* new = emalloc(sizeof(symbol_t));
  new->name = emalloc(strlen(name) + 1);
  strcpy(new->name, name);
  new->type = type;
  new->next = head;
  head = new;
  return new;
}
symbol_t* lookup(char* name)
{
  symbol_t* p;
  for (p = head; p; p = p->next) {
    if ( ! strcmp(p->name, name)) {
      break;
    }
  }
  return p;
}

