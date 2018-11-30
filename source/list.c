#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "bool.h"
#include "site.h"

/* Struct que representa um nó, que fará parte de uma lista encadeada */
typedef struct node{
    SITE* site;
    struct node* next;
} NODE;

struct list{
    NODE* first;
    NODE* last;
    int size;
    /* Should return 0 if equal,
     * greater than 0 first site is greater than the second one,
     * and lesser than 0 otherwise*/
    int (*compare)(SITE*, SITE*); 
};

/* Função que insere um nó numa lista, entre dois nós especificados 
static NODE* insert(LIST* list, NODE* node1, NODE* node2, SITE* site){
    NODE* new_node = malloc(sizeof(NODE));

    new_node->site = site;
    new_node->next = node2;
    node1->next = new_node;
    list->size++;

    return new_node;
}
*/
static void get_nth_first_elements(NODE* node, SITE** site_ptrs, int n)
{
    if (n == 0) return;
    if (!node)
        printf("This shouldnt happen\n");
    else
    {
        *site_ptrs = node->site;

        get_nth_first_elements(node->next, &site_ptrs[1], n - 1);
    }
}

LIST* list_create(int (*compare)(SITE*, SITE*)){
    LIST* list = (LIST*) malloc(sizeof(LIST));

    /* cheagem de erro */
    if(list == NULL){
        printf("list_create: Error on memory alocation\n");
        return NULL;
    }

    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    list->compare = compare;

    return list;
}

void list_destroy(LIST** list_ptr, bool also_destroy_data){
    NODE* current_node = (*list_ptr)->first;
    NODE* next_node = NULL;

    while(current_node != NULL){
        next_node = current_node->next;
        if (also_destroy_data) site_destroy(&current_node->site);
        free(current_node);
        current_node = next_node;
    }
    
    *list_ptr = NULL;
}
static void Insert(NODE* node, SITE* site, LIST* list)
{
    NODE* newNode;
    if (list->compare(site, node->site) <= 0 && node->next)
    {
        Insert(node->next, site, list);
    }
    newNode = malloc(sizeof(NODE));

    newNode->site = site;
    newNode->next = node->next;
    node->next = newNode;

    list->size++;
}
bool list_insert(LIST* list, SITE* site)
{
    if (!list) return false;
    if (!site) return false;
    if (!list->first) return false;

    Insert(list->first, site, list);
    return true;
}
/*
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
   while(current_node != NULL)
   {
       if(site_get_code(current_node->site) == site_code){
           return FALSE;
       }
       else if(list->compare(current_node->site, site) > 0)
       {
           if (previous_node == NULL)
           {
               if (list->first == list->last)
               {
                   list->first = malloc(sizeof(NODE));
                   list->first->site = site;
                   list->first->next = list->last;
                   list->size++;
               }
               else
               {
                   insert(list, list->first, current_node, site);
               }
           }
           else
           {
               insert(list, previous_node, current_node, site);
           }
           printf("Site inserted on list successfully\n");
           return TRUE;
       }
       previous_node = current_node;
       current_node = current_node->next;
   }
   list->last = insert(list, previous_node, current_node, site);

   return TRUE;
}
*/

bool list_remove(LIST* list, int code){
    NODE* current_node = NULL;
    NODE* previous_node = NULL;

    /* cheagem de erro */
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
            printf("loop\n");

/*            site_destroy(&current_node->site);*/
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

    /* cheagem de erro */
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

bool list_is_empty(const LIST* list){
    /* cheagem de erro */
    if(list == NULL){
        printf("list_is_empty: list is null\n");
        return FALSE;
    }

    return list->size == 0;
}

int list_size(const LIST* list){
    /* cheagem de erro */
    if(list == NULL){
        printf("list_size: list is null\n");
        return ERROR;
    }
    return list->size;
}

void list_serialize(LIST* list, FILE* file){
    NODE* current_node = list->first;

    /* cheagem de erro */
    if(list == NULL){
        printf("list_serialize: list is null\n");
        return;
    }

    while(current_node != NULL){
        site_serialize(current_node->site, file);
        current_node = current_node->next;
    }
}

void list_print(const LIST *list){
    NODE* current_node = list->first;

    /* cheagem de erro */
    if(list == NULL){
        printf("list_print: list is null\n");
        return;
    }
    if(list_is_empty(list)){
        printf("list_print: list is empty\n");
        return;
    }

    while(current_node != NULL){
        site_print(current_node->site);
        current_node = current_node->next;
    }
}
const SITE** list_get_nth_first_elements(const LIST* list, int elementCount)
{
    SITE** site_ptrs;
    if (!list) return NULL;
    if (elementCount > list->size) return NULL;
    
    site_ptrs = (SITE**) malloc(sizeof(SITE**) * elementCount);
    
    get_nth_first_elements(list->first, site_ptrs, elementCount);
    
    return (const SITE**) site_ptrs;
}
