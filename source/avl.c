#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "bool.h"
#include "site.h"

/* Struct que representa um nó, que fará parte davl */
typedef struct node {
    SITE* site;
    struct node* right;
    struct node* left;
    int height;

} NODE;

struct avl {
    NODE* root;
    int depth;
};


AVL* avl_create() {
    AVL* avl = malloc(sizeof(AVL));
    if (avl == NULL) {
        printf ("avl_create: creation failed\n");
        return NULL;
    }

    

    return avl;
}
/*
void avl_destroy(AVL** avl) {

}

bool avl_insert(AVL* avl, SITE* site) {

}

bool avl_remove(AVL* avl, int code) {

}

SITE* avl_get(AVL* avl, int code) {

}

void avl_print(AVL* avl) {

}

void avl_serialize(AVL* avl, FILE* file) {

}
*/