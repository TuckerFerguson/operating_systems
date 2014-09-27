#include "CommandParser.h"

char* sanitize_token(char * token);

char** get_tokenized_command(char * line)
{
    char *next;
    const char *delim = " ";
    int cnt=0;
    char **tokens;
    
    char* copy = (char*)malloc(strlen(line) * sizeof(char));
    strcpy(copy, line);
     
    tokens = (char**)malloc(MAX_SHELL_ARGUMENTS * sizeof(char*));

    next = strtok(copy, delim);
    while (next) {
        tokens[cnt] = (char *) malloc(strlen(next)+1);
        char * token = tokens[cnt++];
        strcpy(sanitize_token(token), next);
        next =strtok(NULL, delim);
    }
    tokens[cnt] = (char *) 0; /* make the field array be null-terminated */

    return tokens;
}

char* sanitize_token(char * token)
{
    return token;
}