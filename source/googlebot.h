#ifndef MINIGOOGLEBOT_GOOGLEBOT_H
#define MINIGOOGLEBOT_GOOGLEBOT_H

#include "site.h"
#include "avl.h"
#include "list.h"

typedef struct GOOGLEBOT GOOGLEBOT;
 
/*Cria um TAD que consegue todas funcoes do minigooglebot*/
GOOGLEBOT* googlebot_create();
/*Insere um site na AVL e na WordTree */
void googlebot_insert_site(GOOGLEBOT* googlebot, SITE* site);
/*Remove um site da AVL e da WordTree */
void googlebot_remove_site(GOOGLEBOT* googlebot, int siteCode);
/*Adiciona tag ao site, na AVL, e na WordTree */
void googlebot_add_tag(GOOGLEBOT* googlebot, int siteCode, const char* tag);
/*Atualiza relevancia no site por meio da AVL*/
void googlebot_update_relevance(GOOGLEBOT* googlebot, int siteCode, int relevance);
/*Busca sites que contem uma tag usando a WordTree */
const LIST* googlebot_find_by_tag(GOOGLEBOT* googlebot, const char* tag);
/*Sugere sites que contem as tags que os sites com a tag passada tinham*/
AVL * googlebot_suggest_sites(GOOGLEBOT *googlebot, const char *tag);
/*Imprime os dados do googlebot*/
void googlebot_print(GOOGLEBOT* googlebot);
/**Serializa os dados do googlebot para imprimir no arquivo */
void googlebot_serialize(GOOGLEBOT* googlebot, FILE* file);
/**Destroi a WordTree e a AVL */
void googlebot_destroy(GOOGLEBOT** googlebot);

#endif
