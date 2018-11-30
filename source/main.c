#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "googlebot.h"

#define FILE_NAME "googlebot.txt"
#define LINE_SIZE 700
#define TERMINAL_HEIGHT 60
#define MAX_CMD_SIZE 80


GOOGLEBOT* load_file(FILE* file){
    int i;
    int code;
    SITE* site;
    int tag_count;
    int relevance;
    char* tagBuffer;
    char link[LINK_SIZE];
    char name[NAME_SIZE];
    char buffer[LINE_SIZE];
    char* tags[MAX_TAG_COUNT];
    GOOGLEBOT* googlebot = googlebot_create();

    while(fgets(buffer, LINE_SIZE, file)){
        sscanf(buffer, "%d,%[^,],%d,%[^,]", &code, name, &relevance, link);
        
        for (i = 0; TRUE; i++){
            if (i == 0)
                strtok(buffer, ",");
            else if (i < 4)
                strtok(NULL, ",");
            else
            {
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

        site = site_create(code, name, relevance, link, tags, tag_count);
        googlebot_insert_site(googlebot, site);
    }
    fclose(file);

    return googlebot;
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
    size_t string_size = 0;
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

void insert(GOOGLEBOT* googlebot){
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
    read_parameter("Relevance (Between 0 and 1000): ", "%d", &relevance, interval, MAX_RELEVANCE);
    read_parameter("Link (100 characters): ", "%s", &link, size, LINK_SIZE);
    read_parameter("Tag count (Between 0 and 10): ", "%d", &tag_count, interval, MAX_TAG_COUNT);

    /*  Leitura das tags do site que será inserido */
    for (i = 0; i < tag_count; i++){
        tags[i] = malloc(sizeof(char) * TAG_SIZE);
        sprintf(tagPrompt, "Tags %d: ", i);
        read_parameter(tagPrompt, "%[a-zA-Z]", tags[i], size, TAG_SIZE);
    }
    
    site = site_create(code, name, relevance, link, tags, tag_count);
    site_print(site);

    googlebot_insert_site(googlebot, site);
}

/* Função que remove site de uma lista, dado o código dele */
void remove_site(GOOGLEBOT* googlebot){
    int code;
    read_parameter("Code (4 digits): ", "%d", &code, interval, MAX_CODE_SIZE);

    googlebot_remove_site(googlebot, code);
}

/* Função que adiciona tag a um site, especificado pelo código */
void add_tag(GOOGLEBOT* googlebot){
    int code;
    char tag[TAG_SIZE];
    
    read_parameter("Code (4 digits): ", "%d", &code, interval, MAX_CODE_SIZE);
    read_parameter("Tag (Max 50 letters): ", "%[a-zA-Z]", tag, size, TAG_SIZE);

    googlebot_add_tag(googlebot, code, tag);
}

/* Função que atualiza relevância de um site, especificado pelo código */
void update_relevance(GOOGLEBOT* googlebot){
    int code;
    int relevance;

    read_parameter("Code (4 digits): ", "%d", &code, interval, MAX_CODE_SIZE);
    read_parameter("Relevance (Between 0 and 1000): ", "%d", &relevance, interval, MAX_RELEVANCE);

    googlebot_update_relevance(googlebot, code, relevance);
}
void find_by_tag(GOOGLEBOT* googlebot)
{
    char tag[TAG_SIZE];
    const LIST* sites;

    read_parameter("Search for tag (Max 50 letters): ", "%[a-zA-Z]", &tag, size, TAG_SIZE);

    sites = googlebot_find_by_tag(googlebot, tag);
    if (sites) list_print(sites);
}
void suggest_a_site(GOOGLEBOT* googlebot)
{
    const char* tag[TAG_SIZE];
    const AVL* suggested_sites;
    
    read_parameter("Suggest sites with this tag (Max 50 letters): ", "%[a-zA-Z]", &tag, size, TAG_SIZE);
    
    suggested_sites = googlebot_suggest_sites(googlebot, (const char*) tag);
    if (suggested_sites) avl_print(suggested_sites);
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
            "\t6: Search by tag\n"
            "\t7: Suggest a site\n"
            "\t8: Exit\n"
            "======================================\n\n"
        );
}

/* Função que imprime o menu de ações, e chama funções para cada comando dado pelo usuário */
void menu(GOOGLEBOT* googlebot){
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
                    insert(googlebot);
                    break;
                case 2:
                    remove_site(googlebot);
                    break;
                case 3:
                    add_tag(googlebot);
                    break;
                case 4:
                    update_relevance(googlebot);
                    break;
                case 5:
                    googlebot_print(googlebot);
                    break;
                case 6:
                    find_by_tag(googlebot);
                    break;
                case 7:
                    suggest_a_site(googlebot);
                    break;
                case 8:
                    return;
                    break;
            }
            drawMenu();
        }
        printf("googlebot> ");
    }
}
FILE* open_file_prompt()
{
    int matches;
    FILE* file;
    bool useDefault = false;
    char buffer[MAX_CMD_SIZE];

    while (true)
    {
        printf("Do you want to load a file (yes/no/default): ");
        while (fgets(buffer, MAX_CMD_SIZE, stdin))
        {
            buffer[strlen(buffer) - 1] = '\0';
            if (!strcmp(buffer, "yes") || !strcmp(buffer, "y"))
            {
                useDefault = false;
                break;
            } else if (!strcmp(buffer, "no") || !strcmp(buffer, "n"))
            {
                return NULL;
            } else if (!strcmp(buffer, "default"))
            {
                useDefault = true;
                break;
            }
            printf("Do you want to load a file (yes/no/default): ");
        }

        if (useDefault)
        {
            file = fopen(FILE_NAME, "r");
            if (file) break;
        }
        else
        {
            printf("Filename to open: ");
            while (fgets(buffer, MAX_CMD_SIZE, stdin))
            {
                buffer[strlen(buffer) - 1] = '\0';
                file = fopen(buffer, "r");
                if (file) break;
                printf("Filename to open: ");
            }
            break;
        }
    }
    return file;
}

/* Função principal, que cria um lista de sites, abre o menu para que o usuário possa
solicitar ações,e ao final, salva todos os dados da lista num arquivo, e destroi a lista */
int main(int arg, char** argv){
    FILE* sourceFile = open_file_prompt();
    GOOGLEBOT* googlebot = load_file(sourceFile);
    FILE* file;

    menu(googlebot);

    file = fopen("site_list.txt", "w");
    googlebot_serialize(googlebot, file);
    fclose(file);

    googlebot_destroy(&googlebot);

    return 0;
}
