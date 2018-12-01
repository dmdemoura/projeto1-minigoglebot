#ifndef MINIGOOGLEBOT_GOOGLEBOT_H
#define MINIGOOGLEBOT_GOOGLEBOT_H

#include "site.h"
#include "list.h"
#include "bool.h"

typedef struct GOOGLEBOT GOOGLEBOT;

GOOGLEBOT* googlebot_create();
void googlebot_insert_site(GOOGLEBOT* googlebot, SITE* site);
bool googlebot_remove_site(GOOGLEBOT* googlebot, int siteCode);
bool googlebot_add_tag(GOOGLEBOT* googlebot, int siteCode, const char* tag);
bool googlebot_update_relevance(GOOGLEBOT* googlebot, int siteCode, int relevance);
const LIST* googlebot_find_by_tag(GOOGLEBOT* googlebot, const char* tag);
LIST * googlebot_suggest_sites(GOOGLEBOT *googlebot, const char *tag);
void googlebot_print(GOOGLEBOT* googlebot);
void googlebot_serialize(GOOGLEBOT* googlebot, FILE* file);
void googlebot_destroy(GOOGLEBOT** googlebot);

#endif
