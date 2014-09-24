#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CommandParser.h"

char** parse_line(char * line);

char* get_command(char* line)
{
    return parse_line(line)[0];
}

char** get_arguments(char* line)
{
    char** tokens = parse_line(line);
    return tokens;
}

char** parse_line(char * line)
{
    char *next;
    char *delim = " ";
    int cnt=0;
    char **tokens;

    next = strtok(line, delim);
    while (next) {
        tokens[cnt] = (char *) malloc(strlen(next)+1);
        strcpy(tokens[cnt++],next);
        next =strtok(NULL, delim);
    }
    tokens[cnt] = (char *) 0; /* make the field array be null-terminated */

    return tokens;
}
