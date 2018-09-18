#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "site.h"
#include "list.h"

#define FILE_NAME "googlebot.txt"
#define LINE_SIZE 700

int scan_tags(char* line, char tags [MAX_TAG_COUNT][TAG_SIZE])
{
    int i;
    int comma_count = 0;
    int tag_start = 0;
    bool scanning_tag = FALSE;
    int tag_count = 0;

    for(i=0; line[i] != '\0'; i++)
    {
        if(comma_count >= 4)
        {
            if (!scanning_tag)
            {
                tag_start = i;
                scanning_tag = TRUE;
            }
            else if (line[i] == ',' || line[i] == '\n')
            {
                line[i] = '\0';
                strcpy(tags[comma_count - 4], &line[tag_start]);
                scanning_tag = FALSE;
                tag_count++;
                comma_count++;
            }
        } else if(line[i] == ',')
            comma_count++;
        
    }
    return tag_count;
}

int main(int arg, char** argv)
{
    int j;
    FILE* file;
    LIST* list = list_create();
    char buffer[LINE_SIZE];
    int code;
    char name[NAME_SIZE];
    int relevance;
    char link[LINK_SIZE];
    char tags[MAX_TAG_COUNT][TAG_SIZE];
    int tag_count;

    /*////////////////////////////*/

    file = fopen(FILE_NAME, "r");

    while(fgets(buffer, LINE_SIZE, file))
    {
        sscanf(buffer, "%d,%[^,],%d,%[^,]", &code, name, &relevance, link);
        tag_count = scan_tags(buffer, tags);

        list_insert(list, site_create(code, name, relevance, link, tags, tag_count));
    }

    list_print(list);

    

    fclose(file);
    return 0;
}