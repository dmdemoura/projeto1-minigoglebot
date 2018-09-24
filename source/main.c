#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "site.h"
#include "list.h"

#define FILE_NAME "googlebot.txt"
#define LINE_SIZE 700

int main(int arg, char** argv)
{
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

        //Remove pulo de linha no fim da ultima tag lida
        tags[tag_count - 1][strlen(tags[tag_count - 1]) - 1] = '\0';

        list_insert(list, site_create(code, name, relevance, link, tags, tag_count));
    }

    list_print(list);

    fclose(file);
    return 0;
}