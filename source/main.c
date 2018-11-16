#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "site.h"
#include "list.h"

#define FILE_NAME "googlebot.txt"
#define LINE_SIZE 700
#define TERMINAL_HEIGHT 60
#define MAX_CMD_SIZE 80
 
LIST* load_file(){
    int i;
    int code;
    int tag_count;
    int relevance;
    char* tagBuffer;
    char link[LINK_SIZE];
    char name[NAME_SIZE];
    char buffer[LINE_SIZE];
    char* tags[MAX_TAG_COUNT];
    LIST* list = list_create();
    FILE* file = fopen(FILE_NAME, "r");



    while(fgets(buffer, LINE_SIZE, file)){
        sscanf(buffer, "%d,%[^,],%d,%[^,]", &code, name, &relevance, link);
        
        for (i = 0; TRUE; i++){
            if (i == 0)
                strtok(buffer, ",");
            else if (i < 4)
                strtok(NULL, ",");
            else{
                tagBuffer = strtok(NULL, ",");
                if (tagBuffer)
                    tags[i - 4] = tagBuffer;
                else
                    break;
            }
        }
        tag_count = i - 4;

        /* Remove pulo de linha no fim da ultima tag lida */
        tags[tag_count - 1][strlen(tags[tag_count - 1]) - 1] = '\0';

        list_insert(list, site_create(code, name, relevance, link, tags, tag_count));
    }
    fclose(file);
    
    return list;
}

/* Função que limpa p texto presente no terminal */
void clear_terminal(){
    int i;
    for (i = 0; i < TERMINAL_HEIGHT; i++)
        printf("\n"); 
}

/* Enumerador para dizer se o tipo de dado do parâmetro é limitado por quantidade de chars ou por um intervalo numérico*/
enum type{
    interval = 1,
    size = 2
};

void read_parameter(char* prompt, char* mask, void* data, int type, int max_size){
    int max = 0;
    int matches = 0;
    int string_size = 0;
    char line_buffer[MAX_CMD_SIZE];

    while (matches != 1 || max > max_size){
        printf("%s", prompt);
        fgets(line_buffer, MAX_CMD_SIZE, stdin);
        matches = sscanf(line_buffer, mask, data);

        if(type == size){
            string_size = strlen((char*)data);
            if(string_size > max_size)
                matches = 0;
        }
        else if(type == interval){
            if(*((int*)data) > max_size || *((int*)data) < 0)
                matches = 0;
        }
    }
}

void insert(LIST* list){
    int i;
    int code;
    int relevance;
    int tag_count;
    char tagPrompt[9];
    char link[LINK_SIZE];
    char name[NAME_SIZE];
    char* tags[MAX_TAG_COUNT];
    SITE* site;

    /* Leitura dos dados do site que será innserido */
    read_parameter("Code (4 digits): ", "%d", &code, interval, MAX_CODE_SIZE);
    read_parameter("Name (50 characters): ", "%s", &name, size, NAME_SIZE);
    read_parameter("Relevance (Between 0 and 1000): ", "%d", &relevance, interval, 1000);
    read_parameter("Link (100 characters): ", "%s", &link, size, LINK_SIZE);
    read_parameter("Tag count (Between 0 and 10): ", "%d", &tag_count, interval, MAX_TAG_COUNT);

    /*  Leitura das tags do site que será inserido */
    for (i = 0; i < tag_count; i++){
        tags[i] = malloc(sizeof(char) * TAG_SIZE);
        sprintf(tagPrompt, "Tags %d: ", i);
        read_parameter(tagPrompt, "%s", tags[i], size, TAG_SIZE);
    }
    
    site = site_create(code, name, relevance, link, tags, tag_count);
    site_print(site);
    list_insert(list, site);
}

/* Função que remove site de uma lista, dado o código dele */
void remove_site(LIST* list){
    int code;
    read_parameter("Code (4 digits): ", "%d", &code, interval, MAX_CODE_SIZE);
    list_remove(list, code);
}

/* Função que adiciona tag a um site, especificado pelo código */
void add_tag(LIST* list){
    int code;
    char tag[TAG_SIZE];
    
    read_parameter("Code (4 digits): ", "%d", &code, interval, MAX_CODE_SIZE);
    read_parameter("Tag: ", "%s", tag, size, TAG_SIZE);

    site_add_tag(list_get(list, code), tag);
}

/* Função que atualiza relevância de um site, especificado pelo código */
void update_relevance(LIST* list){
    int code;
    int relevance;

    read_parameter("Code (4 digits): ", "%d", &code, interval, MAX_CODE_SIZE);
    read_parameter("Relevance (Between 0 and 1000): ", "%d", &relevance, interval, 1000);

    site_update_relevance(list_get(list, code), relevance);
}

/* Função que imprime o menu de ações para o usuário */
void drawMenu(){
        printf(
            "\n\n"
            "======================================\n"
            "\t1: Insert site\n"
            "\t2: Remove site\n"
            "\t3: Add tag\n"
            "\t4: Update relevance\n"
            "\t5: Print list\n"
            "\t6: Exit\n"
            "======================================\n\n"
        );
}

/* Função que imprime o menu de ações, e chama funções para cada comando dado pelo usuário */
void menu(LIST* list){
    int code = 0;
    int matches = 0;
    char line_buffer[MAX_CMD_SIZE];

    clear_terminal();
    printf("----Projeto Mini Googlebot----\n");
    drawMenu();
    printf("googlebot> ");

    while(NULL != fgets(line_buffer, MAX_CMD_SIZE, stdin)){
        code = 0;
        matches = sscanf(line_buffer, "%d", &code);

        if (matches == 1){
            switch(code){
                case 1:
                    insert(list);
                    break;
                case 2:
                    remove_site(list);
                    break;
                case 3:
                    add_tag(list);
                    break;
                case 4:
                    update_relevance(list);
                    break;
                case 5:
                    list_print(list);
                    break;
                case 6:
                    return;
                    break;
            }
            drawMenu();
        }
        printf("googlebot> ");
    }
}

/* Função principal, que cria um lista de sites, abre o menu para que o usuário possa
solicitar ações,e ao final, salva todos os dados da lista num arquivo, e destroi a lista */
int main(int arg, char** argv){
    LIST* list = load_file();
    FILE* file;

    menu(list);

    file = fopen("site_list.txt", "w");
    list_serialize(list, file);
    fclose(file);

    list_destroy(&list);

    return 0;
}