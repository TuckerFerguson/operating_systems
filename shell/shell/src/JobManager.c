#include "JobManager.h"

Boolean is_background_job(char** tokenized_command_and_args)
{
//    int i;
    return FALSE;
//    for (i = 0; i < )
}

Boolean start_background_job(char** tokenized_command_and_args)
{
    return FALSE;
}

void start_job(char* line, char** tokenized_command_and_args)
{
    execvp(tokenized_command_and_args[0], tokenized_command_and_args);
    err_ret("\nCouldn't execute: %s", line);
    exit(EXIT_FAILURE);
}

void print_jobs()
{
    
}
