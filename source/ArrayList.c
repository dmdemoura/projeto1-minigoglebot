#include "ArrayList.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct ArrayList
{
    size_t dataSize;
    size_t elementCount;
    size_t allocatedCount;
    unsigned int preallocCount;
    void* array;
};

ArrayList* ArrayList_Create(size_t dataSize, unsigned int preallocCount)
{
    ArrayList* list = malloc(sizeof(ArrayList));
    list->dataSize = dataSize;
    list->elementCount = 0;
    list->allocatedCount = preallocCount;
    list->preallocCount = preallocCount;
    list->array = malloc(dataSize * preallocCount);
    return list;
}
void* ArrayList_Get(const ArrayList* list, size_t index)
{
    assert(index >= 0 && index < list->elementCount);
    return &list->array[index];
}
void * ArrayList_Find(const ArrayList* list, void* data, bool (*compare)(void*, void*))
{
    int i;
    for (i = 0; i < list->elementCount; i++)
    {
        if (compare(&list->array[i], data))
            return &list->array[i];
    }
    return NULL;
}
int ArrayList_Count(const ArrayList* list)
{
    if (!list) return -1;
    
    return list->elementCount;
}
void ArrayList_InsertEnd(ArrayList* list, const void* data)
{
    list->elementCount++;
    if (list->elementCount > list->allocatedCount)
        list->array = realloc(list->array, list->dataSize * (list->allocatedCount + list->preallocCount));

    memcpy(&list->array[list->elementCount - 1], data, list->dataSize);
}
void ArrayList_Destroy(ArrayList* list)
{
    free(list->array);
    free(list);
}
