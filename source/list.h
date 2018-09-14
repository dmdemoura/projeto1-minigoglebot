#ifndef LIST_H
#define LIST_H

#include "site.h"
#include "bool.h"
#define ERROR -1

typedef struct list LIST;

LIST* list_create();
void list_destroy(LIST** list);
bool list_insert(LIST* list, SITE* site);
bool list_remove(LIST* list, int code);
SITE* list_get(LIST* list, int code);
bool list_is_empty(LIST* list);
int list_size(LIST* list);


#endif