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
/**Struct da avl */
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
    /**Inicializa os valores da avl */
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

/* Função que calcula o fator de balanceamento */
int get_balance(NODE* node) {
    if (node == NULL) {
        return 0;
    }
    return (node->left ? node->left->height : -1) - (node->right ? node->right->height : -1);
}
/*_________________________________________________________________________*/

/*=========================================================================*/
/* Funções para rotacionar avl */
/** Função que faz a rotação direita  */
NODE* rotate_right(NODE* node_a) {
    NODE* aux = node_a->left;
    node_a->left = aux->right;
    aux->right = node_a;

    node_a->height = 1 + highest_value(node_a->right ? node_a->right->height : -1, node_a->left ? node_a->left->height : -1);
    aux->height = 1 + highest_value(aux->left ? aux->left->height : -1, node_a->height);

    return aux;
}
/**Função que faz a rotaçao esquerda */
NODE* rotate_left(NODE* node_a) {
    NODE* aux = node_a->right;
    node_a->right = aux->left;
    aux->left = node_a;

    node_a->height = 1 + highest_value(node_a->right ? node_a->right->height : -1, node_a->left ? node_a->left->height : -1);
    aux->height = 1 + highest_value(aux->right ? aux->right->height : -1, node_a->height);

    return aux;
}
/**Função que faz a rotaçao direita/esquerda */
NODE* rotate_right_left(NODE* node_a) {
    node_a->right = rotate_right(node_a->right);
    return rotate_left(node_a);
}
/**Função que faz a rotaçao esquerda/direita */
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
/**Função que destroi os nós da avl recursivamente */
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
    *avl_ptr = NULL;
}
/*_________________________________________________________________________*/

/*=========================================================================*/
/* Funções para buscar site na avl */
/**Função recursiva que encontra úm nó, dado um código */
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
/**Função que remove os nós da avl recursivamente */
NODE* avl_remove_node(NODE* node, int code) {
    NODE* child = NULL;
    NODE* tmp = NULL;
    int balance;

    if (node == NULL) {
        return node;
    }
    /**Faz a busca do nó que deseja remover */
    if (code > site_get_code(node->site)) {
        node->right = avl_remove_node(node->right, code);
    }
    else if (code < site_get_code(node->site)) {
        node->left = avl_remove_node(node->left, code);
    }
    /**Se o site foi encontrado, dado um código */
    else {
        /**Caso em que o nó que deseja remover é uma folha */
        if (node->right == NULL && node->left == NULL) {
            free(node);
            return NULL;
        }
        /**Caso em que o nó que deseja remover tem um único filho, seja ele direito ou esquerdo */
        else if (node->right == NULL|| node->left == NULL) {
            /**o filho é esquerdo */
            if (node->left != NULL) {
                child = node->left;
            }
            /**o filho é direito */
            else {
                child = node->right;
            }
            /**copia o que estava no nó que queremos remover para tmp */
            tmp = malloc(sizeof(NODE));
            *tmp = *node;
            /**copia o conteúdo do node que queremos remover para child, que é o node que vamos remover da árvore */
            *node = *child;
            free(child);

        }
        /**Caso em que o nó que deseja remover possui dois filhos */
        else {
            /**Acha o menor valor na subárvore da direita, para substituir o site no node que está atualmente o site procurado */
            child = minimum_value_node(node->right);
            /**destroi o site que vamos remover */
            site_destroy(&node->site);
            node->site = child->site;
            /**chama recursivamente a função para procurar pelo code do menor valor da subárvore direita, para então remover o nó */
            node->right = avl_remove_node(node->right, site_get_code(child->site));
        }
    }

    if (node == NULL) {
        return node;
    }
    /**atualiza a altura do nó */
    node->height = 1 + highest_value(node->left ? node->left->height : -1, node->right ? node->right->height : -1);
    /**calcula o fator de balanceamento do nó */
    balance = get_balance(node);

    /**Caso esteja desbalanceado, rebalanceia */

    if (balance > 1) {
        if (get_balance(node->left) >= 0) {
            return rotate_right(node);
        }
        else {
            return rotate_left_right(node);
        }
    }
    else if (balance < -1) {
        if (get_balance(node->right) <= 0) {
            return rotate_left(node);
        }
        else {
            return rotate_right_left(node);
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
    
    avl->root = avl_remove_node(avl->root, code);

    return TRUE;
}
/*_________________________________________________________________________*/

/*=========================================================================*/
/* Funções para imprimir sites da avl */
/**Função que imprime a avl em-ordem */
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
/**Função que serializa o conteúdo da avl, para colocar no arquivo recursivamente */
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
