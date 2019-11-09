#include "symlist.h"
#include <string.h>
#include <stdlib.h>

List* list_store(List* list, string id, int type)
{   
  List* new = (List*)malloc(sizeof(List));
  new->id = strdup(id);
  new->type = type;
  new->next = list;
  return new;
}

int list_get(List* list, string id)
{
  List* p = list;
  while(p)
  {
    if( !strcmp(p->id,id) )
      return p->type;

    p = p->next;
  }

  return -1;
}
