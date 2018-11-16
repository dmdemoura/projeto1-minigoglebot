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
    FILE* file;
    LIST* list = list_create();
    char buffer[LINE_SIZE];
    int code;
    char name[NAME_SIZE];
    int relevance;
    char link[LINK_SIZE];
    char* tags[MAX_TAG_COUNT];
    char* tagBuffer;
    int tag_count;

    /*////////////////////////////*/

    file = fopen(FILE_NAME, "r");

    while(fgets(buffer, LINE_SIZE, file))
    {
        sscanf(buffer, "%d,%[^,],%d,%[^,]", &code, name, &relevance, link);
        for (i = 0; TRUE; i++)
        {
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

        list_insert(list, site_create(code, name, relevance, link, tags, tag_count));
    }
    fclose(file);
    
    return list;
}
void clear_terminal(){
    int i;
    for (i = 0; i < TERMINAL_HEIGHT; i++)
        printf("\n"); 
}
void read_parameter(char* prompt, char* mask, void* data){
    char line_buffer[MAX_CMD_SIZE];
    int matches = 0;
    while (matches != 1){
        printf(prompt);
        fgets(line_buffer, MAX_CMD_SIZE, stdin);
        matches = sscanf(line_buffer, mask, data);
    }
}
void insert(LIST* list){
    char line_buffer[MAX_CMD_SIZE];
    char confirmation;
    int i;
    int matches = 0;
    int code;
    char name[NAME_SIZE];
    int relevance;
    char link[LINK_SIZE];
    int tag_count;
    char* tags[MAX_TAG_COUNT];
    char tag_prompt[9];

    read_parameter("Code (4 digits): ", "%d", &code);
    read_parameter("Name (50 characters): ", "%s", &name);
    read_parameter("Relevance (Between 0 and 1000): ", "%d", &relevance);
    read_parameter("Link (100 characters): ", "%s", &link);
    read_parameter("Tag count (Between 0 and 10): ", "%d", &tag_count);

    for (i = 0; i < tag_count; i++){
        tags[i] = malloc(sizeof(char) * TAG_SIZE);
        sprintf(tag_prompt, "Tags %d: ", i);
        read_parameter(tag_prompt, "%s", tags[i]);
        printf("%s\n", tags[i]);
    }

    SITE* site = site_create(code, name, relevance, link, tags, tag_count);
    if (site){
        site_print(site);
        read_parameter("Add site to list? (y/n): ", "%[ynYN]", &confirmation);
        switch (confirmation){
            case 'Y':
            case 'y':
                if (list_insert(list, site))
                    printf("Site succesfully inserted\n");
                else
                    printf("Error on site insertion\n");
            break;
        }
    } else {
        printf("Failed to create site.\n");
    }
}
void remove_site(LIST* list){
    int code;
    char confirmation;
    SITE* site;
    read_parameter("Code (4 digits): ", "%d", &code);
    if (site = list_get(list, code))
        site_print(site);
        read_parameter("Remove site from list? (y/n): ", "%[ynYN]", &confirmation);
        switch (confirmation){
            case 'Y':
            case 'y':
                if (list_remove(list, code);
                    printf("Site succesfully inserted\n");
                else
                    printf("Error on site insertion\n");
            break;
        }
    } else {
        printf("Failed to create site.\n");
    }
}
void drawMenu(){
        printf(
            "=================================\n"
            "\t1: Insert site\n"
            "\t2: Remove site\n"
            "\t3: Insert tag\n"
            "\t4: Update relevance\n"
            "\t5: Exit\n"
            "=================================\n\n"
            "googlebot> "
        );
}
void menu(LIST* list){
    char line_buffer[MAX_CMD_SIZE];
    int code = 0;
    int matches = 0;

    clear_terminal();
    printf("----Projeto Mini Googlebot----\n");
    drawMenu();

    while(NULL != fgets(line_buffer, MAX_CMD_SIZE, stdin)){
        code = 0;
        matches = sscanf(line_buffer, "%d", &code);

        if (matches == 1){
            switch(code){
                case 1:
                    insert(list);
                break;
                /*case 2:
                case 3:
                case 4:
                case 5:
                default: */
            }
            drawMenu();
        }
    }
}
int main(int arg, char** argv){
    LIST* list = load_file();

    menu(list);

    list_destroy(&list);

    return 0;
}