#include <stdlib.h>
#include <assert.h>
#include "WordTree.h"

#define LETTER_COUNT 26

typedef struct Node Node;
struct Node
{
    Node* childs[LETTER_COUNT]; /*Array of pointers to Nodes. */
    LIST* sites;                /*List of sites matched by this word, if different than null, marks if a word ends here, though another word may extend further into the tree. */
};
struct WordTree
{
    Node* firstNode;
};
/*********************
 * Internal functions
 *********************/

/**
 * @brief Converts a character to an array index.
 * @details Lowercase and uppercase are considered the same.
 * @param c An ASCII lowercase or uppercase letter.
 * @returns An index from 0 to 25.
 */
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
static bool IsWordEnd(Node* node)
{
    return node->sites ? true : false;
}
/**
 * @brief Creates a node.
 * @returns An empty node.
 */
static Node* CreateNode()
{
    return calloc(1, sizeof(Node)); /*Ensure all fields will be zero. */
}
/**
 * @brief Destroy a node and all childs.
 * @param node A non null node to destroy.
 */
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
/**
 * @brief Recursevely add a word to the tree.
 * @param node Node from which to starting adding the word.
 * @param word Word to add, actually, what remains of the word to add in a recursive call.
 */
static void AddWord(Node *node, const char *word, SITE *site)
{
    int childIndex = CharToIndex(word[0]);
    if (word[0] == '\0')
    {
        if (!node->sites)
        {
            node->sites = list_create();
        }
        list_insert(node->sites, site);
        return;
    }

    assert(childIndex != -1);

    if (node->childs[childIndex] == NULL)
        node->childs[childIndex] = CreateNode();
    
    AddWord(node->childs[childIndex], &word[1], site); /*Recursively call itself with word starting at next char. */
}
/**
 * @brief Recursevely check if a word exists in the tree.
 * @param node Node from which to starting checking.
 * @param word Word to check, actually, what still needs to be checked in a recursive call.
 */
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
/**
 * @brief Recursively check if a word exists in the tree.
 * @param node Node from which to starting checking.
 * @param word Word to check, actually, what still needs to be checked in a recursive call.
 */
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
static void RemoveSite(Node* node, int code)
{
    if (node != NULL)
    {
        int i;
        if (node->sites)
        {
            list_remove(node->sites, code);
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
/*********************
 * External functions
 *********************/
WordTree* WordTree_Create()
{
    WordTree* tree = malloc(sizeof(WordTree));
    if (!tree) return NULL;

    tree->firstNode = NULL;

    return tree;
}
void WordTree_Add(WordTree *tree, const char *word, SITE *site)
{
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
