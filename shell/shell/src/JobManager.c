#include "JobManager.h"
#include "List.h"

Boolean exec_command(char*, char**);

List* jobs;
int job_identity;

void init_job_manager()
{
    jobs = createList(compareTo, toString, freeJob);
    job_identity = 0;
}

Boolean start_background_job(char* line, char** tokenized_command_and_args, int index_of_ampersand)
{
    remove_ampersand(tokenized_command_and_args, index_of_ampersand);
    return exec_command(line, tokenized_command_and_args);
}

void log_background_job(int pid, char* line)
{
    Job* job = createJob(pid, line, ++job_identity);
    Node* node = createNode(job);
    addAtRear(jobs, node);
    job_creation_printout(job);
}

Boolean start_job(char* line, char** tokenized_command_and_args)
{
    return exec_command(line, tokenized_command_and_args);
}

Boolean exec_command(char* line, char** tokenized_command_and_args)
{
    execvp(tokenized_command_and_args[0], tokenized_command_and_args);
    err_ret("Couldn't execute: %s", line);
    return FALSE;
}

void print_jobs()
{
    if (jobs->size == 0) 
        printf("No jobs to display\n");
    else
        printList(jobs);
}

void update_completed_jobs()
{
//    int status;
    Node* traversal_node = jobs->head;
    
    if (traversal_node == NULL) return;
    
    do 
    {
        Job* job = (Job*)(traversal_node->data);
//        printf("Check if job with %d pid is done\n", job->pid);
        
        job->job_state = process_state(job->pid);
        if (job->job_state == COMPLETE)
            job->exit_status = exit_status(job->pid);
    }
    while((traversal_node = traversal_node->next));
}

void signal_debugging(int pid, int status) 
{
    if (WIFEXITED(status))
        printf("process %d exited with status %d\n", pid, WEXITSTATUS(status));
    if (WIFSIGNALED(status))
        printf("process %d was signaled with signal %d\n", pid, WTERMSIG(status));
    if (WIFSTOPPED(status))
        printf("process %d was stopped with signal %d\n", pid, WSTOPSIG(status)); 
}

int process_state(int pid)
{
    int status;
    int child_pid = waitpid(pid, &status, WNOHANG);
    if (!child_pid && WIFEXITED(status))
        return RUNNING;
    else
        return COMPLETE;
}

int exit_status(int pid)
{
    int status;
    waitpid(pid, &status, WUNTRACED);
//    signal_debugging(pid, status);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) // && WTERMSIG(status) <= SIGUNUSED && WTERMSIG(status) >= SIGHUP))
        return PROBLEM;
    else
        return NORMAL;
}

void free_jobs()
{
    freeList(jobs);
}
