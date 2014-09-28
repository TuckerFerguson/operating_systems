#include "JobManager.h"

void exec_command(char*, char**);

List* jobs;
int job_identity;

void init_job_manager()
{
    jobs = createList(compareTo, toString, freeJob);
    job_identity = 0;
}

void start_background_job(char* line, char** tokenized_command_and_args, int index_of_ampersand)
{
    remove_ampersand(tokenized_command_and_args, index_of_ampersand);
    exec_command(line, tokenized_command_and_args);
}

void log_background_job(int pid, char* line)
{
    Job* job = createJob(pid, line, ++job_identity);
    Node* node = createNode(job);
    addAtRear(jobs, node);
    job_creation_printout(job);
}

void start_job(char* line, char** tokenized_command_and_args)
{
    exec_command(line, tokenized_command_and_args);
}

void exec_command(char* line, char** tokenized_command_and_args)
{
    execvp(tokenized_command_and_args[0], tokenized_command_and_args);
    err_sys("\nCouldn't execute: %s\n", line);
    exit(EXIT_FAILURE);
}

void print_jobs()
{
    if (jobs ->size == 0) printf("No jobs to display\n");
    printList(jobs);
}
