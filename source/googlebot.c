#include "googlebot.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "avl.h"
#include "WordTree.h"
#include "ArrayList.h"

struct GOOGLEBOT
{
    WordTree* word_tree;
    AVL* avl;
};

GOOGLEBOT* googlebot_create()
{
    GOOGLEBOT* googlebot;
    WordTree* word_tree;
    AVL* avl = avl_create();
    if (!avl) return NULL;

    word_tree =  WordTree_Create();
    if (!word_tree) return NULL;

    googlebot = (GOOGLEBOT*) malloc(sizeof(GOOGLEBOT));
    if (!googlebot) return NULL;

    googlebot->avl = avl;
    googlebot->word_tree  = word_tree;

    return googlebot;
}
void googlebot_insert_site(GOOGLEBOT *googlebot, SITE* site)
{
    int i;
    int tag_count;
    if (!googlebot) return;
    if (!site) return;

    avl_insert(googlebot->avl, site);

    tag_count = site_get_num_tags(site);
    for (i = 0; i < tag_count; i++)
    {
        const char* tag = site_get_tag_by_index(site, i);
        WordTree_Add(googlebot->word_tree, tag, site);
    }
}
void googlebot_remove_site(GOOGLEBOT* googlebot, int siteCode)
{
    if (!googlebot) return;

    WordTree_Remove(googlebot->word_tree, siteCode);

    avl_remove(googlebot->avl, siteCode);
}
void googlebot_add_tag(GOOGLEBOT* googlebot, int siteCode, const char* tag)
{
    SITE* site;
    if (!googlebot) return;

    site = avl_get(googlebot->avl, siteCode);
    site_add_tag(site, tag);

    WordTree_Add(googlebot->word_tree, tag, site);
}
void googlebot_update_relevance(GOOGLEBOT* googlebot, int siteCode, int relevance)
{
    SITE* site;
    if (!googlebot) return;

    site = avl_get(googlebot->avl, siteCode);
    site_update_relevance(site, relevance);
}
const LIST* googlebot_find_by_tag(GOOGLEBOT* googlebot, const char* tag)
{
    if (!googlebot) return NULL;

    return WordTree_Get(googlebot->word_tree, tag);
}
static bool compare_ptr(void* ptr1, void* ptr2)
{
    return ptr1 == ptr2;
}
AVL* googlebot_suggest_sites(GOOGLEBOT *googlebot, const char *tag)
{
    int i;
    int j;
    int tag_count;
    const char* tag_buffer;
    const LIST* sites_list_buffer;
    int sites_buffer_size;
    const SITE** sites_buffer;
    ArrayList* array_list;
    AVL* suggested_sites; 
    
    if (!googlebot) return NULL;
    
    sites_list_buffer = WordTree_Get(googlebot->word_tree, tag);
    if (!sites_list_buffer) return NULL;
    
    sites_buffer = list_get_nth_first_elements(sites_list_buffer, list_size(sites_list_buffer));
    sites_buffer_size = list_size(sites_list_buffer);
    
    suggested_sites = avl_create(); /* TODO: add avl relevance sort function */
    array_list = ArrayList_Create(sizeof(char*), 30);
    
    
    for (i = 0; i < sites_buffer_size; i++)
    {
        avl_insert(suggested_sites, (SITE*) sites_buffer[i]);
        tag_count = site_get_num_tags(sites_buffer[i]);
        
        for (j = 0; j < tag_count; j++)
        {
            tag_buffer = site_get_tag_by_index(sites_buffer[i], j);
            if (!ArrayList_Find(array_list, (void*) tag_buffer, compare_ptr))
            {
                printf("Inserting %s: ", tag_buffer);
                ArrayList_InsertEnd(array_list, tag_buffer);
            }
        }
    }

    free(sites_buffer);
    tag_count = (int) ArrayList_Count(array_list);
    
    for (i = 0; i < tag_count; i++)
    {
        printf("Searching for: %s\n", (char*) ArrayList_Get(array_list, (size_t) i));
        sites_list_buffer = WordTree_Get(googlebot->word_tree, ArrayList_Get(array_list, (size_t) i));
        
        sites_buffer = list_get_nth_first_elements(sites_list_buffer, list_size(sites_list_buffer));
        sites_buffer_size = list_size(sites_list_buffer);
        
        for (j = 0; j < sites_buffer_size; j++)
        {
            avl_insert(suggested_sites, (SITE*) sites_buffer[j]);
        }
    }

    ArrayList_Destroy(array_list);

    return suggested_sites;

}
void googlebot_print(GOOGLEBOT* googlebot)
{
    avl_print(googlebot->avl);
}
void googlebot_serialize(GOOGLEBOT* googlebot, FILE* file)
{
    avl_serialize(googlebot->avl, file);
}
void googlebot_destroy(GOOGLEBOT** googlebot)
{
    WordTree_Destroy((*googlebot)->word_tree);
    avl_destroy(&(*googlebot)->avl);

    free(*googlebot);
    *googlebot = NULL;
}
