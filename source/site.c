#include <site.h>
#include <stdlib.h>

struct site 
{
    int code;
    char name[NAME_SIZE];
    int relevance;
    char link[LINK_SIZE];
    char* tag[TAG_SIZE];
};

bool sites_insert(SITE* site, int code, char name[NAME_SIZE], int relevance, char link[LINK_SIZE], char* tag[TAG_SIZE])
{

}

bool sites_remove(SITE* sites, int code)
{

}

bool sites_add_tag(SITE* sites, int code)
{

}

bool sites_update_relevance(SITE* sites, int code)
{

}
