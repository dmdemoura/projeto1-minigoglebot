#ifndef DM_T4_ARRAYLIST_H
#define DM_T4_ARRAYLIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct ArrayList ArrayList;
/**Função que cria uma ArrayList */
ArrayList* ArrayList_Create(size_t dataSize, unsigned int preallocCount);
/**Função que acha um elemento dadod um index */
void* ArrayList_Get(const ArrayList* list, size_t index);
/**Função que acha um elemento dado um informação na qual se quer basear a procura */
void* ArrayList_Find(const ArrayList* list, void* data, bool (*compare)(void*, void*));
/**Função que retorna quantos elementos tem na ArrayList */
size_t ArrayList_Count(const ArrayList* list);
/**Função que insere um elemento no final da ArrayList */
void ArrayList_InsertEnd(ArrayList* list, const void* data);
/**Funcão que destroi a ArrayList */
void ArrayList_Destroy(ArrayList* list);

#endif
