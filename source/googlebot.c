#include "googlebot.h"
#include <stdlib.h>
#include "list.h"
#include "avl.h"
#include "WordTree.h"

struct GOOGLEBOT
{
    WordTree* word_tree;
    AVL* avl;
};

GOOGLEBOT* googlebot_create()
{
    AVL* avl = avl_create();
    if (!avl) return NULL;

    WordTree* word_tree =  WordTree_Create();
    if (!word_tree) return NULL;

    GOOGLEBOT* googlebot = (GOOGLEBOT*) malloc(sizeof(GOOGLEBOT));
    if (!googlebot) return NULL;

    googlebot->avl = avl;
    googlebot->word_tree  = word_tree;

    return googlebot;
}
void googlebot_insert_site(GOOGLEBOT *googlebot, SITE* site)
{
    int tag_count;
    if (!googlebot) return;
    if (!site) return;

    avl_insert(googlebot->avl, site);

    tag_count = site_get_num_tags(site);
    for (int i = 0; i < tag_count; i++)
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
