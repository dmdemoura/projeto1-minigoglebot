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

    avl->root = NULL;
    avl->depth = -1;

    return avl;
}

/* Função que cria um nó com o site passado como parâmetro */
NODE* node_create(SITE* site) {
    NODE* node;

    if (site == NULL) {
        printf("node_create: site is null\n");
        return NULL;
    }
    node = malloc(sizeof(NODE));
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

bool avl_is_empty(const AVL* avl) {
    if (avl == NULL) {
        printf("avl_print: avl is null\n");
        return TRUE;
    }
    if (avl->root == NULL) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

/*=========================================================================*/
/* Funções auxiliares */

/* Função que retorna o maior valor entre dois*/
int highest_value(int value_a, int value_b) {
    return (value_a > value_b) ? value_a : value_b;
}

/* Função que retorna o menor nó de uma subárvore */ 
NODE* minimum_value_node(NODE* node) {
    if (node->left != NULL) {
        return minimum_value_node(node->left);
    }
    else {
        return node;
    }
}

/* Função que o fator de balanceamento */
int get_balance(NODE* node) {
    if (node == NULL) {
        return 0;
    }
    return (node->left ? node->left->height : -1) - (node->right ? node->right->height : -1);
}
/*_________________________________________________________________________*/

/*=========================================================================*/
/* Funções para rotacionar avl */
NODE* rotate_right(NODE* node_a) {
    NODE* aux = node_a->left;
    node_a->left = aux->right;
    aux->right = node_a;

    node_a->height = 1 + highest_value(node_a->right ? node_a->right->height : -1, node_a->left ? node_a->left->height : -1);
    aux->height = 1 + highest_value(aux->left ? aux->left->height : -1, node_a->height);

    return aux;
}

NODE* rotate_left(NODE* node_a) {
    NODE* aux = node_a->right;
    node_a->right = aux->left;
    aux->left = node_a;

    node_a->height = 1 + highest_value(node_a->right ? node_a->right->height : -1, node_a->left ? node_a->left->height : -1);
    aux->height = 1 + highest_value(aux->right ? aux->right->height : -1, node_a->height);

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

/*=========================================================================*/
/* Funções para inserir nó na avl */
NODE* avl_insert_node(NODE* node, SITE* site) {
    if (node == NULL) {
        node = node_create(site);
    }
    else if (site_get_code(site) > site_get_code(node->site)) {
        node->right = avl_insert_node(node->right, site);

        /*if (get_balance(node) == -2) {
            if (site_get_code(site) > site_get_code(node->right->site)) {
                node = rotate_left(node);
            }
            else {
                node = rotate_right_left(node);
            }
        }*/
        
        if (get_balance(node) < -1) {
            if (get_balance(node->right) <= 0) {
               node = rotate_left(node);
            }
            else {
               node = rotate_right_left(node);
            }
        }
    }
    else if (site_get_code(site) < site_get_code(node->site)) {
        node->left = avl_insert_node(node->left, site);

        /*if (get_balance(node) == 2) {
            if (site_get_code(site) < site_get_code(node->left->site)) {
                node = rotate_right(node);
            }
            else {
                node = rotate_left_right(node);
            }
        }*/
        if (get_balance(node) > 1) {
            if (get_balance(node->left) >= 0) {
               node = rotate_right(node);
            }
            else {
                node = rotate_left_right(node);
            }
        }
    }

    node->height = 1 + highest_value(node->left ? node->left->height : -1, node->right ? node->right->height : -1);
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

/*=========================================================================*/
/* Funções para buscar site na avl */
NODE* avl_get_node(NODE* node, int code) {
    if (code == site_get_code(node->site)) {
        return node;
    }
    else if (code > site_get_code(node->site)) {
        if (node->right != NULL) {
            return avl_get_node(node->right, code);
        }
        else {
            return NULL;
        }
    }
    else {
        if (node->left != NULL) {
            return avl_get_node(node->left, code);
        }
        else {
            return NULL;
        }
    }
}

SITE* avl_get(AVL* avl, int code) {
    if (avl == NULL) {
        printf("avl_get: avl is null\n");
        return NULL;
    }
    if (avl_is_empty(avl)) {
        printf("avl_get: avl is empty\n");
        return NULL;
    }

    return avl_get_node(avl->root, code)->site;
}
/*_________________________________________________________________________*/

/*=========================================================================*/
/* Funções para remover nó da avl */
NODE* avl_remove_node(NODE* node, int code) {
    NODE* child = NULL;
    NODE* tmp = NULL;
    int balance;

    if (node == NULL) {
        return node;
    }

    if (code > site_get_code(node->site)) {
        node->right = avl_remove_node(node->right, code);
    }
    else if (code < site_get_code(node->site)) {
        node->left = avl_remove_node(node->left, code);
    }
    else {
        if (node->right == NULL && node->left == NULL) {
            child = node;
            node = NULL;
        }
        else if (node->right == NULL|| node->left == NULL) {
            if (node->left != NULL) {
                child = node->left;
            }
            else {
                child = node->right;
            }
            tmp = malloc(sizeof(NODE));
            *tmp = *node;
            *node = *child;
            free(child);
        }
        else {
            child = minimum_value_node(node->right);
            node->site = child->site;
            node->right = avl_remove_node(node->right, code);
        }
    }

    if (node == NULL) {
        return node;
    }
    
    node->height = 1 + highest_value(node->left ? node->left->height : -1, node->right ? node->right->height : -1);
    balance = get_balance(node);

    if (balance > 1) {
        if (get_balance(node->left) >= 0) {
            rotate_right(node);
        }
        else {
            rotate_left_right(node);
        }
    }
    else if (balance < -1) {
        if (get_balance(node->right) <= 0) {
            rotate_left(node);
        }
        else {
            rotate_right_left(node);
        }
    }

    return node;
}

bool avl_remove(AVL* avl, int code) {
    if (avl == NULL) {
        printf("avl_remove: avl is null\n");
        return FALSE;
    }
    if (avl_is_empty(avl)) {
        printf("avl_remove: avl is empty\n");
        return FALSE;
    }
    
    return avl_remove_node(avl->root, code) ? TRUE : FALSE;
}
/*_________________________________________________________________________*/

/*=========================================================================*/
/* Funções para imprimir sites da avl */
void avl_print_node(const NODE* node) {
    if (node->left != NULL) {
        avl_print_node(node->left);
    }
    site_print(node->site);
    if (node->right != NULL) {
        avl_print_node(node->right);
    }
}

void avl_print(const AVL* avl) {
    if (avl == NULL) {
        printf("avl_print: avl is null\n");
        return;
    }
    if (avl_is_empty(avl)) {
        printf("avl_print: avl is empty\n");
        return;
    }

    avl_print_node(avl->root);
}
/*_________________________________________________________________________*/

/*=========================================================================*/
/* Funções para serializar sites da avl em um arquivo */
void avl_serialize_node(NODE* node, FILE* file) {
    if (node->left != NULL) {
        avl_serialize_node(node->left, file);
    }
    site_serialize(node->site, file);
    if (node->right != NULL) {
        avl_serialize_node(node->right, file);
    }
}

void avl_serialize(AVL* avl, FILE* file) {
    if (avl == NULL) {
        printf("avl_print: avl is null\n");
        return;
    }
    if (avl_is_empty(avl)) {
        printf("avl_print: avl is empty\n");
        return;
    }

    avl_serialize_node(avl->root, file);
}
/*_________________________________________________________________________*/
