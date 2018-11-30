#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include "site.h"
#include "bool.h"
#define ERROR (-1)

/* Struct que representa uma árvore AVL */
typedef struct avl AVL;

/* Função que cria uma AVL */
AVL* avl_create();

/* Função que destroi uma AVL */
void avl_destroy(AVL** avl);

/* Função que insere um site especificado em uma avl dada */
bool avl_insert(AVL* avl, SITE* site);

/* Função que remove um site especificado por código de uma avl */
bool avl_remove(AVL* avl, int code);

/* Função que retorna um site, dado o código dele */
SITE* avl_get(AVL* avl, int code);

/* Função que checa se a avl está vazia */
bool avl_is_empty(const AVL* avl);

/* Função qeu imprime uma avl para o usuário */
void avl_print(const AVL* avl);

/* Função que coloca todas as informações da avl num arquivo de extensão .csv */
void avl_serialize(AVL* avl, FILE* file);

#endif
