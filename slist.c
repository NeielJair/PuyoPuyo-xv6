#include "types.h"
#include "stat.h"
#include "user.h"
#include "slist.h"

SList*
slist_create(void)
{
  SList *list = malloc(sizeof(SList));
  list->data = 0;
  list->next = 0;
  return list;
}

void
slist_add(SList *list, void *data)
{
  assert(list != 0, "slist_add: unexistant list\n");

  if(!data)
    return;

  if(list->data == 0){
    list->data = data;
    return;
  }

  SList *node = slist_create();
  node->data = data;
  node->next = 0;

  for(SList *iter = list; iter != 0; iter = iter->next)
    if(iter->next == 0){
      iter->next = node;
      break;
    }
}

uint
slist_length(SList *list)
{
  assert(list != 0, "slist_length: unexistant list\n");

  uint n = 0;
  for(SList *iter = list; iter != 0; iter = iter->next)
    n++;
  return n;
}

void*
slist_get(SList *list, uint n)
{
  assert(list != 0, "slist_get: unexistant list\n");

  uint i = 0;
  void *data = 0;
  for(SList *iter = list; iter != 0 && i != n; iter = iter->next, i++)
    data = iter->data;
  return data;
}

int
slist_find(SList *list, void *data)
{
  assert(list != 0, "slist_get: unexistant list\n");

  uint i = 0;
  for(SList *iter = list; iter != 0; iter = iter->next, i++)
    if(iter->data == data)
      return i;
  return -1;
}

void
slist_destroy(SList *list, void (*data_destroy)(void*))
{
  assert(list != 0, "slist_destroy: unexistant list\n");

  SList *toBeDestroyed;
  int loop = 1;
  for(SList *iter = list; loop;){
    if(data_destroy != 0)
      (*data_destroy)(iter->data);
    toBeDestroyed = iter;
    if(iter->next == 0)
      loop = 0;
    else
      iter = iter->next;
    free(toBeDestroyed);
  }
}
