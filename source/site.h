#ifndef SITE_H
#define SITE_H

#include <bool.h>

#define NAME_SIZE 50
#define TAG_SIZE 50
#define LINK_SIZE 100

typedef struct site SITE;

bool site_create(SITE* site, int code, char name[NAME_SIZE], int relevance, char link[LINK_SIZE], char* tag[TAG_SIZE]);
//bool sites_remove(SITE* site);
bool site_add_tag(SITE* site, int code);
bool site_update_relevance(SITE* site, int code);

#endif