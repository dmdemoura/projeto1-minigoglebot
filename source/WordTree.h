#ifndef DM_TREE_H
#define DM_TREE_H

#include "bool.h"
#include "site.h"
#include "list.h"

/**Struct que guarda as tags*/
typedef struct WordTree WordTree;

/**Cria uma nova WordTree */
WordTree* WordTree_Create();
/**Adiciona uma tag na WordTree */
void WordTree_Add(WordTree *tree, const char *word, SITE *site);
/**Checa se uma tag existe na WordTree */
bool WordTree_Exists(WordTree* tree, char* word);
/**Retorna a lista de sites que contém a tag */
const LIST* WordTree_Get(WordTree *tree, const char *word);
/**Remove os sites de todas as tags */
void WordTree_Remove(WordTree* tree, int code);
/**Destroi a WorldTree*/
void WordTree_Destroy(WordTree* tree);

#endif