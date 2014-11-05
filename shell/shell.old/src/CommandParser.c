
#include "CommandParser.h"
#include "JobManager.h"

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

    free(copy);
    return tokens;
}

char* sanitize_token(char * token)
{
    return token;
}

int is_background_job(char** tokenized_command_and_args)
{
    //two cases
    //i) it is conjoined with the final command
    //ii) it is all alone as the final arugment
    
    int token_with_ampersand;
    for (token_with_ampersand = 0; token_with_ampersand < MAX_SHELL_ARGUMENTS && tokenized_command_and_args[token_with_ampersand]; ++token_with_ampersand);
    
    --token_with_ampersand;
    if (strstr(tokenized_command_and_args[token_with_ampersand], "&"))
        return token_with_ampersand;
    if (token_with_ampersand >= 1 && strstr(tokenized_command_and_args[token_with_ampersand-1], "&"))
        return token_with_ampersand;
    
    return -1;
}

void remove_ampersand(char** command_and_args, int index_of_ampersand)
{
    char* token_with_ampersand = command_and_args[index_of_ampersand];
    
    if (strlen(token_with_ampersand) == 1)
    {
        command_and_args[index_of_ampersand] = (char*)0;
        return;
    }
    
    while (*token_with_ampersand)
    {
        if (*token_with_ampersand == '&')
        {
            *token_with_ampersand = (char*)0;
            return;
        }
        ++token_with_ampersand;
    }
}