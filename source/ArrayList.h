#ifndef DM_T4_ARRAYLIST_H
#define DM_T4_ARRAYLIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct ArrayList ArrayList;

ArrayList* ArrayList_Create(size_t dataSize, unsigned int preallocCount);
void* ArrayList_Get(const ArrayList* list, size_t index);
void* ArrayList_Find(const ArrayList* list, void* data, bool (*compare)(void*, void*));
size_t ArrayList_Count(const ArrayList* list);
void ArrayList_InsertEnd(ArrayList* list, const void* data);
void ArrayList_Destroy(ArrayList* list);

#endif
