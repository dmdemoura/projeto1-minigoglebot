#include <stdlib.h>
#include "site.h"

struct site 
{
    int code;
    char name[NAME_SIZE];
    int relevance;
    char link[LINK_SIZE];
    char* tag[TAG_SIZE];
};

void site_destroy(SITE* site){

}

int site_get_code(SITE* site){
    return 0;
}