#ifndef __SYMLIST_H__
#define __SYMLIST_H__

typedef char* string;

typedef struct _List
{
  string id;
  int type;
  struct _List *next;
} List;


List* list_store(List* list, string id, int type);
int list_get(List* list, string id);

#endif
