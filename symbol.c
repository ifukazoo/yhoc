#include <string.h>
#include "hoc.h"
#include "symbol.h"

static struct symbol* head = NULL;

struct symbol* install(char* name, int type)
{
  struct symbol* new = emalloc(sizeof(struct symbol));
  new->name = emalloc(strlen(name) + 1);
  strcpy(new->name, name);
  new->type = type;
  new->next = head;
  head = new;
  return new;
}
struct symbol* lookup(char* name)
{
  struct symbol* p;
  for (p = head; p; p = p->next) {
    if ( ! strcmp(p->name, name)) {
      break;
    }
  }
  return p;
}

