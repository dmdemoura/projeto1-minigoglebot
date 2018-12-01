#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "googlebot.h"

#define FILE_NAME "googlebot.txt"
#define LINE_SIZE 700
#define TERMINAL_HEIGHT 60
#define MAX_CMD_SIZE 81


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
enum parameter_type{
    parameter_type_interval = 1,
    parameter_type_size = 2
};
void clear_input(FILE* file)
{
    while (fgetc(file) != '\n');
}
void read_parameter(char* prompt, char* mask, void* data, enum parameter_type type, int max_size){
    int matches = 0;
    int buffer_size = type == parameter_type_size ? max_size : MAX_CMD_SIZE;
    size_t buffer_len;
    char* line_buffer = (char*) malloc(buffer_size);

    while (matches != 1){

        printf("%s", prompt);

        fgets(line_buffer, buffer_size, stdin);

        buffer_len = strlen(line_buffer);
        if(line_buffer[buffer_len - 1] == '\n') /*Se todo input do usuario foi lido*/
        {
            matches = sscanf(line_buffer, mask, data);

            if (type == parameter_type_interval)
            {
                if (*((int *) data) > max_size || *((int *) data) < 0)
                {
                    printf("Error: number entered is out of bounds (0 a %d):\n", max_size);
                    matches = 0;
                }
            }
        }
        else /*Remove o que restou da linha nao lida*/
        {
            printf("Error: amount of characters typed exceeds the limit (%d).\n", buffer_size - 1);
            clear_input(stdin);
        }
    }
    free(line_buffer);
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
    read_parameter("Code (4 digits): ", "%d", &code, parameter_type_interval, MAX_CODE_SIZE);
    read_parameter("Name (50 characters): ", "%s", &name, parameter_type_size, NAME_SIZE);
    read_parameter("Relevance (Between 0 and 1000): ", "%d", &relevance, parameter_type_interval, MAX_RELEVANCE);
    read_parameter("Link (100 characters): ", "%s", &link, parameter_type_size, LINK_SIZE);
    read_parameter("Tag count (Between 0 and 10): ", "%d", &tag_count, parameter_type_interval, MAX_TAG_COUNT);

    /*  Leitura das tags do site que será inserido */
    for (i = 0; i < tag_count; i++){
        tags[i] = malloc(sizeof(char) * TAG_SIZE);
        sprintf(tagPrompt, "Tags %d: ", i);
        read_parameter(tagPrompt, "%[a-zA-Z]", tags[i], parameter_type_size, TAG_SIZE);
    }
    
    site = site_create(code, name, relevance, link, tags, tag_count);
    site_print(site);

    googlebot_insert_site(googlebot, site);
}

/* Função que remove site de uma lista, dado o código dele */
void remove_site(GOOGLEBOT* googlebot){
    int code;
    read_parameter("Code (4 digits): ", "%d", &code, parameter_type_interval, MAX_CODE_SIZE);

    googlebot_remove_site(googlebot, code);
}

/* Função que adiciona tag a um site, especificado pelo código */
void add_tag(GOOGLEBOT* googlebot){
    int code;
    char tag[TAG_SIZE];
    
    read_parameter("Code (4 digits): ", "%d", &code, parameter_type_interval, MAX_CODE_SIZE);
    read_parameter("Tag (Max 50 letters): ", "%[a-zA-Z]", tag, parameter_type_size, TAG_SIZE);

    if (googlebot_add_tag(googlebot, code, tag))
    {
        printf("Tag %s was added successfully to site %d.\n", tag, code);
    }
    else
    {
        printf("Couldn't add tag %s, site %d does not exist.\n", tag, code);
    }
}

/* Função que atualiza relevância de um site, especificado pelo código */
void update_relevance(GOOGLEBOT* googlebot){
    int code;
    int relevance;

    read_parameter("Code (4 digits): ", "%d", &code, parameter_type_interval, MAX_CODE_SIZE);
    read_parameter("Relevance (Between 0 and 1000): ", "%d", &relevance, parameter_type_interval, MAX_RELEVANCE);

    if (googlebot_update_relevance(googlebot, code, relevance))
    {
        printf("Relevance of %d was removed successfully.\n", code);
    }
    else
    {
        printf("Couldn't remove site, site %d does not exist.\n", code);
    }
}
void find_by_tag(GOOGLEBOT* googlebot)
{
    char tag[TAG_SIZE];
    const LIST* sites;

    read_parameter("Search for tag (Max 50 letters): ", "%[a-zA-Z]", &tag, parameter_type_size, TAG_SIZE);

    sites = googlebot_find_by_tag(googlebot, tag);
    if (sites)
    {
        list_print(sites);
        /*Não dar free em sites, é uma lista da tree, por isso é const*/
    }
    else
    {
        printf("Couldn't find any sites with tag %s.\n", tag);
    }
}
void suggest_a_site(GOOGLEBOT* googlebot)
{
    const char tag[TAG_SIZE];
    LIST* suggested_sites;
    
    read_parameter("Suggest sites with this tag (Max 50 letters): ", "%[a-zA-Z]", (void*) &tag, parameter_type_size, TAG_SIZE);
    
    suggested_sites = googlebot_suggest_sites(googlebot, (const char*) tag);
    if (suggested_sites)
    {
        list_print(suggested_sites);
        list_destroy(&suggested_sites, false);
    }
    else
    {
        printf("Couldn't find any sites with tag %s.\n", tag);
    }
}
void save_to_file(GOOGLEBOT* googlebot)
{
    FILE* file = NULL;
    char filename[MAX_CMD_SIZE];

    printf("Filename to save to (Max 80 characters): ");
    while (fgets(filename, MAX_CMD_SIZE, stdin))
    {
        filename[strlen(filename) - 1] = '\0';
        file = fopen(filename, "w");
        if (file) break;
        printf("Filename to save to (Max 80 characters): ");
    }

    if (file)
    {
        googlebot_serialize(googlebot, file);
        fclose(file);
    }
    else
    {
        printf("Failed to open file.");
    }
}
/* Função que imprime o menu de ações, e chama funções para cada comando dado pelo usuário */
void menu(GOOGLEBOT* googlebot){
    int code = 0;

    clear_terminal();
    printf("----Projeto Mini Googlebot----\n");

    while(true){
        read_parameter(
            "\n"
            "======================================\n"
            "\t1: Insert site\n"
            "\t2: Remove site\n"
            "\t3: Add tag\n"
            "\t4: Update relevance\n"
            "\t5: Print list\n"
            "\t6: Search by tag\n"
            "\t7: Suggest a site\n"
            "\t8: Save to file\n"
            "\t9: Exit\n"
            "======================================\n"
            "minigooglebot> ",
            "%d",&code, parameter_type_interval, 9);

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
                save_to_file(googlebot);
                break;
            case 9:
                return;
            default:
                break;
        }
    }
}
FILE* open_file_prompt()
{
    FILE* file = NULL;
    bool useDefault = false;
    char buffer[MAX_CMD_SIZE];

    while (true)
    {
        while (true)
        {
            read_parameter("Do you want to load a file (yes/no/default): ", "%s", buffer, parameter_type_size, MAX_CMD_SIZE);

            if (!strcmp(buffer, "yes") || !strcmp(buffer, "y"))
            {
                useDefault = false;
                break;
            }
            else if (!strcmp(buffer, "no") || !strcmp(buffer, "n"))
            {
                return NULL;
            }
            else if (!strcmp(buffer, "default"))
            {
                useDefault = true;
                break;
            }
        }

        if (useDefault)
        {
            file = fopen(FILE_NAME, "r");
            if (file) return file;
        }
        else
        {
            while(true)
            {
                read_parameter("Filename to open: ", "%s", buffer, parameter_type_size, MAX_CMD_SIZE);

                file = fopen(buffer, "r");
                if (file) return file;
            }
        }
    }
}

/* Função principal, que cria um lista de sites, abre o menu para que o usuário possa
solicitar ações,e ao final, salva todos os dados da lista num arquivo, e destroi a lista */
int main(int arg, char** argv){
    FILE* sourceFile = open_file_prompt();
    GOOGLEBOT* googlebot;

    if (sourceFile)
    {
        googlebot = load_file(sourceFile);
    }
    else
    {
        googlebot = googlebot_create();
    }

    menu(googlebot);

    googlebot_destroy(&googlebot);

    return 0;
}
