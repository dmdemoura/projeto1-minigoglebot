#ifndef SITE_H
#define SITE_H

#include "bool.h"

#define NAME_SIZE 50
#define TAG_SIZE 50
#define MAX_TAG_COUNT 10
#define LINK_SIZE 100
#define MAX_RELEVANCE 1000


typedef struct site SITE;

SITE* site_create(int code, char name[NAME_SIZE], int relevance, char link[LINK_SIZE], char(*tag)[TAG_SIZE], int tag_count);
void site_destroy(SITE** site_ptr);
bool site_add_tag(SITE* site, char tag[TAG_SIZE]);
bool site_update_relevance(SITE* site, int relevance);
int site_get_code(SITE* site);
void site_print(SITE* site);

#endif