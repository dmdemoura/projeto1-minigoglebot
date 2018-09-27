#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "bool.h"
#include "site.h"

typedef struct node{
    SITE* site;
    struct node* next;
} NODE;

struct list{
    NODE* first;
    NODE* last;
    int size;
};

static NODE* insert(LIST* list, NODE* node1, NODE* node2, SITE* site){
    NODE* new_node = malloc(sizeof(NODE));
    new_node->site = site;
    new_node->next = node2;
    node1->next = new_node;
    list->size++;
    return new_node;
}


LIST* list_create(){
    LIST* list = malloc(sizeof(LIST));
    if(list == NULL){
        printf("list_create: Error on memory alocation\n");
        return NULL;
    }
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

void list_destroy(LIST** list_ptr){
    NODE* current_node = (*list_ptr)->first;
    NODE* next_node = NULL;

    while(current_node != NULL){
        next_node = current_node->next;
        site_destroy(&current_node->site);
        free(current_node);
        current_node = next_node;
    }
    
    free(*list_ptr);
    list_ptr = NULL;
}

bool list_insert(LIST* list, SITE* site){
    NODE* current_node = NULL;
    NODE* previous_node = NULL;
    int site_code;

    if(list == NULL){
        printf("list_insert: list is null\n");
        return FALSE;
    }
    if(site == NULL){
        printf("list_insert: site is null\n");
        return FALSE;
    }

    if(list_is_empty(list)){
        list->first = malloc(sizeof(NODE));
        list->first->next = NULL;
        list->last = list->first;
        list->first->site = site;
        list->size++;
        return TRUE;
    }

    site_code = site_get_code(site);
    current_node = list->first;
    while(current_node != NULL){
        if(site_get_code(current_node->site) == site_code){
            printf("list_insert: site already exists\n");
            return FALSE;
        }
        else if(site_get_code(current_node->site) > site_code){
            if(previous_node == NULL)
                insert(list, list->first, current_node, site);

            else
                insert(list, previous_node, current_node, site);

            return TRUE;
        }
        previous_node = current_node;
        current_node = current_node->next;
    }
    list->last = insert(list, previous_node, current_node, site);    
    
    return TRUE;
}

bool list_remove(LIST* list, int code){
    NODE* current_node = NULL;
    NODE* previous_node = NULL;

    if(list == NULL){
        printf("list_remove: list is null\n");
        return FALSE;
    }

    current_node = list->first;

    while(current_node != NULL){
        if(site_get_code(current_node->site) == code){
            if(previous_node == NULL){
                list->first = current_node->next;
            }
            else if(current_node->next != NULL){
                previous_node->next = current_node->next;
            }
            else{
                previous_node->next = NULL;
                list->last = previous_node;
            }

            site_destroy(&current_node->site);
            list->size--;
            return TRUE;
        }

        previous_node = current_node;
        current_node = current_node->next;
    }
    printf("list_remove: site not found\n");
    return FALSE;
}

SITE* list_get(LIST* list, int code){
    NODE* current_node = NULL;

    if(list == NULL){
        printf("list_get: list is null\n");
        return NULL;
    }

    current_node = list->first;

    while(current_node != NULL){
        if(site_get_code(current_node->site) == code){
            return current_node->site;
        }
        current_node = current_node->next;
    }

    printf("list_get: site not found\n");
    return NULL;
}

bool list_is_empty(LIST* list){
    if(list == NULL){
        printf("list_is_empty: list is null\n");
        return FALSE;
    }

    return list->size == 0;
}

int list_size(LIST* list){
    if(list == NULL){
        printf("list_size: list is null\n");
        return ERROR;
    }
    return list->size;
}
void list_serialize(LIST* list, FILE* file){
    NODE* current_node = list->first;

    if(list == NULL){
        printf("list_print: list is null\n");
        return;
    }

    while(current_node != NULL){
        site_serialize(current_node->site, file);
        current_node = current_node->next;
    }
}
void list_print(LIST* list){
    NODE* current_node = list->first;

    if(list == NULL){
        printf("list_print: list is null\n");
        return;
    }

    while(current_node != NULL){
        site_print(current_node->site);
        current_node = current_node->next;
    }
}