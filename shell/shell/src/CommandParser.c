#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CommandParser.h"

char** parse_line(char * line);

char* get_command(char* line)
{
    char* copy = (char *)malloc(strlen(line) * sizeof(char));
    copy = strcpy(copy, line);
    char** tokens = parse_line(copy);
    char* command = tokens[0];
    
    int i;
    for (i = 0; i < 2048; i++)
        free(tokens[i]);
    free(tokens);
    free(copy);
    return command;
}

char** get_arguments(char* line)
{
    char* copy = (char *)malloc(strlen(line) * sizeof(char));
    copy = strcpy(copy, line);
    char** tokens = parse_line(copy);
    free(tokens[0]);
    ++tokens;
    free(copy);
    return tokens;
}

char** parse_line(char * line)
{
    char *next;
    const char *delim = " ";
    int cnt=0;
    char **tokens;
     
    tokens = (char**)malloc(2049 * sizeof(char*));

    next = strtok(line, delim);
    while (next) {
        tokens[cnt] = (char *) malloc(strlen(next)+1);
        strcpy(tokens[cnt++],next);
        next =strtok(NULL, delim);
    }
    tokens[cnt] = (char *) 0; /* make the field array be null-terminated */

    return tokens;
}



