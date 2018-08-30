#ifndef LIST_H
#define LIST_H

#include <site.h>

typedef struct list LIST;

LIST* list_create();
void list_destroy(LIST* list);
int list_insert(LIST* list, SITE* site);
LIST* list_remove(LIST* list, int code);
LIST* list_get(LIST* list, int code);

#endif