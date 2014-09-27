#include "Job.h"

JobPtr createJob(const int pid, const char *original_command, int job_id)
{
    JobPtr newJob = (JobPtr) malloc (sizeof(Job));
    newJob->pid = pid;
    newJob->job_id = job_id;
    newJob->original_command = (char *) malloc(sizeof(char)*(strlen(original_command)+1));
    newJob->job_state = RUNNING;
    newJob->exit_status = NORMAL;
    strcpy(newJob->original_command, original_command); 
    return newJob;
}

int compareTo(const void *obj, const void *other)
{
    JobPtr o1 = (JobPtr) obj;
    JobPtr o2 = (JobPtr) other;
    return o1->pid == o2->pid;
}

char *toString(const void *obj)
{
    JobPtr myobj = (JobPtr) obj;
    char *temp;
    temp = (char *)malloc(sizeof(char)*strlen(myobj->original_command)+1+MAXPID_DIGITS+4);
    sprintf(temp, "%d (%d) %s %s %s",
            myobj->pid, 
            myobj->job_id,
            myobj->job_state == RUNNING ? "Running" : "Completed",
            myobj->exit_status == NORMAL ? "Normal" : "Problem",
            myobj->original_command);
    return temp;
}

void freeJob(const void *obj)
{
    JobPtr myobj = (JobPtr) obj;
    if (myobj == NULL) return;
    free(myobj->original_command);
    free(myobj);
}
