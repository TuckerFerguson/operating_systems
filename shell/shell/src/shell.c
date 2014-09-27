#include "shell.h"

void print_command_and_args(char**);

int main(void) {
    char *line;
    char* prompt;
    pid_t pid;
    int status;
    regex_t contains_word;
    int contains_commands;
    
    if (!(prompt = getenv("DASH_PROMPT")))
    {
        prompt = "dash> ";
        fprintf(stderr, "DASH_PROMPT not set, using default prompt of dash>\n");
    }
        
    contains_commands = regcomp(&contains_word, "[\\w]*", 0);
    if (contains_commands)
       err_ret("Error compiling regex, whitespace will be interpreted as commands\n");

    using_history();
    while ((line = readline(prompt))) {

        add_history(line);
        char** tokenized_command_and_args = get_tokenized_command(line);
        
        //user hit enter
        if (tokenized_command_and_args[0] == NULL)
            continue;

        //if command was only whitespace, get another        
        contains_commands = regexec(&contains_word, line, 0, NULL, 0);
        if (contains_commands == REG_NOMATCH) 
            continue;
        
        //if the parent handled this command, get another
        if (handle_parent_commands(tokenized_command_and_args))
            continue;
        
        //if our fork fails, bail out
        if ((pid = fork()) < 0)
            err_sys("fork error");

        else if (pid == 0)
            start_job(line, tokenized_command_and_args);
        
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");
        
        free(line);
    }
    regfree(&contains_word);
    exit(EXIT_SUCCESS);
}

int handle_parent_commands(char** command_and_args)
{
    char* command = command_and_args[0];
    
    if (strstr(command,"cd") && command_and_args[1] == NULL)
    {
        change_to_home_directory();
        return 1;
    }
    else if (strstr(command,"cd") && command_and_args[1] != NULL)
    {
        change_to_directory(command_and_args[1]);
        return 1;
    }
    else if (strstr(command, "exit"))
    {
        kill(getpid(), SIGKILL);
    }
    return 0;
}

void print_command_and_args(char** tokenized_command_and_args)
{
    printf("command %s\n", tokenized_command_and_args[0]);
    int i;
    for (i = 0; i < 5; ++i) {
        printf("arg %d %s\n", i, tokenized_command_and_args[i]);
    }
}