#ifndef SITE_H
#define SITE_H

#include <stdio.h>
#include "bool.h"

#define MAX_CODE_SIZE 9999
#define NAME_SIZE 51
#define TAG_SIZE 51
#define MAX_TAG_COUNT 10
#define LINK_SIZE 101
#define MAX_RELEVANCE 1000

/* Struct com informações sobre um site */
typedef struct site SITE;

/* Função que cria um site contendo informações passadas por parâmetro, e retorna esse site */
SITE* site_create(int code, char name[NAME_SIZE], int relevance, char link[LINK_SIZE], char** tag, int tag_count);
/* Função que destroi um site, dado um ponteiro para ponteiro para site */
void site_destroy(SITE** site_ptr);

/* Função que adiciona tag a um site, ambos passados por parâmetro */
bool site_add_tag(SITE* site, const char tag[TAG_SIZE]);
/* Função atualiza relevância de um site, ambos passados por parâmetro */
bool site_update_relevance(SITE* site, int relevance);

/* Função que retorna o site representado pelo código enviado por parãmetro */
int site_get_code(SITE* site);

/* Função qeu imprime um site para o usuário */
void site_print(SITE* site);
/* Função que coloca todas as informações da site num arquivo de extensão .csv */
void site_serialize(SITE* site, FILE* file);

int site_get_num_tags(const SITE* site);
const char* site_get_tag_by_index(const SITE* site, int index);
int site_compare_relevance(SITE* site1, SITE* site2);

#endif
