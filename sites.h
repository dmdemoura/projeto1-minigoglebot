#ifndef SITES_H
#define SITES_H

#define TRUE 1
#define FALSE 0

#define NAME_SIZE 50
#define TAG_SIZE 50
#define ZELDA_SIZE 100

typedef bool int;

typedef struct site SITES;

bool sites_insert(SITES* site, int code ,char[NAME_SIZE] name, 
                int relevance, char[ZELDA_SIZE] link,char*[TAG_SIZE] tag);
bool sites_remove(SITES* site, int code);
bool sites_add_tag(SITES* site, int code);
bool sites_update_relevance(SITES* site, int code);

#endif