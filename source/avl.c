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

/* Função que cria um nó com o site passado como parâmetro */
NODE* node_create(SITE* site) {
    NODE* node = malloc(sizeof(NODE));

    if (site == NULL) {
        printf("node_create: site is null\n");
        return NULL;
    }
    if (node == NULL) {
        printf("node_create: node creation falied\n");
        return NULL;
    }

    node->site = site;
    node->right = NULL;
    node->left= NULL;
    node->height = -1;

    return node;
}

int highest_node_high(int height_a, int height_b) {
    return (height_a > height_b) ? height_a : height_b;
}

/*=========================================================================*/
/* Funções para rotacionar avl */
NODE* rotate_right(NODE* node_a) {
    NODE* aux = node_a->left;
    node_a->left = aux->right;
    aux->right = node_a;

    node_a->height = highest_node_high(node_a->right->height, node_a->left->height) + 1;
    aux->height = highest_node_high(aux->left->height, node_a->height) + 1;

    return aux;
}

NODE* rotate_left(NODE* node_a) {
    NODE* aux = node_a->right;
    node_a->right = aux->left;
    aux->left = node_a;

    node_a->height = highest_node_high(node_a->right->height, node_a->left->height) + 1;
    aux->height = highest_node_high(aux->right->height, node_a->height) + 1;

    return aux;
}

NODE* rotate_right_left(NODE* node_a) {
    node_a->right = rotate_right(node_a->right);
    return rotate_left(node_a);
}

NODE* rotate_left_right(NODE* node_a) {
    node_a->left = rotate_left(node_a->left);
    return rotate_right(node_a);
}
/*_________________________________________________________________________*/

AVL* avl_create() {
    AVL* avl = malloc(sizeof(AVL));
    if (avl == NULL) {
        printf ("avl_create: creation failed\n");
        return NULL;
    }

    avl->root = NULL;
    avl->depth = -1;

    return avl;
}

/*=========================================================================*/
/* Funções para inserir nó na avl */
NODE* avl_insert_node(NODE* node, SITE* site) {
    if (node == NULL) {
        node = node_create(site);
    }
    else if (site_get_code(site) > site_get_code(node->site)) {
        node->right = avl_insert_node(node->right, site);

        if (node->left->height - node->right->height == -2) {
            if (site_get_code(site) > site_get_code(node->right->site)) {
                node = rotate_left(node);
            }
            else {
                node = rotate_right_left(node);
            }
        }
    }
    else if (site_get_code(site) < site_get_code(node->site)) {
        node->left = avl_insert_node(node->left, site);

        if (node->left->height - node->right->height == 2) {
            if (site_get_code(site) < site_get_code(node->left->site)) {
                node = rotate_right(node);
            }
            else {
                node = rotate_left_right(node);
            }
        }
    }

    node->height = highest_node_high(node->left->height, node->right->height) + 1;
    return node;
}

bool avl_insert(AVL* avl, SITE* site) {
    if (avl == NULL) {
        printf("avl_insert: avl is null\n");
        return FALSE;
    }
    if (site == NULL) {
        printf("avl_insert: site is null\n");
        return FALSE;
    }

    avl->root = avl_insert_node(avl->root, site);

    if (avl->root == NULL) {
        printf("avl_insert: avl doesn't have root anymore\n");
        return FALSE;
    }

    return TRUE;
}
/*_________________________________________________________________________*/

bool avl_remove(AVL* avl, int code) {
    if (avl == NULL) {
        printf("avl_remove: avl is null\n");
        return FALSE;
    }
    return TRUE;
}

/*=========================================================================*/
/* Funções para destruir avl */
void avl_destroy_node(NODE* node) {
    if (node != NULL) {
        avl_destroy_node(node->left);
        avl_destroy_node(node->right);

        site_destroy(&(node->site));
        free(node);
    }
}

void avl_destroy(AVL** avl_ptr) {
    AVL* avl = (*avl_ptr);
    if (avl == NULL) {
        printf("avl_destroy: avl is null\n");
        return;
    }
    
    avl_destroy_node(avl->root);
    free(avl);
    avl = NULL;
}
/*_________________________________________________________________________*/


/*
SITE* avl_get(AVL* avl, int code) {

}

void avl_print(AVL* avl) {

}

void avl_serialize(AVL* avl, FILE* file) {

}
*/