#ifndef SITE_H
#define SITE_H

#include <stdio.h>
#include "bool.h"

#define MAX_CODE_SIZE 9999
#define NAME_SIZE 51
#define TAG_SIZE 51
#define MAX_TAG_COUNT 10
#define LINK_SIZE 101
#define MAX_RELEVANCE 1000


typedef struct site SITE;

SITE* site_create(int code, char name[NAME_SIZE], int relevance, char link[LINK_SIZE], char** tag, int tag_count);
void site_destroy(SITE** site_ptr);
bool site_add_tag(SITE* site, char tag[TAG_SIZE]);
bool site_update_relevance(SITE* site, int relevance);
int site_get_code(SITE* site);
void site_serialize(SITE* site, FILE* file);
void site_print(SITE* site);

#endif