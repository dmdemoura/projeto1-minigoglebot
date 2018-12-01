#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "list.h"
#include "avl.h"
#include "WordTree.h"
#include "ArrayList.h"
#include "googlebot.h"

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
bool googlebot_remove_site(GOOGLEBOT* googlebot, int siteCode)
{
    if (!googlebot) return false;

    if (avl_remove(googlebot->avl, siteCode))
    {
        WordTree_Remove(googlebot->word_tree, siteCode);
        return true;
    }
    return false;
}
bool googlebot_add_tag(GOOGLEBOT* googlebot, int siteCode, const char* tag)
{
    SITE* site;
    if (!googlebot) return false;

    site = avl_get(googlebot->avl, siteCode);
    if (site)
    {
        site_add_tag(site, tag);
        WordTree_Add(googlebot->word_tree, tag, site);
        return true;
    }
    return false;
}
bool googlebot_update_relevance(GOOGLEBOT *googlebot, int siteCode, int relevance)
{
    int i;
    SITE* site;
    int tag_count;
    if (!googlebot) return false;

    site = avl_get(googlebot->avl, siteCode);
    if (!site) return false;

    site_update_relevance(site, relevance);

    WordTree_Remove(googlebot->word_tree, siteCode);

    tag_count = site_get_num_tags(site);
    for (i = 0; i < tag_count; i++)
    {
        const char* tag = site_get_tag_by_index(site, i);
        WordTree_Add(googlebot->word_tree, tag, site);
    }

    return TRUE;
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
LIST* googlebot_suggest_sites(GOOGLEBOT *googlebot, const char *tag)
{
    int i;
    int j;
    int tag_count;
    const char* tag_buffer;
    const LIST* sites_list_buffer;
    int sites_buffer_size;
    const SITE** sites_buffer;
    ArrayList* array_list;
    LIST* suggested_sites;
    
    if (!googlebot) return NULL;
    
    sites_list_buffer = WordTree_Get(googlebot->word_tree, tag);
    if (!sites_list_buffer) return NULL;
    if (list_is_empty(sites_list_buffer)) return NULL;
    
    sites_buffer = list_get_nth_first_elements(sites_list_buffer, list_size(sites_list_buffer));
    sites_buffer_size = list_size(sites_list_buffer);

    suggested_sites = list_create(site_compare_relevance);
    array_list = ArrayList_Create(sizeof(char*), 30);
    
    
    for (i = 0; i < sites_buffer_size; i++)
    {
        assert(sites_buffer[i]);
        list_insert(suggested_sites, (SITE*) sites_buffer[i]);

        tag_count = site_get_num_tags(sites_buffer[i]);
        
        for (j = 0; j < tag_count; j++)
        {
            tag_buffer = site_get_tag_by_index(sites_buffer[i], j);
            if (!ArrayList_Find(array_list, (void*) tag_buffer, compare_ptr))
            {
                ArrayList_InsertEnd(array_list, tag_buffer);
            }
        }
    }

    free(sites_buffer);
    tag_count = (int) ArrayList_Count(array_list);
    
    for (i = 0; i < tag_count; i++)
    {
        sites_list_buffer = WordTree_Get(googlebot->word_tree, ArrayList_Get(array_list, (size_t) i));
        
        sites_buffer = list_get_nth_first_elements(sites_list_buffer, list_size(sites_list_buffer));
        sites_buffer_size = list_size(sites_list_buffer);
        
        for (j = 0; j < sites_buffer_size; j++)
        {
            assert(sites_buffer[j]);
            list_insert(suggested_sites, (SITE*) sites_buffer[j]);
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
