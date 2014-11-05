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
    char* copy = (char*)malloc(strlen(line) + 1 *sizeof(char));
    strcpy(copy, line);
    Job* job = createJob(pid, copy, ++job_identity);
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
    Node* traversal_node = jobs->head;
    if (traversal_node == NULL) return;
    
    do 
    {
        Job* job = (Job*)(traversal_node->data);
        
        job->job_state = process_state(job->pid);
        if (job->job_state == COMPLETE)
            job->exit_status = exit_status(job->pid);
    }
    while((traversal_node = traversal_node->next));
}

void signal_debugging(int pid, int status) 
{
    printf("Errno: %d\n", errno);
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
    int child_pid = waitpid(pid, &status, WUNTRACED);
//    signal_debugging(child_pid, status);
    
    //it was either a short background job or the job was killed
    /*
     This is inaccurate...I can't differentiate between a short background job that completed normally and one that was signaled
     in both cases errno says i have no child processes
     */
    if (child_pid == -1 && errno == ECHILD)
        return NORMAL;
    if (!child_pid && (WEXITSTATUS(status) || WIFSIGNALED(status))) // && WTERMSIG(status) <= SIGUNUSED && WTERMSIG(status) >= SIGHUP))
        return PROBLEM;
    else
        return NORMAL;
}

void free_jobs()
{
    freeList(jobs);
}

void remove_completed_jobs()
{
    Node* traversal_node = jobs->head;
    if (traversal_node == NULL) return;
    
    while (traversal_node != NULL)
    {
        Job* job = (Job*)(traversal_node->data);
        
        if (job->job_state == COMPLETE)
        {
            Node* copy = traversal_node->next;
            freeNode(removeNode(jobs, traversal_node), freeJob);
            traversal_node = copy;
        }
        else
        {
            traversal_node = traversal_node->next;
        }
    }
    
    if (jobs->size == 0) 
        job_identity = 0;
}
