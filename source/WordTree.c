#include <stdlib.h>
#include <assert.h>
#include "WordTree.h"

#define LETTER_COUNT 26

typedef struct Node Node;
struct Node
{
    Node* childs[LETTER_COUNT]; 
    LIST* sites;                
};
struct WordTree
{
    Node* firstNode;
};

/*Converte um char para um index de array*/
static int CharToIndex(char c)
{
    int index;
    if ('a' <= c && c <= 'z')
        index = c - 'a';
    else if ('A' <= c && c <= 'Z')
        index = c - 'A';
    else
        return -1;

    return index;
}
/**Checa se é o final de uma palavra */
static bool IsWordEnd(Node* node)
{
    return node->sites ? true : false;
}
/**Cria um nó vazio */
static Node* CreateNode()
{
    return calloc(1, sizeof(Node)); /*Ensure all fields will be zero. */
}
/**Destroi um nó e todos os seus filhos */
static void DestroyNode(Node* node)
{
    int i;
    for (i = 0; i < LETTER_COUNT; i++)
        if (node->childs[i])
            DestroyNode(node->childs[i]);
        
    if (node->sites)
        list_destroy(&node->sites, false);

    free(node);
}

/**Adiciona uma palavra a arvore recursivamente */
static void AddWord(Node *node, const char *word, SITE *site)
{
    int childIndex; 
    if (!site) printf("AddWord: Trying to insert NULL site");
    childIndex = CharToIndex(word[0]);
    if (word[0] == '\0')
    {
        if (!node->sites)
        {
            node->sites = list_create(site_compare_relevance);
        }
        list_insert(node->sites, site);
        return;
    }

    assert(childIndex != -1);

    if (node->childs[childIndex] == NULL)
        node->childs[childIndex] = CreateNode();
    
    AddWord(node->childs[childIndex], &word[1], site); 
}
/**Checa se uma palavra existe em uma árvore recursivamente*/
static bool ExistsWord(Node* node, char* word)
{
    int childIndex = CharToIndex(word[0]);
    if (word[0] == '\0')
    {
       return IsWordEnd(node);
    }

    if (childIndex == -1)
    {
        return false;
    }

    if (node->childs[childIndex] == NULL)
        return false;
    
    return ExistsWord(node->childs[childIndex], &word[1]);
}
/**Retorna os site, dado uma palavra */
static LIST* GetSites(Node *node, const char *word)
{
    int childIndex = CharToIndex(word[0]);
    if (word[0] == '\0')
    {
        if (IsWordEnd(node))
        {
            return node->sites;
        }
        else return NULL;
    }

    if (childIndex == -1)
    {
        return NULL;
    }

    if (node->childs[childIndex] == NULL)
        return NULL;
    
    return GetSites(node->childs[childIndex], &word[1]);
}
/**Remove um site */
static void RemoveSite(Node* node, int code)
{
    if (node != NULL)
    {
        int i;
        if (node->sites)
        {
            list_remove(node->sites, code, false);
            if (list_is_empty(node->sites))
            {
                list_destroy(&node->sites, false);
            }
        }

        for (i = 0; i < LETTER_COUNT; i++)
        {

            RemoveSite(node->childs[i], code);
        }
    }
}

WordTree* WordTree_Create()
{
    WordTree* tree = malloc(sizeof(WordTree));
    if (!tree) return NULL;

    tree->firstNode = NULL;

    return tree;
}
void WordTree_Add(WordTree *tree, const char *word, SITE *site)
{
    if (!site)
        printf("WordTree: Trying to add NULL site");
    if (tree->firstNode == NULL)
        tree->firstNode = CreateNode();
    
    AddWord(tree->firstNode, word, site);
}
bool WordTree_Exists(WordTree* tree, char* word)
{
    if (tree->firstNode == NULL)
        return false;

    return ExistsWord(tree->firstNode, word);    
}
const LIST* WordTree_Get(WordTree *tree, const char *word)
{
    if (tree->firstNode == NULL)
        return NULL;

    return GetSites(tree->firstNode, word);
}
void WordTree_Remove(WordTree* tree, int code)
{
    RemoveSite(tree->firstNode, code);
}
void WordTree_Destroy(WordTree* tree)
{
    if (tree->firstNode)
        DestroyNode(tree->firstNode);
    free(tree);
}
