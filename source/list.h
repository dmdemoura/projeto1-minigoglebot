#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include "site.h"
#include "bool.h"
#define ERROR -1

/* Struct que representa uma lista liner encadeada ordenada dinãmica */
typedef struct list LIST;

/* Função que cria uma lista */
LIST* list_create();
/* Função que destroi uma lista especificada */
void list_destroy(LIST** list, bool also_destroy_data);

/* Função que insere um site especificado em uma lista dada.
   O site será nserido de forma ordenada                  */
bool list_insert(LIST* list, SITE* site);
/* Função que remove um site especificado por código de uma lista */
bool list_remove(LIST* list, int code);

/* Função que retorna um site, dado o código dele*/
SITE* list_get(LIST* list, int code);

/* Função que checa se a lista está vazia */
bool list_is_empty(const LIST* list);
/* Função que retorna a quantidade de elementos da lista*/
int list_size(LIST* list);

/* Função qeu imprime uma lista para o usuário */
void list_print(const LIST *list);
/* Função que coloca todas as informações da lista num arquivo de extensão .csv */
void list_serialize(LIST* list, FILE* file);

#endif
