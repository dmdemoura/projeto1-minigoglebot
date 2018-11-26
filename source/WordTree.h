#ifndef DM_TREE_H
#define DM_TREE_H

#include "bool.h"
#include "site.h"
#include "list.h"

/**
 * @brief A tree for storing english words containing only english letters.
 */
typedef struct WordTree WordTree;

/**
 * @brief Creates a new word tree.
 */
WordTree* WordTree_Create();
/**
 * @brief Adds a word to the tree
 * @param tree A non null pointer to a WordTree.
 * @param word An word containing only lowercase or uppercase ASCII letters, and null terminated.
 * @param site Site matched by this word, will NOT be freed on tree destruction.
 */
void WordTree_Add(WordTree* tree, char* word, SITE* site);
/**
 * @brief Checks if a word exists in the tree.
 * @param tree A non null pointer to a WordTree.
 * @param word An word containing only lowercase or uppercase ASCII letters, and null terminated.
 * @returns true if word exists, false if it doesn't.
 */
bool WordTree_Exists(WordTree* tree, char* word);
/**
 * @brief Returns the list of sites matched by word.
 * @param tree A non null pointer to a WordTree.
 * @param word An word containing only lowercase or uppercase ASCII letters, and null terminated.
 * @returns A list of sites if word exists, NULL if it doesn't.
 */
const LIST* WordTree_Get(WordTree* tree, char* word);
/**
 * @brief Removes site from all words.
 * @param tree A non null pointer to a WordTree.
 * @param code The code of the site to remove.
 */
void WordTree_Remove(WordTree* tree, int code);
/**
 * @brief Destroy a WordTree, freeing all allocated resources.
 * @details The SITE pointers inserted into the tree will NOT be freed.
 * @param tree The tree to be freed, do not try access the tree after destroying it.
 */
void WordTree_Destroy(WordTree* tree);

#endif